#include "world.h"

#include "raymath.h"
#include <algorithm>
#include <numbers>
#include <random>
#include <ranges>

namespace {
static std::random_device seeder;
static std::mt19937 generator(seeder());

template <typename IntType>
IntType createUniformlyDistributedInt(IntType lower, IntType upper)
{
    return std::uniform_int_distribution<IntType>(lower, upper)(generator);
}

template <typename RealType>
RealType createUniformlyDistributedReal(RealType lower, RealType upper)
{
    return std::uniform_real_distribution<RealType>(lower, upper)(generator);
}

Vector2 createRandomPositionForBallWithRadius(float radius, const Vector2& limits)
{
    return { createUniformlyDistributedReal(radius, limits.x - radius), createUniformlyDistributedReal(radius, limits.y - radius) };
}

Vector2 createRandomDirection()
{
    const auto angle = createUniformlyDistributedReal(0.f, 2 * std::numbers::pi_v<float>);
    return { std::cos(angle), std::sin(angle) };
}

Color createRandomColor()
{
    const auto r = static_cast<unsigned char>(createUniformlyDistributedInt(0, 255));
    const auto g = static_cast<unsigned char>(createUniformlyDistributedInt(0, 255));
    const auto b = static_cast<unsigned char>(createUniformlyDistributedInt(0, 255));
    return { r, g, b, 255 };
}

Ball createRandomBall(const World& world)
{
    constexpr float minRadius = 8.f;
    constexpr float maxRadius = 64.f;

    const auto radius = createUniformlyDistributedReal(minRadius, maxRadius);
    const auto position = createRandomPositionForBallWithRadius(radius, world.limits);

    constexpr float minSpeed = 32.f;
    constexpr float maxSpeed = 256.f;

    const auto velocity = createRandomDirection() * createUniformlyDistributedReal(minSpeed, maxSpeed);
    const auto color = createRandomColor();

    return Ball { position, velocity, radius, color };
}

bool areColliding(const Ball& lhs, const Ball& rhs)
{
    return CheckCollisionCircles(lhs.position, lhs.radius, rhs.position, rhs.radius);
}

std::optional<Ball> createRandomNonOverlappingBall(const World& world)
{
    for ([[maybe_unused]] auto _ : std::views::iota(0, 50)) {
        const auto ball = createRandomBall(world);
        const auto collides = [&ball](const Ball& existingBall) {
            return areColliding(ball, existingBall);
        };

        if (!std::ranges::any_of(world.balls, collides)) {
            return ball;
        }
    }

    return std::nullopt;
}

float computeCircleArea(float radius)
{
    return std::numbers::pi_v<float> * radius * radius;
}

float computeMass(const Ball& ball)
{
    return computeCircleArea(ball.radius);
}

Vector2 computeNewVelocityOfLhsAfterElasticCollision(const Ball& lhs, const Ball& rhs)
{
    const auto m1 = computeMass(lhs);
    const auto m2 = computeMass(rhs);
    const auto positionDelta = lhs.position - rhs.position;
    const auto velocityDelta = lhs.velocity - rhs.velocity;
    const auto factor = (2 * m2) / (m1 + m2) * Vector2DotProduct(velocityDelta, positionDelta) / Vector2LengthSqr(positionDelta);

    return lhs.velocity - positionDelta * factor;
}

void pushApart(Ball& lhs, Ball& rhs)
{
    const auto displacement = Vector2Normalize(lhs.position - rhs.position) * 0.5f;
    lhs.position += displacement;
    rhs.position -= displacement;
}

template <typename BallIterator>
void separate(BallIterator begin, BallIterator end)
{
    while (true) {
        auto change = false;

        for (auto lhs = begin; lhs != end; ++lhs) {
            for (auto rhs = std::next(lhs); rhs != end; ++rhs) {
                while (areColliding(*lhs, *rhs)) {
                    pushApart(*lhs, *rhs);
                    change = true;
                }
            }
        }

        if (!change) {
            break;
        }
    }
}
}

void update(World& world, float elapsedTimeInSeconds)
{
    if (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsGestureDetected(GESTURE_TAP)) {
        const auto randomBall = createRandomNonOverlappingBall(world);
        if (randomBall.has_value()) {
            world.balls.push_back(*randomBall);
        }
    }

    for (auto ball = world.balls.begin(); ball != world.balls.end(); ++ball) {
        for (auto other = std::next(ball); other != world.balls.end(); ++other) {
            if (areColliding(*ball, *other)) {
                const auto newVelocityBall = computeNewVelocityOfLhsAfterElasticCollision(*ball, *other);
                const auto newVelocityOther = computeNewVelocityOfLhsAfterElasticCollision(*other, *ball);
                ball->velocity = newVelocityBall;
                other->velocity = newVelocityOther;
            }
        }
    }

    separate(world.balls.begin(), world.balls.end());

    for (auto& ball : world.balls) {
        ball.position += ball.velocity * elapsedTimeInSeconds;

        if (ball.position.x - ball.radius <= 0.f) {
            ball.position.x = ball.radius;
            ball.velocity.x = std::abs(ball.velocity.x);
        }
        if (ball.position.x + ball.radius >= world.limits.x) {
            ball.position.x = world.limits.x - ball.radius;
            ball.velocity.x = -std::abs(ball.velocity.x);
        }
        if (ball.position.y - ball.radius <= 0.f) {
            ball.position.y = ball.radius;
            ball.velocity.y = std::abs(ball.velocity.y);
        }
        if (ball.position.y + ball.radius >= world.limits.y) {
            ball.position.y = world.limits.y - ball.radius;
            ball.velocity.y = -std::abs(ball.velocity.y);
        }
    }
}
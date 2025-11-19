#include "world.h"

#include "raymath.h"
#include <algorithm>
#include <numbers>
#include <random>
#include <ranges>

namespace {
static std::mt19937 generator;

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
    const auto color = createRandomColor();

    return Ball { position, radius, color };
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
}

void update(World& world, float)
{
    if (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsGestureDetected(GESTURE_TAP)) {
        const auto randomBall = createRandomNonOverlappingBall(world);
        if (randomBall.has_value()) {
            world.balls.push_back(*randomBall);
        }
    }
}
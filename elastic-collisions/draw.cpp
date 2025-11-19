#include "draw.h"
#include "world.h"

#include "raymath.h"
#include <algorithm>

void draw(const World& world)
{
    const auto screenWidth = GetScreenWidth();
    const auto screenHeight = GetScreenHeight();
    const auto scalingFactor = std::min(screenWidth / world.limits.x, screenHeight / world.limits.y);
    const auto spareWidth = screenWidth - scalingFactor * world.limits.x;
    const auto spareHeight = screenHeight - scalingFactor * world.limits.y;
    const Vector2 offset { spareWidth / 2.f, spareHeight / 2.f };

    BeginDrawing();

    ClearBackground(BLACK);
    DrawRectangleV(offset, world.limits * scalingFactor, SKYBLUE);

    for (const auto& ball : world.balls) {
        DrawCircleV(ball.position * scalingFactor + offset, ball.radius * scalingFactor, ball.color);
    }

    EndDrawing();
}
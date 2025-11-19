#include "raylib.h"
#include "raymath.h"

#include <algorithm>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

struct World {
    explicit World(float width, float height)
        : limits { width, height }
    {
    }

    Vector2 limits;
};

void updateAndDraw(void* worldTypeErased)
{
    const auto& world = *static_cast<World*>(worldTypeErased);
    const auto screenWidth = GetScreenWidth();
    const auto screenHeight = GetScreenHeight();
    const auto scalingFactor = std::min(screenWidth / world.limits.x, screenHeight / world.limits.y);
    const auto spareWidth = screenWidth - scalingFactor * world.limits.x;
    const auto spareHeight = screenHeight - scalingFactor * world.limits.y;
    const Vector2 offset { spareWidth / 2.f, spareHeight / 2.f };

    BeginDrawing();

    ClearBackground(BLACK);
    DrawRectangleV(offset, world.limits * scalingFactor, SKYBLUE);

    EndDrawing();
}

int main()
{
    World world(1920.f, 1080.f);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(world.limits.x, world.limits.y, "Demo");

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop_arg(updateAndDraw, static_cast<void*>(&world), 0, true);
#else
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        updateAndDraw(static_cast<void*>(&world));
    }
#endif

    CloseWindow();
}
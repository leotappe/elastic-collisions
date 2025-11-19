#include "draw.h"
#include "world.h"

#include "raylib.h"
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

void updateAndDraw(void* worldTypeErased)
{
    auto& world = *static_cast<World*>(worldTypeErased);
    update(world, GetFrameTime());
    draw(world);
}

int main()
{
    World world(1920.f, 1080.f);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(world.limits.x, world.limits.y, "Elastic Collisions");

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
#ifndef ELASTIC_COLLISIONS_WORLD_H
#define ELASTIC_COLLISIONS_WORLD_H

#include "raylib.h"
#include <vector>

struct Ball {
    explicit Ball(Vector2 position, float radius, Color color)
        : position(position)
        , radius(radius)
        , color(color)
    {
    }

    Vector2 position;
    float radius;
    Color color;
};

struct World {
    explicit World(float width, float height)
        : limits { width, height }
    {
        balls.emplace_back(Vector2 { width / 2.f, height / 2.f }, 16.f, YELLOW);
    }

    Vector2 limits;
    std::vector<Ball> balls;
};

void update(World& world, float elapsedTimeInSeconds);

#endif
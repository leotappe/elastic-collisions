#ifndef ELASTIC_COLLISIONS_WORLD_H
#define ELASTIC_COLLISIONS_WORLD_H

#include "raylib.h"
#include <vector>

struct Ball {
    explicit Ball(Vector2 position, Vector2 velocity, float radius, Color color)
        : position(position)
        , velocity(velocity)
        , radius(radius)
        , color(color)
    {
    }

    Vector2 position;
    Vector2 velocity;
    float radius;
    Color color;
};

struct World {
    explicit World(float width, float height)
        : limits { width, height }
    {
    }

    Vector2 limits;
    std::vector<Ball> balls;
};

void update(World& world, float elapsedTimeInSeconds);

#endif
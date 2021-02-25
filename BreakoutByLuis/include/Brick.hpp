#ifndef BRICK_H
#define BRICK_H

#include <Math.hpp>
#include <SDL2/SDL.h>
// #include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

const int BRICK_WIDTH = 50;
const int BRICK_HEIGHT = 10;

class Brick
{
public:
    Brick(Vec2 position, Vec2 velocity)
        : position(position), velocity(velocity)
    {
        rect.x = static_cast<int>(position.x);
        rect.y = static_cast<int>(position.y);
        rect.w = BRICK_WIDTH;
        rect.h = BRICK_HEIGHT;
        state = 0;
    }

    Brick(Vec2 position, Vec2 velocity, int stateI)
        : position(position), velocity(velocity)
    {
        rect.x = static_cast<int>(position.x);
        rect.y = static_cast<int>(position.y);
        rect.w = BRICK_WIDTH;
        rect.h = BRICK_HEIGHT;
        state = stateI;
    }

    void Update(float dt)
    {
        position += velocity * dt;
    }

    void Draw(SDL_Renderer *renderer)
    {
        if (state == 0)
        {
            rect.x = static_cast<int>(position.x);
            rect.y = static_cast<int>(position.y);
        }
        else
        {
            rect.x = 10000; //sent of off screen
        }
        SDL_RenderFillRect(renderer, &rect);
    }

    Brick *Break()
    {
        state = 1;
        return this;
    }

    Vec2 position;
    Vec2 velocity;
    SDL_Rect rect{};
    int state;
};

#endif
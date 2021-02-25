#ifndef PADDLE_H
#define PADDLE_H

#include <Math.hpp>
#include <SDL2/SDL.h>
// #include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

const int PADDLE_WIDTH = 100;
const int PADDLE_HEIGHT = 10;
const float PADDLE_SPEED = 1.0f;

class Paddle
{
public:
	Paddle(Vec2 position, Vec2 velocity)
		: position(position), velocity(velocity)
	{
		rect.x = static_cast<int>(position.x);
		rect.y = static_cast<int>(position.y);
		rect.w = PADDLE_WIDTH;
		rect.h = PADDLE_HEIGHT;
	}

	void Update(float dt)
	{
		position += velocity * dt;
	}

	void Draw(SDL_Renderer *renderer)
	{
		rect.x = static_cast<int>(position.x);

		SDL_RenderFillRect(renderer, &rect);
	}

	Vec2 position;
	Vec2 velocity;
	SDL_Rect rect{};
};

#endif
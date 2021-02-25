#ifndef LABEL_H
#define LABEL_H

#include <SDL2/SDL.h>
// #include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <Math.hpp>

class Label
{
public:
	Label(Vec2 position, SDL_Renderer *renderer, TTF_Font *font)
		: renderer(renderer), font(font)
	{
		if (!font)
		{
			printf("TTF_OpenFont: %s\n", TTF_GetError());
		}
		else
		{
			surface = TTF_RenderText_Solid(font, "0", {0xFF, 0xFF, 0xFF, 0xFF});
			texture = SDL_CreateTextureFromSurface(renderer, surface);

			int width, height;
			SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

			rect.x = static_cast<int>(position.x);
			rect.y = static_cast<int>(position.y);
			rect.w = width;
			rect.h = height;
		}
	}

	Label(Vec2 position, SDL_Renderer *renderer, TTF_Font *font, const char * message)
		: renderer(renderer), font(font)
	{
		if (!font)
		{
			printf("TTF_OpenFont: %s\n", TTF_GetError());
		}
		else
		{
			surface = TTF_RenderText_Solid(font, message, {0xFF, 0xFF, 0xFF, 0xFF});
			texture = SDL_CreateTextureFromSurface(renderer, surface);

			int width, height;
			SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

			int offset = strlen(message);
			float offsetPerCharacter = 10;

			rect.x = static_cast<int>(position.x-(offset*offsetPerCharacter));
			rect.y = static_cast<int>(position.y);
			rect.w = width;
			rect.h = height;
		}
	}

	~Label()
	{
		// SDL_FreeSurface(surface);
		//SDL_DestroyTexture(texture);
	}

	void SetScore(int score)
	{
		// SDL_FreeSurface(surface);
		// SDL_DestroyTexture(texture);
		const char * score_txt = std::to_string(score).c_str();
		surface = TTF_RenderText_Solid(font, score_txt, {0xFF, 0xFF, 0xFF, 0xFF});
		texture = SDL_CreateTextureFromSurface(renderer, surface);

		int width, height;
		SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
		rect.w = width;
		rect.h = height;
	}

	void SetMessage(const char * message)
	{
		// SDL_FreeSurface(surface);
		// SDL_DestroyTexture(texture);
		surface = TTF_RenderText_Solid(font, message, {0xFF, 0xFF, 0xFF, 0xFF});
		texture = SDL_CreateTextureFromSurface(renderer, surface);

		int width, height;
		SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
		rect.w = width;
		rect.h = height;
	}

	void Draw()
	{
		SDL_RenderCopy(renderer, texture, nullptr, &rect);
	}

	SDL_Renderer *renderer;
	TTF_Font *font;
	SDL_Surface *surface{};
	SDL_Texture *texture{};
	SDL_Rect rect{};
};

#endif
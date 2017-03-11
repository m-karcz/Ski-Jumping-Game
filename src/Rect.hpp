#pragma once
#include <SDL2/SDL.h>
#include "SDL_Wrapper.hpp"

namespace BetterSDL
{
	SDL_Rect* SDL_RectFactory(int x, int y, int w, int h);
	class Texture;
	class Rect
	{
	public:
		Rect() : ptr(SDL_RectFactory(0,0,0,0)){}
		Rect(int x, int y, int width, int height) : ptr(SDL_RectFactory(x, y, width, height)) {}
		Rect(const Vector2d<int>& location) : ptr(SDL_RectFactory(get_x(location), get_y(location), 0, 0)) {}
		Rect(const Vector2d<int>& location, const Vector2d<int>& size) : ptr(SDL_RectFactory(get_x(location), get_y(location), get_x(size), get_y(size))) {}
		std::unique_ptr<SDL_Rect> ptr;
		operator SDL_Rect*() const
		{
			return ptr.get();
		}
		void parseTexture(Texture& texture);
	};

}

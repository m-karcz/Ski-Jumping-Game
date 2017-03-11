#pragma once
#include <SDL2/SDL.h>
#include "../../Simple-Algebra-Library/vectors2d.hpp"
#include <memory>

namespace BetterSDL
{
	using MyAlgLib::Vector2d;
	using MyAlgLib::get_x;
	using MyAlgLib::get_y;

	struct SDL_Destroyer
	{
		void operator()(SDL_Window* window) const
		{
			SDL_DestroyWindow(window);	
		}
		void operator()(SDL_Renderer* renderer) const
		{
			SDL_DestroyRenderer(renderer);
		}
		void operator()(SDL_Surface* surface) const
		{
			SDL_FreeSurface(surface);
		}
		void operator()(SDL_Texture* texture) const
		{
			SDL_DestroyTexture(texture);
		}
	};

	template<typename T>
	using SDL_unique_ptr = std::unique_ptr<T, SDL_Destroyer>;

	template<typename T>
	class SDL_Wrapper
	{
	public:
		SDL_unique_ptr<T> ptr=nullptr;
		operator T*() const
		{
			return ptr.get();
		}
	};
}

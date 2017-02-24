#pragma once
#include <SDL2/SDL.h>
#include <memory>

namespace SkiJump
{
	class Numbers
	{
	public:
		static void draw_number(SDL_Renderer* renderer, double value, int x, int y);
	private:
		static std::shared_ptr<SDL_Texture> numbers_texture;
		static void init_textures(SDL_Renderer* renderer);
		static int w;
		static int h;
		static std::unique_ptr<SDL_Rect> location;
		static std::unique_ptr<SDL_Rect> number;
	};
}

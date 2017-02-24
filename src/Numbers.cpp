#include "Numbers.hpp"
#include <stack>
#include <sstream>
#include <iomanip>
#include <iostream>

namespace SkiJump
{
	std::shared_ptr<SDL_Texture> Numbers::numbers_texture;
	std::unique_ptr<SDL_Rect> Numbers::location = std::make_unique<SDL_Rect>();
	std::unique_ptr<SDL_Rect> Numbers::number = std::make_unique<SDL_Rect>();
	int Numbers::w;
	int Numbers::h;

	void Numbers::init_textures(SDL_Renderer* renderer)
	{
		SDL_Surface* temp_surf=SDL_LoadBMP("img/numbers.bmp");
		if(temp_surf!=nullptr)
		{
			SDL_SetColorKey(temp_surf, SDL_TRUE, SDL_MapRGB(temp_surf->format, 100, 100, 150));
			numbers_texture=std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, temp_surf), SDL_DestroyTexture);
			SDL_FreeSurface(temp_surf);
			SDL_QueryTexture(numbers_texture.get(), nullptr, nullptr, &w, &h);
			w/=12;
		}
	}

	void Numbers::draw_number(SDL_Renderer* renderer, double value, int x, int y)
	{
		if(!numbers_texture)
			init_textures(renderer);
		int n=static_cast<int>(value);
		location->w = number->w = w;
		location->h = number->h = h;
		location->x = x;
		location->y = y;
		std::stringstream ss;
		ss << std::setprecision(1) << std::fixed << value;
		for(char c : ss.str())
		{
			if(c <= '9' && c >= '0')
			{
				number->x = (c - '0') * w;
				SDL_RenderCopy(renderer, numbers_texture.get(), number.get(), location.get());
				location->x += w;
			}
			else
			{
				if( c == '.')
				{
					number->x = 10 * w;
					SDL_RenderCopy(renderer, numbers_texture.get(), number.get(), location.get());
					location->x += w;
				}
				if( c== '-' )
				{
					number->x = 11 * w;
					location->x -= w;
					SDL_RenderCopy(renderer, numbers_texture.get(), number.get(), location.get());
					location->x += w;
				}
			}
		}
	}
}

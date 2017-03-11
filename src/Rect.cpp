#include "Rect.hpp"
#include "Texture.hpp"


namespace BetterSDL
{
	SDL_Rect* SDL_RectFactory(int x, int y, int w, int h)
	{
		auto* ptr=new SDL_Rect();
		ptr->x = x;
		ptr->y = y;
		ptr->w = w;
		ptr->h = h;
		return ptr;
	}
	void Rect::parseTexture(Texture& texture)
	{
		SDL_QueryTexture(texture, nullptr, nullptr, &(ptr->w), &(ptr->h));
	}

}

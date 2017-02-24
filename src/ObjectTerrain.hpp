#include "SplineTerrain.hpp"


namespace SkiJump
{
	class ObjectTerrain : public SplineTerrain
	{
	public:
		using SplineTerrain::SplineTerrain;
		void draw_range(SDL_Renderer* renderer, MyAlgLib::Vector2d<>& offset, int from, int to) const override
		{
			SDL_Rect rect1;
			SDL_Rect rect2;
			rect1.x=0;
			rect1.y=0;
			SDL_QueryTexture(texture, nullptr, nullptr, &rect1.w, &rect1.h);
			rect2.h=rect1.w/2;
			rect2.w=rect1.h/2;
			rect2.x=MyAlgLib::get_x(offset)-25;
			rect2.y=MyAlgLib::get_y(offset)-225;
			SDL_RenderCopy(renderer, texture, &rect1, &rect2);
		}
			
	}; 
}

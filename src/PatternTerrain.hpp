#include "SplineTerrain.hpp"

namespace SkiJump
{
	class PatternTerrain : public SplineTerrain
	{
	public:
		using SplineTerrain::SplineTerrain;
		void draw_range(SDL_Renderer* renderer, MyAlgLib::Vector2d<>& offset, int from, int to) const override
		{
			SDL_Rect rect;
			SDL_Point point;
			point.x=0;
			point.y=100;
			SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
			rect.w*=2;
			rect.h*=1;
			int y_offset=MyAlgLib::get_y(offset)-rect.h/2;
			for(int x=from-60; x < to; x++)	
			{
				int xx=x-MyAlgLib::get_x(offset);
				if(does_exist(xx))
				{
					rect.x=x-rect.w/2;
					rect.y=-get_height(xx)+y_offset;
					SDL_RenderCopyEx(renderer, texture, nullptr, &rect, MyAlgLib::rad2deg(std::atan(0.5)), &point, SDL_FLIP_NONE);
				}
			}
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			int y=-get_height(766)+MyAlgLib::get_y(offset);
			SDL_RenderDrawLine(renderer, 734+MyAlgLib::get_x(offset), y+64, 798+MyAlgLib::get_x(offset), y-64);
		}
	};
}

#include "Numbers.hpp"

namespace Numbers
{
	void draw(BetterSDL::Renderer& renderer, double value, int x, int y, int precision)
	{
		class NumbersDrawer : public SimpleDrawable
		{
		public:
			NumbersDrawer(BetterSDL::Renderer& renderer) : SimpleDrawable{"img/numbers.bmp"}
			{
				destination.parseTexture(getTexture(renderer));
				gap = destination.ptr->w / charsAmount;
				destination.ptr->w = gap;
				source.ptr->w = gap;
				source.ptr->h = destination.ptr->h;
			}
			void draw(BetterSDL::Renderer& renderer) override
			{
					renderer.simpleDraw(getTexture(renderer), source, destination);
			}
			void setChar(char c)
			{
				if(c >= '0' && c <= '9')
					source.ptr->x = gap * (c - '0');
				if(c == '.')
					source.ptr->x = gap * 10;
				if(c == '-')
					source.ptr->x = gap * 11;
			}
			BetterSDL::Rect destination;
			BetterSDL::Rect source;
			int gap=0;
			const int charsAmount{12};
		};
		static NumbersDrawer drawer(renderer);
		drawer.destination.ptr->y = y;
		std::stringstream ss;
		ss << std::setprecision(precision) << std::fixed << value;
		drawer.destination.ptr->x = x - drawer.gap * ss.str().size();
		for(char c : ss.str())
		{
			drawer.setChar(c);
			drawer.draw(renderer);
			drawer.destination.ptr->x += drawer.gap;
		}
	}
}

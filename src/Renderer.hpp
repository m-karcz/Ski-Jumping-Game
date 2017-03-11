#pragma once
#include "SDL_Wrapper.hpp"
#include "Window.hpp"
#include "Texture.hpp"
#include "Rect.hpp"

namespace BetterSDL
{
	class Texture;
	class Renderer : public SDL_Wrapper<SDL_Renderer>
	{
	public:
		Renderer(Window& window);
		void simpleDraw(Texture& texture, const Rect& destination);
		void simpleDraw(Texture& texture, const Rect& destination, const Rect& source);
		void clear();
		void show();
		/*void rotateDraw(const std::unique_ptr<Texture>& texture, const SDL_Rect& destination, const SDL_Rect& source, const SDL_Point& rotation, const double deg_angle)
		{
			
		}*/
	private:
	};
}

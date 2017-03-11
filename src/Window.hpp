#pragma once
#include "SDL_Wrapper.hpp"

namespace BetterSDL
{
	class Window : public SDL_Wrapper<SDL_Window>
	{
	public:
		Window(const std::string& title, const int x, const int y, const int w, const int h);
	private:
	};
}

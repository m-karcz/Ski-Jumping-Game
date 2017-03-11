#include "Window.hpp"

namespace BetterSDL
{
	Window::Window(const std::string& title, const int x, const int y, const int w, const int h)
	{
		ptr=SDL_unique_ptr<SDL_Window>(SDL_CreateWindow(title.c_str(), x, y, w, h, SDL_WINDOW_SHOWN));
	}
}

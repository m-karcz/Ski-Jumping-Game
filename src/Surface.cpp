#include "Surface.hpp"

namespace BetterSDL
{
	Surface::Surface(const std::string& path)
	{
		ptr=SDL_unique_ptr<SDL_Surface>(SDL_LoadBMP(path.c_str()));
	}
}

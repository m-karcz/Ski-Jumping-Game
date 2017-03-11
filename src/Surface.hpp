#pragma once
#include "SDL_Wrapper.hpp"

namespace BetterSDL
{
	class Surface : public SDL_Wrapper<SDL_Surface>
	{
	public:
		Surface(const std::string& path);
	};

}

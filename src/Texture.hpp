#pragma once
#include "SDL_Wrapper.hpp"

namespace BetterSDL
{
	struct Color
	{
		Color(uint8_t R, uint8_t G, uint8_t B) : R(R), G(G), B(B) {}
		uint8_t R;
		uint8_t G;
		uint8_t B;
	};
	class Renderer;
	class Surface;
	class Texture : public SDL_Wrapper<SDL_Texture>
	{
	public:
		Texture(Renderer& renderer, Surface& surface, Color transparent={100, 100, 150});
		Texture(Renderer& renderer, const std::string& path, Color transparent={100, 100, 150});
		void setTransparency(uint8_t value);
	};
}

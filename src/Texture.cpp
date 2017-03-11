#include "Texture.hpp"
#include "Surface.hpp"
#include "Renderer.hpp"


namespace BetterSDL
{

	Texture::Texture(Renderer& renderer, Surface& surface, Color transparent)
	{	
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface.ptr->format, transparent.R, transparent.G, transparent.B));
		ptr=SDL_unique_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, surface));
	}
	Texture::Texture(Renderer& renderer, const std::string& path, Color transparent)
	{
		Surface surface(path);
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface.ptr->format, transparent.R, transparent.G, transparent.B));
		ptr=SDL_unique_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, surface));
	}
	void Texture::setTransparency(uint8_t value)
	{
		SDL_SetTextureAlphaMod(*this, value);
	}
}

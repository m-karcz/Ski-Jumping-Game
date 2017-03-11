#include "Renderer.hpp"
#include "Texture.hpp"

namespace BetterSDL
{
	Renderer::Renderer(Window& window)
	{
		ptr=SDL_unique_ptr<SDL_Renderer>(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED));
	}
	void Renderer::simpleDraw(Texture& texture, const Rect& destination)
	{
		SDL_RenderCopy(*this, texture,	nullptr, destination);
	}
	void Renderer::simpleDraw(Texture& texture, const Rect& source, const Rect& destination)
	{
		SDL_RenderCopy(*this, texture, source, destination);
	}
	void Renderer::show()
	{
		SDL_RenderPresent(*this);
	}
	void Renderer::clear()
	{
		SDL_RenderClear(*this);
	}
}

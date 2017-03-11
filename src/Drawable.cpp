#include "Drawable.hpp"
#include <algorithm>
#include <functional>


BetterSDL::Texture& SimpleDrawable::getTexture(BetterSDL::Renderer& renderer)
{
	if(!texture)
	{
		texture=std::make_unique<BetterSDL::Texture>(renderer, path);
	}
	return *texture;
}


BetterSDL::Texture& ComplexDrawable::getTexture(BetterSDL::Renderer& renderer, std::size_t index)
{
	if(!std::all_of(textures.begin(), textures.end(), [](auto& ptr){return static_cast<bool>(ptr);}));
	{
		textures.resize(paths.size());
		for(std::size_t i=0; i < paths.size(); i++)
		{
			textures[i] = std::make_unique<BetterSDL::Texture>(renderer, paths[i]);
		}
	}
	return *(textures[index]);
}

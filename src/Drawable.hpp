#pragma once
#include "BetterSDL.hpp"
#include <vector>
#include <memory>

class Drawable
{
public:
	virtual void draw(BetterSDL::Renderer& renderer) = 0;
protected:
//	virtual void initializeTexture(const std::unique_ptr<BetterSDL::Renderer>& renderer) = 0;
};

class SimpleDrawable : Drawable
{
protected:
	SimpleDrawable(const std::string& path) : path(path) {}
	std::string path="";
	BetterSDL::Texture& getTexture(BetterSDL::Renderer& renderer);
private:
	std::unique_ptr<BetterSDL::Texture> texture;
};

class ComplexDrawable : Drawable
{
protected:
	ComplexDrawable(std::initializer_list<std::string>&& paths) : paths(std::forward<std::initializer_list<std::string>>(paths)) {}
	std::vector<std::string> paths;
	BetterSDL::Texture& getTexture(BetterSDL::Renderer& renderer, std::size_t index);
private:
	std::vector<std::unique_ptr<BetterSDL::Texture>> textures;
};

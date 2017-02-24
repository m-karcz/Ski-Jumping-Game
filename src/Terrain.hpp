#pragma once
#include "SplineTerrain.hpp"
#include "PatternTerrain.hpp"
#include "ObjectTerrain.hpp"
#include "Jumper.hpp"
#include <memory>


namespace SkiJump
{
	class Terrain
	{
	public:
		Terrain(SDL_Renderer* renderer);
		void draw(SDL_Renderer* renderer, MyAlgLib::Vector2d<>& offset);
		void draw_shadow(SDL_Renderer* renderer, std::shared_ptr<Jumper> jumper);
		double height(unsigned int i);
		double slope(unsigned int i);
		double angle(unsigned int i);
		double height(double x);
		double slope(double x);
		double angle(double x);
		double get_distance(int x);
	private:
		std::unique_ptr<SplineTerrain> hill;
		std::unique_ptr<SplineTerrain> landing;
		SDL_Texture* shadow;
	};
}

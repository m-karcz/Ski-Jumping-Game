#pragma once
#include "SplineTerrain.hpp"
#include "Drawable.hpp"

class InRun : public SimpleDrawable, public SplineTerrain
{
public:
	InRun() : SplineTerrain(MyAlgLib::Vector2d_factory(0, 0), MyAlgLib::Vector2d_factory(0.0, 10.0), 2.0, MyAlgLib::Vector2d_factory(5.0, 1.0), 17.0/120.0, -61.0/15.0, -17.0/180.0, 11.0/60.0, MyAlgLib::Vector2d_factory(40.0, 20.0)), SimpleDrawable("img/hill2.bmp")
	{
		
	}
	void draw(BetterSDL::Renderer& renderer) override
	{}
};

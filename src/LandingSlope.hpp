#pragma once
#include "SplineTerrain.hpp"
#include "Drawable.hpp"


class LandingSlope : public SplineTerrain, public SimpleDrawable
{
public:
	LandingSlope() : SplineTerrain(MyAlgLib::Vector2d_factory(200, -10), MyAlgLib::Vector2d_factory(0.0, 0.0), 5.0, MyAlgLib::Vector2d_factory(20.0, -10.0), 1.0/300.0, -13.0/12.0, -1.0/900.0, -1.0/12.0, MyAlgLib::Vector2d_factory(80.0, 30.0)), SimpleDrawable("img/slope.bmp")
	{
			
	}
	void draw(BetterSDL::Renderer& renderer) override
	{}
};

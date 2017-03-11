#pragma once
#include "Drawable.hpp"
#include "SplineTerrain.hpp"
#include <array>

class Terrain : public SimpleDrawable
{
public:
	Terrain();
	void draw(BetterSDL::Renderer& renderer) override;
	double getDistance(double x) const;
	double getSlope(double x) const;
	double getHeight(double x) const;
	bool contains(double x) const;
private:
	std::array<std::unique_ptr<SplineTerrain>, 2> data;
};

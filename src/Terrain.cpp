#include "Terrain.hpp"
#include "LandingSlope.hpp"
#include "InRun.hpp"
#include <stdexcept>
#include <numeric>
#include <algorithm>

Terrain::Terrain() : SimpleDrawable("")
{
	data[0] = std::make_unique<InRun>();
	data[1] = std::make_unique<LandingSlope>();
}

void Terrain::draw(BetterSDL::Renderer& renderer)
{
	for(auto& terrain_ptr : data)
		terrain_ptr->draw(renderer);
}


double Terrain::getDistance(double x) const
{
	for(auto& terrain_ptr : data)
		if(terrain_ptr->contains(x))
			return terrain_ptr->getDistance(x);
	throw std::out_of_range{""};
}
double Terrain::getSlope(double x) const
{
	for(auto& terrain_ptr : data)
		if(terrain_ptr->contains(x))
			return terrain_ptr->getSlope(x);
	throw std::out_of_range{""};
}
double Terrain::getHeight(double x) const
{
	for(auto& terrain_ptr : data)
		if(terrain_ptr->contains(x))
			return terrain_ptr->getHeight(x);
	throw std::out_of_range{""};
}
bool Terrain::contains(double x) const
{
	return std::any_of(data.begin(), data.end(), [=](const auto& terrain_ptr){return terrain_ptr->contains(x);});
}


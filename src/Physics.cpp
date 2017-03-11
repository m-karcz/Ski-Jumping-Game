#include "Physics.hpp"
#include <cmath>

namespace Physics
{
	Wind::Wind()
	{
		init();
	}
	void Wind::init()
	{
		value = Random::getReal(min, max); 
	}
	void Wind::tick()
	{
		value += Random::getReal(min/10.0, max/10.0);
	}
	double Wind::getValue()
	{
		return value;
	}

	Wind wind;

	MyAlgLib::Vector2d<> rampForce(Jumper& jumper, Terrain& terrain)
	{
		const double gravityLength = MyAlgLib::length(gravity);
		double angle = std::tan(terrain.getSlope(get_x(jumper.location)));
		double actualLength = gravityLength * std::sin(angle);
		return MyAlgLib::Vector2d_factory(actualLength * std::sin(angle), actualLength * std::cos(angle));
	}

	MyAlgLib::Vector2d<> airResistance(Jumper& jumper)
	{
		const double coefficient = 0.002;
		const double body_angle = MyAlgLib::deg2rad(jumper.angles[0] + jumper.angles[1]);
		const double velocity_angle = MyAlgLib::arg(jumper.velocity);
		return -1.0 * coefficient * MyAlgLib::length(jumper.velocity) * std::abs(std::sin(body_angle - velocity_angle)) * jumper.velocity;
	}

	MyAlgLib::Vector2d<> airForce(Jumper& jumper)
	{
		const double coefficient = 0.02;
		const double body_angle = MyAlgLib::deg2rad(jumper.angles[0] + jumper.angles[1]);
		return coefficient * std::sin(body_angle) * wind.getValue() * MyAlgLib::Vector2d_factory(std::sin(body_angle), std::cos(body_angle));
	}

	void updateOnTerrain(Jumper& jumper, Terrain& terrain)
	{
		get_y(jumper.location) = terrain.getHeight(get_x(jumper.location));
	}

	void setForcesOnRamp(Jumper& jumper, Terrain& terrain)
	{
		jumper.velocity += dt * rampForce(jumper, terrain);
		jumper.location += dt * jumper.velocity;
		updateOnTerrain(jumper, terrain);
	}

	void setForcesAfterJump(Jumper& jumper)
	{
		jumper.velocity = jumper.velocity + dt * (airForce(jumper) + airResistance(jumper) - gravity);
		jumper.location += dt * jumper.velocity;
	}

	void setForcesInFlight(Jumper& jumper)
	{
		jumper.velocity = jumper.velocity + dt * (gravity + airForce(jumper) + airResistance(jumper));
		jumper.location += dt * jumper.velocity;
	}

	void setForcesAfterLand(Jumper& jumper, Terrain& terrain)
	{
		get_x(jumper.velocity) -= 1.0;
		if(get_x(jumper.velocity) < 0.0)
			get_x(jumper.velocity) = 0.0;
		updateOnTerrain(jumper, terrain);
	}



}

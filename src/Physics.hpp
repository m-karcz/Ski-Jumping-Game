#pragma once
#include "../../Simple-Algebra-Library/vectors2d.hpp"
#include "Jumper.hpp"
#include "Terrain.hpp"
#include "Random.hpp"

namespace Physics
{
	const MyAlgLib::Vector2d<> gravity = MyAlgLib::Vector2d_factory(0.0, -1.0);
	const double dt = 0.1;
	void setForcesOnRamp(Jumper& jumper, Terrain& terrain);
	void setForcesAfterJump(Jumper& jumper);
	void setForcesInFlight(Jumper& jumper);
	void setForcesAfterLand(Jumper& jumper, Terrain& terrain);
	MyAlgLib::Vector2d<> airForce(Jumper& jumper);
	class Wind
	{
	public:
		Wind();
		void init();
		double getValue();
		void tick();
	private:
		const double min = -2.0;
		const double max = 2.0;
		double value;
	};
}

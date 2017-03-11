#include "Random.hpp"

namespace Random
{
	std::mt19937 generator(std::random_device{}());
	int getInt(int min, int max)
	{
		static std::uniform_int_distribution<> dist{0, 1};
		dist.param(std::uniform_int_distribution<>::param_type(min, max));
		return dist(generator);
	}
	double getReal(double min, double max)
	{
		static std::uniform_real_distribution<> dist{0.0, 1.0};
		dist.param(std::uniform_real_distribution<>::param_type(min, max));
		return dist(generator);
	}
}

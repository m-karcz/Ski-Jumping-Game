#include <random>

namespace Random
{
	std::mt19937 generator(std::random_device{}());
	std::uniform_real_distribution<> real_dist;
	std::uniform_int_distribution<> int_dist;
	int get_int(int min, int max)
	{
		int_dist.param(std::uniform_int_distribution<>::param_type(min, max));
		return int_dist(generator);
	}
	double get_real(double min, double max)
	{
		real_dist.param(std::uniform_real_distribution<>::param_type(min, max));
		return real_dist(generator);
	}
}

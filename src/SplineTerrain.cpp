#include "SplineTerrain.hpp"
#include <cmath>

using MyAlgLib::get_x;
using MyAlgLib::get_y;

double splineValue(double a, double b, const MyAlgLib::Vector2d<>& p0, double x)
{
	return a * (x - get_x(p0)) * (x - get_x(p0)) * (x - get_x(p0)) + b * (x - get_x(p0)) + get_y(p0);
}

double splineDerivative(double a, double b, const MyAlgLib::Vector2d<>& p0, double x)
{
	return 3.0 * a * (x - get_x(p0)) * (x - get_x(p0)) + b;
}

double SplineTerrain::getHeight(double x) const
{
	double xx = x/get_x(scale) - get_x(offset);
	if(xx < get_x(p1) || xx > get_x(p3))
		return -1.0;
	if(xx < x2)
		return get_y(offset) + get_y(scale)*splineValue(a1, b1, p1, xx);
	else
		return get_y(offset) + get_y(scale)*splineValue(a2, b2, p3, xx);
}
double SplineTerrain::getSlope(double x) const
{
	double xx = x/get_x(scale) - get_x(offset);
	if(xx < get_x(p1) || xx > get_x(p3))
		return -1.0;
	if(xx < x2)
		return get_y(scale) / get_x(scale) * splineDerivative(a1, b1, p1, xx);
	else
		return get_y(scale) / get_x(scale) * splineDerivative(a2, b2, p3, xx);

}
double SplineTerrain::getDistance(double x) const
{
	double distance=0;
	for(double i = 0.0; i <= x; i++)
		distance += std::sqrt(1.0 + std::pow(getSlope(i), 2.0)); //line integral
	return distance;
}

bool SplineTerrain::contains(double x) const
{
	double xx = x - get_x(offset);
	return (xx >= 0 && x <= (get_x(p3) - get_x(p1)) * get_x(scale));
}

#pragma once
#include "../../Simple-Algebra-Library/vectors2d.hpp"
#include <vector>
#include "Drawable.hpp"


class SplineTerrain : public Drawable
{
public: 
	SplineTerrain(const MyAlgLib::Vector2d<int>& offset, const MyAlgLib::Vector2d<>& p1, const double x2, const MyAlgLib::Vector2d<>& p3, const double a1, const double b1, const double a2, const double b2, const MyAlgLib::Vector2d<>& scale) : offset(offset), p1(p1), x2(x2), p3(p3), a1(a1), b1(b1), a2(a2), b2(b2), scale(scale) {}
	double getHeight(double x) const;
	double getSlope(double x) const;
	double getDistance(double x) const;
	bool contains(double x) const;
	virtual ~SplineTerrain() = default;
protected:
	const MyAlgLib::Vector2d<int> offset;
	const MyAlgLib::Vector2d<> p1;
	const double x2;
	const MyAlgLib::Vector2d<> p3;
	const double a1;
	const double b1;
	const double a2;
	const double b2;
	const MyAlgLib::Vector2d<> scale;
};

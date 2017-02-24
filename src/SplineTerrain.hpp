#pragma once
#include "../../Simple-Algebra-Library/vectors2d.hpp"
#include <array>
#include <vector>
#include <SDL2/SDL.h>
#include <algorithm>
#include <numeric>

namespace SkiJump
{
	class SplineTerrain
	{
	public:
		SplineTerrain(const MyAlgLib::Vector2d<int> offset, const MyAlgLib::Vector2d<> p1, const double x2, const MyAlgLib::Vector2d<> p3, const double a1, const double b1, const double a2, const double b2, const MyAlgLib::Vector2d<> scale, const std::string& filename, SDL_Renderer* renderer, const int r=255, const int g=255, const int b=255) : x_location(MyAlgLib::get_x(offset)), y_location(MyAlgLib::get_y(offset)), r(r), g(g), b(b)
		{
			std::size_t amount=static_cast<std::size_t>((MyAlgLib::get_x(p3)-MyAlgLib::get_x(p1))*MyAlgLib::get_x(scale));
			height.reserve(amount);
			slope.reserve(amount);
			const double step=1.0/MyAlgLib::get_x(scale);
			for(double x=MyAlgLib::get_x(p1); x < x2; x+=step)
			{
				height.push_back(MyAlgLib::get_y(scale)*get_value_of_spline(p1, a1, b1, x));
				slope.push_back(MyAlgLib::get_y(scale)*get_slope_of_spline(p1, a1, b1, x)/MyAlgLib::get_x(scale));
			}
			for(double x=x2; x < MyAlgLib::get_x(p3); x+=step)
			{
				height.push_back(MyAlgLib::get_y(scale)*get_value_of_spline(p3, a2, b2, x));
				slope.push_back(MyAlgLib::get_y(scale)*get_slope_of_spline(p3, a2, b2, x)/MyAlgLib::get_x(scale));
			}
			SDL_Surface* temp_surf=SDL_LoadBMP(filename.c_str());
			if(temp_surf!=nullptr)
			{
				SDL_SetColorKey(temp_surf, SDL_TRUE, SDL_MapRGB(temp_surf->format, 100, 100, 150));
				texture=SDL_CreateTextureFromSurface(renderer, temp_surf);
				SDL_FreeSurface(temp_surf);
			}
		}
		double get_height(int n) const
		{
			return height[n-x_location]+y_location;
		}
		double get_slope(int n) const
		{
			return slope[n-x_location];
		}
		double get_distance(int n)
		{
			return std::accumulate(slope.begin(), slope.begin() + n - x_location + 1, 0.0, [](auto sum, auto dx){ return sum+std::sqrt(1.0+dx*dx); });
		}
		bool does_exist(int n) const
		{
			return !(n < x_location || n >= x_location + height.size());
		}
		virtual void draw_range(SDL_Renderer* renderer, MyAlgLib::Vector2d<>& offset, int from, int to) const {}
		virtual ~SplineTerrain()
		{
			if(texture!=nullptr)
				SDL_DestroyTexture(texture);
		}
	protected:
		const int x_location;
		const int y_location;
		const int r;
		const int g;
		const int b;
		std::vector<double> height;
		std::vector<double> slope;
		MyAlgLib::Vector2d<double> p1;
		double x2;
		MyAlgLib::Vector2d<double> p3;
		double get_value_of_spline(const MyAlgLib::Vector2d<>& p, const double a, const double b, const double x)
		{
			using namespace MyAlgLib;
			return get_y(p)+a*(x-get_x(p))*(x-get_x(p))*(x-get_x(p))+b*(x-get_x(p));
		}
		double get_slope_of_spline(const MyAlgLib::Vector2d<>& p, const double a, const double b, const double x)
		{
			using namespace MyAlgLib;
			return 3.0*a*(x-get_x(p))*(x-get_x(p))+b;
		}
		SDL_Texture* texture=nullptr;


	};
}

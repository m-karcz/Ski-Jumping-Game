#include "Terrain.hpp"
#include <algorithm>

namespace SkiJump
{
	Terrain::Terrain(SDL_Renderer* renderer)
	{
		hill=std::make_unique<ObjectTerrain>(MyAlgLib::Vector2d_factory(0, 0), MyAlgLib::Vector2d_factory(0.0, 10.0), 2.0, MyAlgLib::Vector2d_factory(5.0, 1.0), 17.0/120.0, -61.0/15.0, -17.0/180.0, 11.0/60.0, MyAlgLib::Vector2d_factory(40.0, 20.0), "img/hill2.bmp", renderer, 255, 255, 255);
		landing=std::make_unique<PatternTerrain>(MyAlgLib::Vector2d_factory(200, -10), MyAlgLib::Vector2d_factory(0.0, 0.0), 5.0, MyAlgLib::Vector2d_factory(20.0, -10.0), 1.0/300.0, -13.0/12.0, -1.0/900.0, -1.0/12.0, MyAlgLib::Vector2d_factory(80.0, 30.0), "img/slope.bmp", renderer, 255, 255, 255);
		SDL_Surface* temp_surf=SDL_LoadBMP("img/shadow.bmp");
		SDL_SetColorKey(temp_surf, SDL_TRUE, SDL_MapRGB(temp_surf->format, 100, 100, 150));
		shadow=SDL_CreateTextureFromSurface(renderer, temp_surf);
		SDL_SetTextureAlphaMod(shadow, 100);
		SDL_FreeSurface(temp_surf);
	}

	void Terrain::draw(SDL_Renderer* renderer, MyAlgLib::Vector2d<>& offset)
	{
		landing->draw_range(renderer, offset, 0, 360);
		hill->draw_range(renderer, offset, 0, 360);
	}

	void Terrain::draw_shadow(SDL_Renderer* renderer, std::shared_ptr<Jumper> jumper)
	{
		double start = MyAlgLib::get_x(jumper->body->joints[0]) - std::cos(MyAlgLib::deg2rad(jumper->body->angles[0]))*5.0;
		double end = MyAlgLib::get_x(jumper->body->joints[0]) + std::cos(MyAlgLib::deg2rad(jumper->body->angles[0]))*15.0;
		double y_diff = MyAlgLib::get_y(jumper->location) - height(MyAlgLib::get_x(jumper->location));
		for(auto& joint : jumper->body->joints)
		{
			start = std::min(start, MyAlgLib::get_x(joint));
			end = std::max(end, MyAlgLib::get_x(joint));
		}
		double rad = -angle(MyAlgLib::get_x(jumper->location));
		double x_center = MyAlgLib::get_x(jumper->camera) + (start + end)/2;
		double y_center = MyAlgLib::get_y(jumper->offset) - height(MyAlgLib::get_x(jumper->location) + x_center - MyAlgLib::get_x(jumper->camera));
		double length = (end - start)/std::cos(rad) / std::cbrt(y_diff / 10 + 1);
		SDL_Rect shape;
		shape.w = static_cast<int>(length);
		shape.h = static_cast<int>(length/4.0);
		shape.x = MyAlgLib::get_x(jumper->camera) + start;
		shape.y = y_center-2;
		SDL_Point point;
		point.x = shape.w / 2;
		point.y = shape.h / 2;
		if(static_cast<int>(jumper->body->phase) >= static_cast<int>(Jumper::Body::Phase::Jump2))
			SDL_RenderCopyEx(renderer, shadow, nullptr, &shape, MyAlgLib::rad2deg(rad), &point, SDL_FLIP_NONE);
	}


	double Terrain::height(unsigned int i)
	{
		if(i < 0)
			return 0.0;
		if(i < 200)
			return hill->get_height(i);
		if(i < 1800)
			return landing->get_height(i);
		return landing->get_height(1799);
	}

	double Terrain::get_distance(int x)
	{
		return landing->get_distance(std::forward<int>(x))/5.0;
	}

	double Terrain::slope(unsigned int i)
	{
		if(i < 0)
			return 0.0;
		if(i < 200)
			return hill->get_slope(i);
		if(i < 1800)
			return landing->get_slope(i);
		return landing->get_slope(1799);
	}

	double Terrain::angle(unsigned int i)
	{
		return std::atan(slope(i));
	}

	double Terrain::height(double x)
	{
		return height(static_cast<unsigned int>(x));
	}

	double Terrain::slope(double x)
	{
		return slope(static_cast<unsigned int>(x));
	}

	double Terrain::angle(double x)
	{
		return angle(static_cast<unsigned int>(x));
	}
}

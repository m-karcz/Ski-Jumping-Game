#pragma once
#include "../../Simple-Algebra-Library/vectors2d.hpp"
#include "Jumper.hpp"
#include "Terrain.hpp"
#include "Randomness.hpp"
#include "Numbers.hpp"
#include <memory>

namespace SkiJump
{
	class Physics
	{
	public:
		Physics(SDL_Renderer* renderer, std::shared_ptr<Jumper> jumper, std::shared_ptr<Terrain> terrain, const double dt);
		const double dt=0.1;
		void update_jumper();
		std::shared_ptr<Terrain> terrain;
		std::shared_ptr<Jumper> jumper;
		void draw_wind(SDL_Renderer* renderer)
		{
			wind->draw(renderer);
		}
	private:
		MyAlgLib::Vector2d<double> gravity = MyAlgLib::Vector2d_factory(0.0, -1.0);
		struct Wind;
		void update_speeding();
		void update_flight();
		void update_landing();
		void update_trying_to_land();
		void update_jumping();
		unsigned int get_jumper_x();
		void update_location();
		void slide_on_terrain();
		bool check_if_touched_terrain();
		void proceed_landing();
		MyAlgLib::Vector2d<double> ramp_force(double angle);
		MyAlgLib::Vector2d<double> air_resistance_force();
		std::unique_ptr<Wind> wind;
		struct Wind
		{
			Wind(SDL_Renderer* renderer)
			{
				init();
				SDL_Surface* temp_surf=SDL_LoadBMP("img/arrow.bmp");
				SDL_SetColorKey(temp_surf, SDL_TRUE, SDL_MapRGB(temp_surf->format, 100, 100, 150));
				arrow_texture=std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, temp_surf), SDL_DestroyTexture);
				SDL_FreeSurface(temp_surf);
			}
			void init()
			{
				value=Random::get_real(-1.0, 1.0);
			}
			void update()
			{
				value+=Random::get_real(-0.04, 0.04);
				if(value > 1.0)
					value = 1.0;
				if(value < -1.0)
					value = -1.0;
			}
			MyAlgLib::Vector2d<> get_force(double deg)
			{
				const double coeff = 0.2;
				double rad=MyAlgLib::deg2rad(deg);
				double length=std::sin(rad)*value;
				return coeff * length * MyAlgLib::Vector2d_factory(std::sin(rad), std::cos(rad));
			}
			void draw(SDL_Renderer* renderer)
			{
				SDL_Rect location;
				location.x=290;
				location.y=10;
				SDL_QueryTexture(arrow_texture.get(), nullptr, nullptr, &location.h, &location.w);
				SDL_Point point;
				point.y=location.h/2;
				point.x=0;
				location.w=static_cast<int>(static_cast<double>(location.w)*std::abs(value));
				Numbers::draw_number(renderer, value*3.0, 275, 30);
				SDL_RenderCopyEx(renderer, arrow_texture.get(), nullptr, &location, value > 0 ? 180.0 : 0.0, &point, SDL_FLIP_NONE);
			}
			std::shared_ptr<SDL_Texture> arrow_texture;
			double value;
		};
	};
}

#include "Physics.hpp"
#include <cmath>

namespace SkiJump
{

	Physics::Physics(SDL_Renderer* renderer, std::shared_ptr<Jumper> jumper, std::shared_ptr<Terrain> terrain, const double dt) : jumper(jumper), terrain(terrain), dt(dt)
	{
		wind=std::make_unique<Wind>(renderer);
	}

	void Physics::update_jumper()
	{
		using namespace MyAlgLib;
		switch(jumper->state)
		{
			case JumperState::Wait:
				MyAlgLib::get_y(jumper->location) = terrain->height(get_jumper_x());
				jumper->body->angles[0] = MyAlgLib::rad2deg(terrain->angle(get_x(jumper->location)));
				break;
			case JumperState::SpeedingUp:
				update_speeding();
				if(get_x(jumper->location) > 195.0)
				{
					jumper->jump(201.0);
				}
				break;
			case JumperState::Jumping:
				update_jumping();
				break;
			case JumperState::TryingToLand:
				update_trying_to_land();
				break;
			case JumperState::Flight:
				update_flight();
				break;
			case JumperState::Landing:
				update_landing();
				break;
			default:
				break;
		}
		jumper->update_offset();
		jumper->update();
		wind->update();
	}

	void Physics::update_speeding()
	{
		using namespace MyAlgLib;
		jumper->velocity = jumper->velocity + dt* 0.8 * ramp_force(terrain->angle(get_x(jumper->location)));
		slide_on_terrain();
		jumper->body->angles[0] = MyAlgLib::rad2deg(terrain->angle(get_x(jumper->location)));
		return;
	}

	void Physics::update_jumping()
	{
		using namespace MyAlgLib;
		jumper->velocity = jumper->velocity + dt * (gravity + Vector2d_factory(0.0, 2.0));
		jumper->location = jumper->location + dt * jumper->velocity;
		jumper->timer--;
		if(jumper->timer==0)
			jumper->state = JumperState::Flight;
		return;
	}

	void Physics::update_flight()
	{
		using namespace MyAlgLib;
		jumper->velocity = jumper->velocity + dt  * (gravity + wind->get_force(jumper->body->angles[1]) + air_resistance_force());
		jumper->location = jumper->location + dt * jumper->velocity;
		if(get_x(jumper->location) > 210.0 && check_if_touched_terrain())
		{
			proceed_landing();
		}
		return;
	}

	void Physics::update_trying_to_land()
	{
		using namespace MyAlgLib;
		jumper->velocity = jumper->velocity + dt * gravity;
		jumper->location = jumper->location + dt * jumper->velocity;
		double landing_angle = terrain->angle(get_x(jumper->location));
		if(jumper->timer>0)
		{
			jumper->body->angles[0]+=((landing_angle-jumper->body->angles[0])/static_cast<double>(jumper->timer));
			jumper->timer--;
		}
		else
		{
			jumper->body->angles[0]=landing_angle;
		}
		if(check_if_touched_terrain())
			proceed_landing();
		return;
	}

	void Physics::update_landing()
	{
		using namespace MyAlgLib;
		if(get_x(jumper->velocity) > 0)
		{
			jumper->velocity = jumper->velocity + dt * Vector2d_factory(-1.0, 0.0);
			slide_on_terrain();
		}
		jumper->body->angles[0] = MyAlgLib::rad2deg(terrain->angle(get_x(jumper->location)));
		return;
	}

	MyAlgLib::Vector2d<double> Physics::ramp_force(double rad)
	{
		using namespace MyAlgLib;
		Vector2d<double> vector;
		double gravity_length=length(gravity)*2;
		get_x(vector)=gravity_length * std::sin(rad) * std::sin(rad);
		get_y(vector)=gravity_length * std::sin(rad) * std::cos(rad);
		return vector;
	}

	unsigned int Physics::get_jumper_x()
	{
		return static_cast<unsigned int>(MyAlgLib::get_x(jumper->location));
	}

	void Physics::update_location()
	{
		jumper->location += dt * jumper->velocity;	
	}

	void Physics::slide_on_terrain()
	{
		using namespace MyAlgLib;
		jumper->velocity = length(jumper->velocity) * rotation_matrix_rad(terrain->angle(get_x(jumper->location))) * Vector2d_factory(1.0, 0.0);
		update_location();
		MyAlgLib::get_y(jumper->location) = terrain->height(get_jumper_x());
	}

	bool Physics::check_if_touched_terrain()
	{
		return get_y(jumper->location) <= terrain->height(get_x(jumper->location));
	}

	void Physics::proceed_landing()
	{
		jumper->distance = terrain->get_distance(static_cast<int>(get_x(jumper->location)));
		jumper->state = JumperState::Landing;
		jumper->landing(jumper->body->phase == Jumper::Body::Phase::WaitForLand);
	}

	MyAlgLib::Vector2d<double> Physics::air_resistance_force()
	{
		const double coeff = 0.0002;
		using namespace MyAlgLib;
		double len = length(jumper->velocity);
		double body_angle = deg2rad(jumper->body->angles[0]+jumper->body->angles[1]);
		double force_angle = arg(jumper->velocity);
		return - coeff * len * len * std::abs(std::sin(body_angle - force_angle)) * rotation_matrix_rad(force_angle) * Vector2d_factory(1.0, 0.0);
	}
}

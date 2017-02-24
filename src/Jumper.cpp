#include "Jumper.hpp"
#include <string>

namespace SkiJump
{

	Jumper::Jumper(SDL_Renderer* renderer)
	{
		SDL_Surface* surf=SDL_LoadBMP("img/ski.bmp");
		body=std::make_unique<Body>(renderer);
		SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format, 100, 100, 150));
		ski_texture = SDL_CreateTextureFromSurface(renderer, surf);
		SDL_FreeSurface(surf);
	}

	void Jumper::draw(SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_Point point;
		point.x=5;
		point.y=5;
		int x_off=static_cast<int>(std::cos(body->angles[0])*ski_length);
		int y_off=static_cast<int>(std::sin(body->angles[0])*ski_length);
		SDL_Rect rect;
		rect.x=MyAlgLib::get_x(camera)-point.x;
		rect.y=MyAlgLib::get_y(camera)-point.y;
		rect.w=20;
		rect.h=5;
		SDL_RenderCopyEx(renderer, ski_texture, nullptr, &rect, -body->angles[0], &point, SDL_FLIP_NONE);
		body->draw(renderer, camera);
		if(distance > 0.1)
			Numbers::draw_number(renderer, distance, 160, 210);
		if(std::abs(points) > 0.01)
			Numbers::draw_number(renderer, points, 240, 210);
	}

	void Jumper::update()
	{
		body->calculate_joints();
		body->move_body();
	}

	void Jumper::click()
	{
		if(state == JumperState::SpeedingUp && MyAlgLib::get_x(location) > 150.0)
		{
			jump(MyAlgLib::get_x(location));
		}
		if(state == JumperState::Flight)
		{
			state = JumperState::TryingToLand;
			body->set_phase(Body::Phase::PrepareToLand);
			timer=10;
		}
		if(state == JumperState::Wait)
		{
			state = JumperState::SpeedingUp;
			body->set_phase(Body::Phase::Push);
		}
	}

	void Jumper::jump(double location)
	{
		if(location < 200.0)
		{
			state = JumperState::Jumping;
			timer=static_cast<int>(std::sqrt(location/3.0-50.0))+10;
		}
		else
		{
			state= JumperState::Flight;
		}
		move_camera_timer=50;
		body->set_phase(Body::Phase::Jump1);
	}

	void Jumper::balance(int32_t movement)
	{
		if((body->phase == Body::Phase::Fly || body->phase == Body::Phase::Jump2) )
		{
			double value=static_cast<double>(movement);
			if(value > 0.0)
			{
				if(body->angles[1] > 10.0)
				{
					body->angles[1]-=value*0.5;
					moves+=std::abs(movement);
				}
			}
			if(value < 0.0)
			{
				if(body->angles[1] < 60.0)
				{
					body->angles[1]-=value*0.5;
					moves+=std::abs(movement);
				}
			}
		}
	}

	void Jumper::update_offset()
	{
		if(move_camera_timer>0)
		{
			MyAlgLib::get_y(camera)++;
			move_camera_timer--;
		}
		MyAlgLib::SquareMatrix<int, 2> coeff;
		coeff(0,0)=-1;
		coeff(1,1)=1;
		offset = coeff * location + camera;
	}

	Jumper::~Jumper()
	{
		SDL_DestroyTexture(ski_texture);
	}


	Jumper::Body::Body(SDL_Renderer* renderer)
	{
		joints[0]=MyAlgLib::Vector2d_factory(0.0, 0.0);
		std::string path="img/body";
		for(int i=0; i < 4; i++)
		{
			SDL_Surface* temp_surf=SDL_LoadBMP((path+std::to_string(i)+std::string(".bmp")).c_str());
			if(temp_surf!=nullptr)
			{
				SDL_SetColorKey(temp_surf, SDL_TRUE, SDL_MapRGB(temp_surf->format, 100, 100, 150));
				textures[i]=SDL_CreateTextureFromSurface(renderer, temp_surf);
				SDL_FreeSurface(temp_surf);
			}
		}
		std::array<double, 5> nothing{1000.0, 1000.0, 1000.0, 1000.0, 1000.0};
		angles_lookup_table[static_cast<int>(Phase::Wait)]={1000.0, 90.0, 90.0, -90.0, 170.0};
		angles_lookup_table[static_cast<int>(Phase::Push)]={1000.0, 90.0, 90.0, -180.0, 150.0};
		angles_lookup_table[static_cast<int>(Phase::Slide)]=nothing;
		angles_lookup_table[static_cast<int>(Phase::Jump1)]={1000.0, 90.0, 0.0, -90.0, 90.0};
		angles_lookup_table[static_cast<int>(Phase::Jump2)]={1000.0, 1000.0, 0.0, 0.0, 150.0};
		angles_lookup_table[static_cast<int>(Phase::Fly)]=nothing;
		angles_lookup_table[static_cast<int>(Phase::PrepareToLand)]={1000.0, 90.0, 90.0, -135.0, 150.0};
		angles_lookup_table[static_cast<int>(Phase::WaitForLand)]=nothing;
		angles_lookup_table[static_cast<int>(Phase::DuckAfterLand)]={1000.0, 45.0, 90.0, -45.0, 1000.0};
		angles_lookup_table[static_cast<int>(Phase::WaitAfterDuck)]=nothing;
		angles_lookup_table[static_cast<int>(Phase::StandUpAfterWaiting)]={1000.0, 90.0, 30.0, -30.0, 179.0};
		angles_lookup_table[static_cast<int>(Phase::EndOfJump)]={1000.0, 90.0, 0.0, 0.0, 179.0};
		angles_lookup_table[static_cast<int>(Phase::Fall1)]={1000.0, 0.0, 90.0, -30.0, 210.0};
		angles_lookup_table[static_cast<int>(Phase::Fall2)]={1000.0, 0.0, 0.0, 0.0, 360.0};

	}

	Jumper::Body::~Body()
	{
		for(auto& texture : textures)
			if(texture!=nullptr)
				SDL_DestroyTexture(texture);
	}

	void Jumper::Body::calculate_joints()
	{
		double angle_sum=0;
		for(std::size_t i=1; i < 5; i++)
		{
			angle_sum+=MyAlgLib::deg2rad(angles[i]);
			joints[i]=lengths[i-1]*MyAlgLib::Vector2d_factory(std::cos(angle_sum), std::sin(angle_sum)) + joints[i-1];
		}
		for(auto& joint : joints)
			joint = MyAlgLib::rotation_matrix_rad(MyAlgLib::deg2rad(angles[0])) * joint;
	}

	void Jumper::Body::draw(SDL_Renderer* renderer, const MyAlgLib::Vector2d<double>& offset)
	{
		calculate_joints();
		double angle_sum=angles[0];
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		for(int i=1; i < 5; i++)
		{
			SDL_Rect source;
			SDL_Rect location;
			SDL_Point point;
			SDL_QueryTexture(textures[i-1], nullptr, nullptr, &source.w, &source.h);
			location.w = source.w;
			location.h = source.h;
			angle_sum+=angles[i];
			point.x=1;
			point.y=(source.h-1)/2;
			location.x = (MyAlgLib::get_x(joints[i-1]) - point.x)+MyAlgLib::get_x(offset);
			location.y = -(MyAlgLib::get_y(joints[i-1]) + point.y)+MyAlgLib::get_y(offset);
			source.x=0;
			source.y=0;
			SDL_RenderCopyEx(renderer, textures[i-1], &source, &location, -angle_sum, &point, SDL_FLIP_NONE);
		}
	}

	void Jumper::Body::move_body()
	{
		auto default_action=[&](){
			if(body_timer!=0)
			{
				set_figure();
				if(--body_timer==0)
					set_phase(static_cast<Phase>(static_cast<int>(phase)+1));
			}
		};
		switch(phase)
		{
			case Phase::Push:
			case Phase::Jump1:
			case Phase::PrepareToLand:
			case Phase::DuckAfterLand:
			case Phase::WaitAfterDuck:
			case Phase::StandUpAfterWaiting:
			case Phase::WaitForLand:
			case Phase::Fall1:
				default_action();
				break;
			case Phase::Jump2:
				increment_angle(1, -3.0, 40.0, 90.0);
				default_action();
				break;
			case Phase::Fall2:
				if(body_timer!=0)
				{
					set_figure();
					if(--body_timer==0)
						set_phase(Phase::EndOfJump);
				}
				break;
			case Phase::Slide:
			case Phase::EndOfJump:
			default:
				break;

		}
	}

	void Jumper::Body::set_phase(Phase new_phase)
	{
		switch(new_phase)
		{
			case Phase::Jump2:
			case Phase::Fall1:
				body_timer=5;
				break;
			case Phase::Jump1:
			case Phase::PrepareToLand:
			case Phase::DuckAfterLand:
			case Phase::StandUpAfterWaiting:
			case Phase::Fall2:
				body_timer=10;
				break;
			case Phase::WaitAfterDuck:
			case Phase::Push:
				body_timer=30;
				break;
			default:
				break;
		}
		phase=new_phase;
	}

	void Jumper::landing(bool success)
	{
		fall = !success;
		if(success)
		{
			body->set_phase(Jumper::Body::Phase::DuckAfterLand);
		}
		else
		{
			body->set_phase(Jumper::Body::Phase::Fall1);
		}
	}

	void Jumper::Body::set_figure()
	{
		for(int i=1; i < 5; i++)
			if(angles_lookup_table[static_cast<int>(phase)][i] < 500.0)
				angles[i]+=(angles_lookup_table[static_cast<int>(phase)][i]-angles[i])/body_timer;
	}

	void Jumper::Body::increment_angle(std::size_t index, double step, double min, double max)
	{
		if(angles[index]+step > min && angles[index]+step < max)
		{
			angles[index]+=step;
		}
	}

	bool Jumper::Body::balance_angle(std::size_t source, std::size_t destination, double max_step, double offset)
	{
		if(std::abs(offset+(source==0 ? -1 : 1)*angles[source]-angles[destination]) < std::abs(max_step))
		{
			angles[destination]=(source==0 ? -1 : 1)*angles[source]+offset;
			return true;
		}
		else
		{
			angles[destination]+=max_step;
			return false;
		}
	}
}

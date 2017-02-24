#pragma once
#include "../../Simple-Algebra-Library/vectors2d.hpp"
#include "JumperState.hpp"
#include <SDL2/SDL.h>
#include <array>
#include <memory>
#include "Numbers.hpp"


namespace SkiJump
{
	class Jumper
	{
	public:
		struct Body;
		Jumper(SDL_Renderer* renderer);
		~Jumper();
		MyAlgLib::Vector2d<double> location = MyAlgLib::Vector2d_factory(0.0, 0.0);
		MyAlgLib::Vector2d<double> velocity = MyAlgLib::Vector2d_factory(0.0, 0.0);
		MyAlgLib::Vector2d<double> offset = MyAlgLib::Vector2d_factory(0.0, 0.0);
		MyAlgLib::Vector2d<double> camera = MyAlgLib::Vector2d_factory(50.0, 50.0);
		void update_offset();
		JumperState state = JumperState::Wait;
		std::unique_ptr<Body> body;
		const int ski_length=10;
		int move_camera_timer=0;
		int jump_timer=0;
		void balance(int32_t movement);
		void draw(SDL_Renderer* renderer);
		void draw_body(SDL_Renderer* renderer);
		void click();
		void jump(double location);
		void landing(bool success);
		int timer=0;
		SDL_Texture* ski_texture=nullptr;
		void update();
		double distance=0.0;
		double points=0.0;
		bool fall=false;
		int moves=0;
		struct Body
		{
			enum class Phase;
			Body(SDL_Renderer* renderer);
			~Body();
			std::array<int, 4> lengths{{5, 5, 8, 5}};
			std::array<double, 5> angles{{0.0, 90.0, 90.0, -90.0, 170.0}};
			std::array<MyAlgLib::Vector2d<>, 5> joints;
			void calculate_joints();
			int body_timer=0;
			std::array<SDL_Texture*, 4> textures;
			void draw(SDL_Renderer* renderer, const MyAlgLib::Vector2d<double>& offset);
			Phase phase=Phase::Wait;
			void move_body();
			void set_phase(Phase new_phase);
			void set_figure();
			void increment_angle(std::size_t index, double step, double min, double max);
			bool balance_angle(std::size_t source, std::size_t destination, double max_step, double offset);
			std::array<std::array<double, 5>, 14> angles_lookup_table;
			enum class Phase
			{
				Wait=0,
				Push=1,
				Slide=2,
				Jump1=3,
				Jump2=4,
				Fly=5,
				PrepareToLand=6,
				WaitForLand=7,
				DuckAfterLand=8,
				WaitAfterDuck=9,
				StandUpAfterWaiting=10,
				EndOfJump=11,
				Fall1=12,
				Fall2=13
			};
		};
	};
}

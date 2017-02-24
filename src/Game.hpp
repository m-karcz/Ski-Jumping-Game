#pragma once
#include <SDL2/SDL.h>
#include "Jumper.hpp"
#include "Physics.hpp"
#include <memory>
#include "Terrain.hpp"
#include "Judges.hpp"

namespace SkiJump
{
	class Game
	{
	public:
		Game();
		Game(const Game& game)=delete;
		Game(Game&& game)=delete;
		~Game();
		void run();
	private:
		std::shared_ptr<Terrain> terrain;
		SDL_Window* window=nullptr;
		SDL_Renderer* renderer=nullptr;
		//SDL_Texture* overlay=nullptr;
		uint32_t static draw_timer_callback(uint32_t interval, void* params); 
		const uint32_t delay=30;
		const double dt=0.2;
		SDL_TimerID game_loop_timer;
		SDL_Event event;
		bool quit=false;
		std::shared_ptr<Jumper> jumper;
		std::unique_ptr<Physics> physics;
		std::unique_ptr<Judges> judges;

	};
}

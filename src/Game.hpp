#pragma once
#include <SDL2/SDL.h>
#include "BetterSDL/BetterSDL.hpp"
#include <memory>

class Game
{
	Game()
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		window = std::make_unique<BetterSDL::Window>("Ski Jumping Game", 100, 100, 640, 480);
		renderer = std::make_unique<BetterSDL::Renderer>(*window);
		terrain = std::make_shared<Terrain>();
		jumper = std::make_unique<Jumper>(terrain);
	}
	~Game()
	{
		SDL_Quit();
	}
	std::unique_ptr<BetterSDL::Renderer> renderer;
	std::unique_ptr<BetterSDL::Window> window;
	std::shared_ptr<Terrain> terrain;
	std::unique_ptr<Jumper> jumper;
};

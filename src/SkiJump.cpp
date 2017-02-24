#include <SDL2/SDL.h>
#include <iostream>
#include <array>
#include <vector>
#include "SkiJump.hpp"
#include "Game.hpp"


int main(int argc, char** argv)
{
	//SDL_UpdateWindowSurface(window);
	SkiJump::Game game;
	game.run();
	return 0;
}

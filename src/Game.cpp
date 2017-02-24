#include "Game.hpp"
#include "Numbers.hpp"

namespace SkiJump
{
	Game::Game()// : physics(jumper, hill, slope, dt)
	{
		//SDL_Init(SDL_INIT_VIDEO || SDL_INIT_TIMER);
		SDL_Init(SDL_INIT_EVERYTHING);
		window=SDL_CreateWindow("xDD", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
		SDL_ShowCursor(SDL_DISABLE);
		renderer=SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		SDL_RenderSetScale(renderer, 2.0, 2.0);
		jumper=std::make_shared<Jumper>(renderer);
		terrain=std::make_shared<Terrain>(renderer);
		physics=std::make_unique<Physics>(renderer, jumper, terrain, dt);
		judges=std::make_unique<Judges>(jumper);
/*		SDL_Surface* temp_surf=SDL_LoadBMP("img/overlay.bmp");
		SDL_SetColorKey(temp_surf, SDL_TRUE, SDL_MapRGB(temp_surf->format, 100, 100, 150));
		overlay=SDL_CreateTextureFromSurface(renderer, temp_surf);
		SDL_FreeSurface(temp_surf);*/
	}

	void Game::run()
	{
		game_loop_timer = SDL_AddTimer(delay, draw_timer_callback, nullptr);
		while(!quit)
		{
			while(SDL_PollEvent(&event))
			{
				switch(event.type)
				{
					case SDL_QUIT:
						quit=true;
						break;
					case SDL_USEREVENT:
						physics->update_jumper();
						judges->update();
						break;
					case SDL_MOUSEBUTTONDOWN:
						jumper->click();
						break;
					case SDL_MOUSEMOTION:
						jumper->balance(event.motion.yrel);	
						break;
					case SDL_KEYDOWN:
						{
							const uint8_t* state = SDL_GetKeyboardState(nullptr);
							if(state[SDL_SCANCODE_ESCAPE])
								quit=true;
						}
						break;
					default:
						break;
				}
				SDL_SetRenderDrawColor(renderer, 100, 100, 150, 255);
				SDL_RenderClear(renderer);
				terrain->draw(renderer, jumper->offset);
				judges->draw(renderer);
				terrain->draw_shadow(renderer, jumper);
				//SDL_RenderCopy(renderer, overlay, nullptr, nullptr);
				physics->draw_wind(renderer);
				jumper->draw(renderer);
				SDL_Rect rect;
				SDL_RenderPresent(renderer);
			}

		}
		
	}

	uint32_t Game::draw_timer_callback(uint32_t interval, void* params)
	{
		SDL_Event event;
		SDL_UserEvent userevent;
		userevent.type = SDL_USEREVENT;
		userevent.code = 0;
		userevent.data1 = NULL;
		userevent.data2 = NULL;
		event.type = SDL_USEREVENT;
		event.user = userevent;
		SDL_PushEvent(&event);
		return(interval);
	}

	Game::~Game()
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
}

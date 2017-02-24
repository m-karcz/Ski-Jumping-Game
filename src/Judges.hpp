#pragma once
#include "Jumper.hpp"
#include "Numbers.hpp"
#include "Randomness.hpp"
#include <array>

namespace SkiJump
{
	class Judges
	{
	public:
		Judges(std::shared_ptr<Jumper> jumper);
		void update();
		void draw(SDL_Renderer* renderer);
	private:
		int timestamp=-1;
		std::shared_ptr<Jumper> jumper;
		std::array<int, 5> timestamps{{-1, -1, -1, -1, -1}};
		std::array<int, 5> points{{-1, -1, -1, -1, -1}};
		const int x=270;
		const int y=90;
		const int max_timestamp=50;
		const double k=120.0;
		int base=0;
		int from_moving();
		int from_distance();
		int from_landing();
	};
}

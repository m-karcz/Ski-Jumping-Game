#include "Judges.hpp"
#include <iostream>
#include <algorithm>
#include <functional>

namespace SkiJump
{
	Judges::Judges(std::shared_ptr<Jumper> jumper) : jumper(jumper) {}

	void Judges::draw(SDL_Renderer* renderer)
	{
		for(int i=0; i < 5; i++)
		{
			if(points[i] != -1)
			{
				Numbers::draw_number(renderer, static_cast<double>(points[i])/2.0, x, y+i*15);
			}
		}
	}		

	void Judges::update()
	{
		using namespace std::placeholders;
		if(timestamp==-1 && jumper->distance > 0.0)
		{
			base = 3 + from_moving() + from_distance() + from_landing();
			timestamp=max_timestamp;
			for(int i=0; i < 5; i++)
			{
				timestamps[i]=Random::get_int(1, max_timestamp);
			}
		}
		if(timestamp > 0)
		{
			for(int i=0; i < 5; i++)
			{
				if(timestamps[i]==timestamp)
				{
					points[i]=base+Random::get_int(-2, 2);
				}
			}
			if(std::none_of(points.begin(), points.end(), std::bind(std::equal_to<int>(), _1, -1)))
			{
				std::array<int, 5> points_copy = points;
				std::sort(points_copy.begin(), points_copy.end());
				jumper->points = std::accumulate(points_copy.begin()+1, points_copy.end()-1, 0.0, std::plus<double>())/2.0 + (jumper->distance-k)*1.8 + 60.0;
				timestamp=0;
				return;
			}
			timestamp--;
		}
	}

	int Judges::from_moving()
	{
		int points=15;
		if(jumper->moves > 100)
		{
			int diff = jumper->moves - 100;
			points -= diff > 100 ? 5 : diff/20;
		}
		return points;
	}
	int Judges::from_distance()
	{
		int points = 15;
		if(jumper->distance < k)
		{
			double diff = k - jumper->distance;
			points -= diff > 30.0 ? 5 : static_cast<int>(diff/6.0);
		}
		return points;	
	}
	int Judges::from_landing()
	{
		return jumper->fall ? 0 : 5;
	}
}

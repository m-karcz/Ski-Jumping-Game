#pragma once
#include "Drawable.hpp"
#include <type_traits>
#include <sstream>
#include <iomanip>

namespace Numbers
{
	void draw(BetterSDL::Renderer& renderer, double value, int x, int y, int precision=1);
}

#pragma once
#include <vector>
#include <array>
#include <memory>
#include "Drawable.hpp"
#include "../../Simple-Algebra-Library/vectors2d.hpp"
#include "Terrain.hpp"

class Jumper :  public ComplexDrawable
{	
public:
	Jumper(std::shared_ptr<Terrain> terrain);
	Jumper(const Jumper& body) = delete;
	MyAlgLib::Vector2d<> location;
	MyAlgLib::Vector2d<> velocity;
	void draw(BetterSDL::Renderer& renderer) override;
	std::array<double, 5> angles{{0.0, 90.0, 90.0, -90.0, 170.0}};
	const std::array<double, 5> lengths{{10, 5, 5, 8, 5}};
	const std::array<double, 5> jointLengths{{3, 5, 5, 8, 5}};
	std::array<MyAlgLib::Vector2d<>, 5> jointLocations;
	std::shared_ptr<Terrain> terrain;
	void reset();
	void calculateJoints();
	struct State
	{
		State(Jumper& jumper) : jumper(jumper) {}
		State(const State& state) = delete;
		Jumper& jumper;
		virtual void tick() = 0;
		virtual void mouseMovement(int32_t move) = 0;
		virtual void click() {}
	};
	std::unique_ptr<State> state;
	
};

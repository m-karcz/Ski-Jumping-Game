#include "Jumper.hpp"
#include "Physics.hpp"


using JmpState = Jumper::State;

struct StateWithFunctions : public Jumper::State
{
	using JmpState::JmpState;
	void moveAnkle(int32_t move)
	{
		double value = static_cast<double>(move) / 2.0;
		if(value > 0.0)
		{
			if(jumper.angles[1] > 10.0)
			{
				jumper.angles[1] -= value;
			}
		}
		else
		{
			if(jumper.angles[1] < 60.0)
			{
				jumper.angles[1] -= value;
			}
		}
	}
	void setSkiOnTerrain()
	{
		const double terrainAngle = MyAlgLib::rad2deg(std::atan(jumper.terrain->getSlope(MyAlgLib::get_x(jumper.location))));
		if(std::abs(jumper.angles[0] - terrainAngle) < 10.0)
			jumper.angles[0] = terrainAngle;
		else
			jumper.angles[0] += jumper.angles[0] < terrainAngle ? 10.0 : -10.0;

	}
	void setSki(const double angle)
	{
		setAngle<0>(std::forward<decltype(angle)>(angle));
	}
	void setAnkle(const double angle)
	{
		setAngle<1>(std::forward<decltype(angle)>(angle));
	}
	void setKnee(const double angle)
	{
		setAngle<2>(std::forward<decltype(angle)>(angle));
	}
	void setBottom(const double angle)
	{
		setAngle<3>(std::forward<decltype(angle)>(angle));
	}
	void setArm(const double angle)
	{
		setAngle<4>(std::forward<decltype(angle)>(angle));
	}
	bool touchedTerrain()
	{
		return jumper.terrain->getHeight(MyAlgLib::get_x(jumper.location)) > MyAlgLib::get_y(jumper.location);
	}
	template<typename NewState>
	void setState()
	{
		static_assert(std::is_base_of<State, NewState>::value, "It is not state!");
		jumper.state = std::make_unique<NewState>(jumper);
	}
	int ticksLeft;
private:
	template<std::size_t I>
	void setAngle(const double angle)
	{
		static_assert(I >= 0 && I <= 4, "wrong index!");
		jumper.angles[I] += (angle - jumper.angles[I]) / static_cast<double>(ticksLeft);
	}
};

template<bool Moveable, int TicksLeft = -1>
struct ImprovedState : public StateWithFunctions
{
	using StateWithFunctions::StateWithFunctions;
	void mouseMovement(int32_t move) override; 
	int ticksLeft = TicksLeft;
};

template<int TicksLeft>
struct ImprovedState<true, TicksLeft> : public StateWithFunctions
{
	using StateWithFunctions::StateWithFunctions;
	void mouseMovement(int32_t move) override
	{
		moveAnkle(std::forward<decltype(move)>(move));
	}
	int ticksLeft = TicksLeft;

};

template<int TicksLeft>
struct ImprovedState<false, TicksLeft> : public StateWithFunctions
{
	using StateWithFunctions::StateWithFunctions;
	void mouseMovement(int32_t move) override
	{}
	int ticksLeft = TicksLeft;
};

struct DoNothingAfterLand : public ImprovedState<false>
{
	using ImprovedState::ImprovedState;
	void tick() override
	{
		Physics::setForcesAfterLand(jumper, *(jumper.terrain));
		setSkiOnTerrain();
	}
};

struct Fail2Land2 : public ImprovedState<false, 5>
{
	using ImprovedState::ImprovedState;
	void tick() override
       	{
		Physics::setForcesAfterLand(jumper, *(jumper.terrain));
		setSkiOnTerrain();
		setAnkle(0.0);
		setKnee(0.0);
		setBottom(0.0);
		setArm(360.0);
		if(--ticksLeft == 0)
			setState<DoNothingAfterLand>();
	}
};

struct Fail2Land1 : public ImprovedState<false, 5>
{
	using ImprovedState::ImprovedState;
	void tick() override 
	{
		Physics::setForcesAfterLand(jumper, *(jumper.terrain));
		setSkiOnTerrain();
		setAnkle(0.0);
		setKnee(90.0);
		setBottom(-30.0);
		setArm(210.0);
		if(--ticksLeft == 0)
			setState<Fail2Land2>();
	}
};

struct StandAfterDuck : public ImprovedState<false, 5>
{
	using ImprovedState::ImprovedState;
	void tick() override
	{
		Physics::setForcesAfterLand(jumper, *(jumper.terrain));
		setSkiOnTerrain();
		setAnkle(90.0);
		setKnee(30.0);
		setBottom(-30.0);
		setArm(179.0);
		if(--ticksLeft == 0)
			setState<DoNothingAfterLand>();
	}
};

struct WaitAfterDuck : public ImprovedState<false, 5>
{
	using ImprovedState::ImprovedState;
	void tick() override
	{
		Physics::setForcesAfterLand(jumper, *(jumper.terrain));
		setSkiOnTerrain();
		if(--ticksLeft == 0)
			setState<StandAfterDuck>();
		
	}
};

struct DuckAfterLand : public ImprovedState<false, 5>
{
	using ImprovedState::ImprovedState;
	void tick() override
	{
		Physics::setForcesAfterLand(jumper, *(jumper.terrain));
		setSkiOnTerrain();
		setAnkle(45.0);
		setKnee(90.0);
		setBottom(-45.0);
		if(--ticksLeft == 0)
			setState<Fail2Land2>();
	}
};

struct WaitForLand : public ImprovedState<false>
{
	using ImprovedState::ImprovedState;
	void tick() override
	{
		Physics::setForcesInFlight(jumper);
		if(touchedTerrain())
			setState<DuckAfterLand>();
	}
};


struct Try2Land : public ImprovedState<false, 5>
{
	using ImprovedState::ImprovedState;
	void tick() override
	{
		Physics::setForcesInFlight(jumper);
		setSkiOnTerrain();
		setAnkle(90.0);
		setKnee(90.0);
		setBottom(-134.0);
		setArm(150.0);
		if(touchedTerrain())
			setState<Fail2Land1>();
	}
};



struct Fly : public ImprovedState<true>
{
	using ImprovedState::ImprovedState;
	void tick() override
	{
		Physics::setForcesInFlight(jumper);
		if(touchedTerrain())
			setState<Fail2Land1>();
	}
	void click() override
	{
		setState<Try2Land>();
	}
};

template<bool Jump>
struct MoveToFly2 : public ImprovedState<true, 5>
{
	using ImprovedState::ImprovedState;
	void tick() override
	{
		if(Jump)
			Physics::setForcesAfterJump(jumper);
		else
			Physics::setForcesInFlight(jumper);
		moveAnkle(6);
		setKnee(0.0);
		setBottom(0.0);
		setArm(150.0);
		if(--ticksLeft == 0)
			setState<Fly>();
	}
};

template<bool Jump>
struct MoveToFly1 : public ImprovedState<false, 5>
{
	using ImprovedState::ImprovedState;
	void tick() override
	{
		if(Jump)
			Physics::setForcesAfterJump(jumper);			
		else
			Physics::setForcesInFlight(jumper);
		setAnkle(90.0);
		setKnee(0.0);
		setBottom(-90.0);
		setArm(90.0);
		if(--ticksLeft == 0)
			setState<MoveToFly2<Jump>>();
	}
};

struct RunUp : public ImprovedState<false>
{
	using ImprovedState::ImprovedState;
	void tick() override
	{
		Physics::setForcesOnRamp(jumper, *(jumper.terrain));
		if(get_x(jumper.location) > 195.0)
			setState<MoveToFly1<false>>();
	}
	void click() override
	{
		if(get_x(jumper.location) > 180.0)
			setState<MoveToFly1<true>>();
	}
};

struct PushState : public ImprovedState<false, 5>
{
	using ImprovedState::ImprovedState;
	void tick() override
	{
		Physics::setForcesOnRamp(jumper, *(jumper.terrain));
		setAnkle(90.0);
		setKnee(90.0);
		setBottom(-180.0);
		setArm(170.0);
		setSkiOnTerrain();
		if(--ticksLeft == 0)
			setState<RunUp>();
	}
};



struct WaitState : public ImprovedState<false>
{
	using ImprovedState::ImprovedState;
	void tick() override
	{
		setSkiOnTerrain();
	}
	void click() override
	{
		setState<PushState>();
	}
};

Jumper::Jumper(std::shared_ptr<Terrain> terrain) : terrain(terrain), ComplexDrawable({"xD"})
{
	state = std::make_unique<WaitState>(*this);	
}

void Jumper::draw(BetterSDL::Renderer& renderer)
{

}


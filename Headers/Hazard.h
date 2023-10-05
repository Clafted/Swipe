#pragma once

#ifndef HAZARD_H
#define HAZARD_H

#include "Spawnable.h"
#include "Map.h"
#include "Timer.h"

// A subclass of Spawnable that is dangerous to the Player
class Hazard : public Spawnable
{
public:
	enum class DIRECTION{ UP, DOWN, LEFT, RIGHT, NONE };

	Hazard(std::string texturePath, float duration = 3.0f)
		: Spawnable(texturePath, duration)
	{ 
		classID = "Hazard";
		components.push_back(&collider);
		xPos = -100000.0f;
		yPos = -100000.0f;
	}

	~Hazard() 
	{ 
		std::cout << "\nDestroying Hazard"; 
	};

	void activate()
	{
		startDelayTimer.reset();
		Spawnable::activate();

	}

	void update(float updatePeriod)
	{
		// Don't start if still in delay
		if (startDelayTimer.numberOfTicksPassed() == 0)
		{
			startDelayTimer.updateClock(updatePeriod);
		} else {
			Spawnable::update(updatePeriod);
		}
	}

protected:
	Timer startDelayTimer = Timer(3.0f);
};

#endif
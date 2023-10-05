#pragma once

#ifndef TILEWARNING_H
#define TILEWARNING_H

#include "Object.h"
#include "Timer.h"

class TileWarning : public Object
{
public:
	Timer activeTimer = Timer(1.0f);
	bool isActive = false;

	TileWarning(float x, float y) : Object(x, y, "Textures/SingleTileWarning.png") {}
	~TileWarning(){}

	void trigger()
	{
		activeTimer.reset();
		isActive = true;

		std::cout << "\nActivating tile at: (" << xPos << ", " << yPos << ")";
	}

	void update(float updatePeriod)
	{
		activeTimer.updateClock(updatePeriod);
		if (activeTimer.numberOfTicksPassed() > 0) isActive = false;
	}
};

#endif
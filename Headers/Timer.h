#pragma once
#ifndef TIMER_H
#define TIMER_H

#include <math.h>

// A class to keep track of time
class Timer
{
public:
	float duration;

	Timer(float duration) : duration(duration)
	{
		timeIsUp = false;
		clock = 0.0f;
		tickCounter = 1;
	}
	~Timer() {}

	void setDuration(float newDuration) { duration = newDuration; }
	bool getTimeIsUp() { return timeIsUp; }

	// Returns the number of times the timer has reached its period
	int numberOfTicksPassed()
	{
		return tickCounter;
	}

	// Resets both the clock and the tick counter
	void reset()
	{
		resetClock();
		resetTickCounter();
	}

	void resetTickCounter()
	{
		tickCounter = 0;
	}

	void resetClock()
	{
		clock = 0.0f;
	}
	void updateClock(float period)
	{
		if (duration <= 0.0f)
		{			
			timeIsUp = true;
			return;
		}

		clock += period;
		timeIsUp = (clock >= duration);

		if (timeIsUp)
		{	
			clock = fmod(clock, duration);
			tickCounter++;
		}
	}

private:
	float clock;
	int tickCounter;
	bool timeIsUp;
};

#endif

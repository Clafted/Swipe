#pragma once

#ifndef MANHOLE_H
#define MANHOLE_H

#include "Hazard.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class ManHole : public Hazard
{
public:
	ManHole(std::string texturePath, float duration = 1.0f)
		: Hazard(texturePath, duration)
	{
		classID = "ManHole";
		startDelayTimer.setDuration(2.0f);
	}

	void proceed()
	{
		xPos = 50.0f * (currentIndex % map->width) - (((map->width - 1) * map->tileWidth) / 2.0f);
		yPos = -50.0f * (currentIndex / map->width) + (((map->width - 1) * map->tileWidth) / 2.0f);

		if (durationTimer.getTimeIsUp())
		{
			state = STATE::RESETTING;
		}
	}

	void reset()
	{
		availableTiles[currentIndex] = false; //Set currentIndex to unoccupied
		state = STATE::READY;
	}

	void relocate()
	{	
		srand((unsigned int) glfwGetTime());
		currentIndex = rand() % (map->width * map->width);
		
		// Iterate the Map twice to cover tiles before and after the first randomly selected tile
		while (currentIndex < 2 * (map->width * map->width) && availableTiles[currentIndex % (map->width * map->width)] == true)
		{
			currentIndex++;
		}

		if (availableTiles[currentIndex]) //if (selected tile is occupied)
		{
			currentIndex = -1;
		}
		else 
		{
			currentIndex %= (map->width * map->width);
			availableTiles[currentIndex] = true; //Set new currentIndex to occupied
			map->tileWarnings[currentIndex]->activeTimer.setDuration(startDelayTimer.duration);
			map->tileWarnings.at(currentIndex)->trigger();
		}
	}

private:
	int currentIndex = 0;
};

#endif

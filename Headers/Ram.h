#pragma once

#ifndef RAM_H
#define RAM_H

#include "Hazard.h"

// A sbuclass of Hazard which dashes forward increments
class Ram : public Hazard
{
public:
	Ram(std::string texturePath, float duration = 0.45f)
		: Hazard(texturePath, duration)
	{
		classID = "Ram";
		startingX = xPos;
		startingY = yPos;
		speed = 600.0f;
		startDelayTimer.setDuration(1.0f);

		activationSound = SoundManager::Instance()->soundEngine->getSoundSource("SFX/PlainRamHum.wav");
		attackSound = SoundManager::Instance()->soundEngine->getSoundSource("SFX/PlainRamAttack.wav");
		resettingSound = SoundManager::Instance()->soundEngine->getSoundSource("SFX/PlainRamReset.wav");
	}

	void setDirection(DIRECTION direction) { this->direction = direction; }

	//  Implements the ramming attack
	void proceed()
	{
		if (durationTimer.getTimeIsUp())
		{
			// if (moved 3 times already)
			if (moveCount++ == 3)
			{
				state = STATE::RESETTING; 
			}
			else
			{
				if (direction == DIRECTION::UP) moveTarget(0.0f, 50.0f);
				else if (direction == DIRECTION::DOWN) moveTarget(0.0f, -50.0f);
				else if (direction == DIRECTION::LEFT) moveTarget(-50.0f, 0.0f);
				else if (direction == DIRECTION::RIGHT) moveTarget(50.0f, 0.0f);
			}
		}
	}

	void reset()
	{
		// Move to its first position
		moveTarget(startingX - xPos, startingY - yPos);	

		if (!gliding) // if (Ram has reached starting position)
		{
			// Set the pre-occupied row/column to unoccupied
			for (int i = 0; i < map->width; i++)
			{	
				if (yIndex == -1) availableTiles[xIndex + (i * map->width)] = false;
				else availableTiles[i + (yIndex * map->width)] = false;
			}
			moveCount = 0;
			state = STATE::READY; 
		}
	}

	void relocate()
	{
		srand((unsigned int)glfwGetTime());
		if (rand() % 2 == 0)
		{
			yIndex = -1;

			// Change collision dimensions
			collider.width = 50;
			collider.height = 150;

			// Either move top-to-bottom or vice versa
			if (rand() % 2 == 0)
			{
				rotationRadians = 0.0f; // Rotate to 0 degrees
				yPos = -150.0f; // Move to bottom of map
				direction = DIRECTION::UP;
			}
			else
			{
				rotationRadians = 3.1416f; // Rotate to 180 degrees
				yPos = 150.0f; // Move to the top of map
				direction = DIRECTION::DOWN;
			}

			srand((unsigned int)glfwGetTime());
			xIndex = rand() % map->width; // Pick a random column

			for (int i = 0; i < 2; i++)
			{
				if (!columnIsOccupied(xIndex % map->width)) break;

				srand((unsigned int)glfwGetTime());
				xIndex = rand() % map->width; // Pick a random column
			}
			
			if (columnIsOccupied(xIndex % map->width));//ISSUE: FIGURE OUT HOW TO HANDLE A FULLY HAZARD-OCCUPIED MAP

			xPos = (float)((xIndex * map->tileWidth) - (map->width / 2 * map->tileWidth)); // Transform xIndex to world coords
		}
		else
		{
			xIndex = -1;

			// Change collision dimensions
			collider.width = 150;
			collider.height = 50;

			if (rand() % 2 == 0)
			{
				xPos = -150.0f;
				rotationRadians = 3.1416f * 1.5f;
				direction = DIRECTION::RIGHT;
			}
			else
			{
				xPos = 150.0f;
				rotationRadians = 3.1416f / 2.0f;
				direction = DIRECTION::LEFT;
			}

			do { // Search for an empty row
				srand((unsigned int)glfwGetTime());
				yIndex = rand() % map->width; //Pick a random row
			} while (rowIsOccupied(yIndex));
			
			yPos = (float)((-yIndex * map->tileWidth) + (map->width / 2 * map->tileWidth)); //Transform yIndex into world coords
		}

		for (int i = 0; i < map->width; i++)
		{
			// Set a row/column to occupied
			if (yIndex == -1)
			{
				availableTiles[xIndex + (i * 3)] = true;
				map->tileWarnings.at(xIndex + (i * 3))->activeTimer.setDuration(startDelayTimer.duration);
				map->tileWarnings.at(xIndex + (i * 3))->trigger();
			}
			else 
			{
				availableTiles[i + (yIndex * 3)] = true;
				map->tileWarnings.at(i + (yIndex * 3))->activeTimer.setDuration(startDelayTimer.duration);
				map->tileWarnings.at(i + (yIndex * 3))->trigger();
			}
		}

		startingX = xPos;
		startingY = yPos;
	}

	/*Returns true if a row with the yIndex is occupied at all. Returns false otherwise.*/
	bool rowIsOccupied(int yIndex)
	{
		if (yIndex > map->width || yIndex < 0) return false;

		for (int i = 0; i < map->width; i++)
		{
			if (availableTiles[i + (yIndex * map->width)] == true) return true;
		}

		return false;
	}

	/*Returns true if a column with the xIndex coordinate is occupied at all. Returns false otherwise.*/
	bool columnIsOccupied(int xIndex)
	{
		if (xIndex > map->width || xIndex < 0) return false;

		for (int i = 0; i < map->width; i++)
		{
			if (availableTiles[xIndex + (i * map->width)] == true) return true;
		}

		return false;
	}

private:
	DIRECTION direction = DIRECTION::UP;
	float startingX, startingY;
	int moveCount = 0;
};

#endif
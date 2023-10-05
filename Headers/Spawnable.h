#pragma once

#ifndef SPAWNABLE_H
#define SPAWNABLE_H

#include "Object.h"
#include "Map.h"
#include "Timer.h"
#include "Spritesheet.h"
#include "SoundManager.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <irrKlang.h>

/*A subclass of Object which can spawn based on a Map's occupied tiles.*/
class Spawnable : public Object
{
public:
	enum class STATE { READY, ACTIVATED, RESETTING };

	Collider collider;
	STATE state = STATE::READY;
	bool hasCompletedIteration = false;
	static bool* availableTiles;
	static Map* map;
	Timer durationTimer;
	Spritesheet spritesheet;
	int xIndex =  0, yIndex = 0;
	
	/*A method to be called while Spawnable is activated*/
	virtual void proceed() = 0;

	/*A method to be called when Hazard is finished attacking*/
	virtual void reset() = 0;

	/*Set up hazard in a different location in a given Map*/
	virtual void relocate() = 0;

	Spawnable(std::string texturePath, float duration)
		: Object(0, 0, texturePath), spritesheet(texturePath, 1, 1, 1, 1), durationTimer(duration), collider(this, texture.Width(), texture.Height())
	{
		classID = "Spawnable";
	}

	~Spawnable()
	{
	}

	/*Return true if a horizontal/vertical path on the map is occupied. Return false otherwise.*/
	bool pathIsOccupied(int index, bool horizontal)
	{
		if (index == -1) return false;
		for (int i = 0; i < map->width; i++)
		{
			if (availableTiles[((horizontal)? i + (index * 3): index + (i * 3))]) return true;
		}

		return false;
	}

	/*Sets the Spawnable up to be (re)spawned. Plays an activation sound, changes the state, calls relocate(), and resets durationTimer.*/
	virtual void activate()
	{
		// Play activationSound if not already playing
		if (activationSound != nullptr && !SoundManager::Instance()->soundEngine->isCurrentlyPlaying(activationSound))
		{
			SoundManager::Instance()->soundEngine->play2D(activationSound);
		}

		state = STATE::ACTIVATED;
		relocate();

		durationTimer.reset();
	}

	// Override of Object::update(). Progresses spawning cycle
	virtual void update(float updatePeriod)
	{
		Object::update(updatePeriod);

		switch (state)
		{
		case STATE::ACTIVATED:

			if (attackSound != nullptr && !SoundManager::Instance()->soundEngine->isCurrentlyPlaying(attackSound))
			{
				SoundManager::Instance()->soundEngine->play2D(attackSound);
			}

			durationTimer.updateClock(updatePeriod);
			proceed();

			break;
		case STATE::RESETTING:
	
			if (resettingSound != nullptr && !SoundManager::Instance()->soundEngine->isCurrentlyPlaying(resettingSound))
			{
				SoundManager::Instance()->soundEngine->play2D(resettingSound);
			}

			reset();

			if (state == STATE::READY)
			{
				hasCompletedIteration = true;
			}
		}
	}

protected:
	irrklang::ISoundSource* activationSound = nullptr, 
							*attackSound = nullptr,
							*resettingSound = nullptr;
};
#endif SPAWNABLE_H
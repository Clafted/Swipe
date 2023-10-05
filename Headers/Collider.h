#pragma once

#ifndef COLLIDER_H
#define COLLIDER_H

#include "Component.h"
#include "Timer.h"
#include "Object.h"

/*A class to check and handle AABB collisions between other instances.*/
class Collider : public Component
{
public:
	int width, height;

	Collider(Object* contextObject, int width, int height, float collisionCooldown = 0.0f)
		: Component(contextObject), width(width), height(height), collisionCooldown(collisionCooldown), xPos(contextObject->xPos), yPos(contextObject->yPos)
	{}

	~Collider() {}

	/*Update Collider coordinates.*/
	void update(float updatePeriod)
	{
		collisionCooldown.updateClock(updatePeriod);
	}

	/*Returns true if the collision cooldown has ended and other Collider is colliding with this Collider, false otherwise.*/
	bool isCollidingWith(Collider& other)
	{
		return (collisionCooldown.numberOfTicksPassed() > 0 && !(xPos + (width / 2.0f) < other.xPos - (other.width / 2.0f) || xPos - (width / 2.0f) > other.xPos + (other.width / 2.0f) || yPos + (height / 2.0f) < other.yPos - (other.height / 2.0f) || yPos - (height / 2.0f) > other.yPos + (other.height / 2.0f)));
	}

private:
	Timer collisionCooldown;
	float &xPos, &yPos;
};

#endif
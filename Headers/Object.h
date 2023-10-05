#pragma once

#ifndef OBJECT_H
#define OBJECT_H

#include "Texture.h"
#include "Atlas.h"
#include "Component.h"
#include <string>
#include <vector>

// A class for anything that has a texture and a position
class Object
{
public:
	Texture texture;
	std::vector<Component*> components;
	std::string classID = "Object";
	bool gliding = false;

	float rotationRadians = 0.0f;
	float xPos, yPos;
	float xTarget = 0.0f, yTarget = 0.0f;
	float xStarting = 0.0f, yStarting = 0.0f;
	float speed;
	float xVelocity = 0.0f, yVelocity = 0.0f;

	Object(float xPos, float yPos, std::string texturePath)
		: xPos(xPos), yPos(yPos), speed(200.0f), texture(texturePath)
	{
	}

	~Object() {}

	void move(float x, float y) { xPos += x; yPos += y; }
	virtual void glideToTarget();
	virtual void moveTarget(float x, float y);
	virtual void update(float updatePeriod)
	{ 
		this->updatePeriod = updatePeriod;
		if (gliding) glideToTarget();
		for (int i = 0; i < components.size(); i++) components.at(i)->update(updatePeriod);
	}

protected:
	float updatePeriod = 0.0f;
};

#endif
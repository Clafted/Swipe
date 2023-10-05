#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

class Object;

/*A class that grants Objects a certain ability (Sound, Collision, Player Control)*/
class Component
{
public:
	Object & contextObject;
	Component(Object * contextObject) :contextObject(*contextObject) {};
	~Component() {};

	/*Update any variables every frame*/
	virtual void update(float updatePeriod) = 0;

protected:
	float updatePeriod = 0.0f;
};

#endif
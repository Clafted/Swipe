#pragma once

#ifndef PLAYERCONTROLLER
#define PLAYERCONTROLLER

#include "Component.h"
#include "Timer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Player;
class PlayerController : public Component
{
public:
	PlayerController(Player* contextObject, GLFWwindow& window);
	~PlayerController() {}

	void update(float updatePeriod);
private:
	
	GLFWwindow& window;
	Timer moveCooldownTimer;
	float updatePeriod = 0.0f, previousUpdateTime = 0.0f;
};

#endif
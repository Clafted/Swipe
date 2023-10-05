#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"
#include "Collider.h"
#include "Timer.h"
#include "PlayerController.h"
#include "Map.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Player : public Object
{
public:
	Collider collider;
	int lives = 3;

	Player(std::string texturePath, GLFWwindow & window, float xPos, float yPos, Map *map)
		:Object(xPos, yPos, texturePath), window(window), controller(this, window), collider(this, texture.Width(), texture.Height(), 2.0f), map(map)
	{
		classID = "Player";
		components.push_back(&controller);
		components.push_back(&collider);
		speed = 500.0f;	
	}

	void moveTarget(float x, float y);
	void update(float updatePeriod);
	void damage(int dmg);
	void handleCollisionWith(Collider& other);

private:
	PlayerController controller;
	Timer damageCooldownTimer = Timer(3.0f);
	GLFWwindow& window;
	Map* map = nullptr;
};

#endif
#pragma once
#ifndef CLICKABLE_H
#define CLICKABLE_H

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Texture.h"

/*A class for anything that can be, put simply, clicked.*/
class Clickable
{
public:
	Texture texture;
	int xPos, yPos, width, height;

	Clickable(std::string texturePath, int xPos, int yPos, int width, int height) 
		: texture(texturePath), xPos(xPos), yPos(yPos), width(width), height(height) {}
	~Clickable(){}

	int ClickCount() { return clickCount; }
	bool Pressed() { return pressed; }

	// Set the method to be called when Clickable has been clicked
	void setOnClickAction(void (*action)())
	{ this->action = action; }

	/*Check if this Clickable has been clicked and perform action() if so.*/
	void checkIfClicked(GLFWwindow & window, int viewportWidth, int viewportHeight)
	{
		double cursorX, cursorY;
		glfwGetCursorPos(&window, &cursorX, &cursorY);

		cursorX -= viewportWidth / 2.0f; //Correct the X-coordinate
		cursorY = -(cursorY - viewportHeight / 2.0f); //Correct the Y-coordinate

		// if (left-clicked && NOT previously clicked && cursor is within bounds)
		if (glfwGetMouseButton(&window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && clickCount == 0 && cursorX >= xPos - (width / 2.0f) && cursorX <= xPos + (width / 2.0f) && cursorY <= yPos + (height / 2.0f) && cursorY >= yPos - (height / 2.0f))
		{
			if (action) action(); //call action() if it exists
			clickCount++;
		}
		else if(glfwGetMouseButton(&window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE)
		{ 
			clickCount = 0; 
		}

		pressed = clickCount > 0;
	}

private:
	void (*action)() = NULL;
	int clickCount = 0;
	bool pressed = false;
};

#endif
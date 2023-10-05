#pragma once

#ifndef MENU_H
#define MENU_H

#include "Scene.h"
#include "GameLoop.h"
#include "Texture.h"
#include "Clickable.h"

class Menu : public Scene
{
public:
	Menu(GLFWwindow* window, Shader * shader, int viewportWidth = 1200, int viewportHeight = 800) 
		: Scene(window, shader, viewportWidth, viewportHeight)
	{ 
		std::cout << "\nCreating Menu";
	}
	~Menu() override{ std::cout << "\nDestroying Menu"; }

	Scene* refresh () override
	{
		// Check Clickable clicks
		start.checkIfClicked(window, (int)viewportDimensions.x, (int)viewportDimensions.y);

		// Reset view matrix
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, &screenScaling[0][0]);
		
		drawTexture(background, glm::vec3(0.0f), 0.0f, glm::vec3(4.0f, 4.0f, 1.0f));
		drawTexture(start.texture, glm::vec3(start.xPos, start.yPos, 1.0f), 0.0f, glm::vec3(4.0f, 4.0f, 1.0f));

		if (start.Pressed()) return new GameLoop(&window, &shader, (int)viewportDimensions.x, (int)viewportDimensions.y);

		return nullptr;
	}

private:
	Texture background = Texture("Textures/MenuBackground.png");
	Clickable start = Clickable("Textures/StartButton.png", 0, 80, 600, 160);
};

#endif
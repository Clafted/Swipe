#pragma once

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Scene.h"
#include "Menu.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

/*A class to manage the creation, switching, and destroying of scenes.Implements the State Pattern*/
class SceneManager
{
public:
	// Singleton instantiation
	static SceneManager* Instance(GLFWwindow * window, Shader * shader)
	{
		if (instance == nullptr)
		{
			instance = new SceneManager();

			SceneManager::window = window;
			SceneManager::shader = shader;
			currentScene = new Menu(window, shader,viewportWidth, viewportHeight);
		}

		return instance;
	}

	// Update the size of the viewport
	static void resizeViewport(int width, int height)
	{ 
		viewportWidth = width;
		viewportHeight = height;

		currentScene->resizeViewport(width, height);
	}

	// Update the scene and switch if needed
	static void refresh()
	{
		// Refresh current Scene and save any new Scenes
		Scene* tempScene = currentScene->refresh();

		// Check if Scene is switching
		if (tempScene != nullptr)
		{
			delete currentScene;
			currentScene = tempScene;
		}
	}

private:
	SceneManager() { std::cout << "\nCreating SceneManager"; };
	~SceneManager() { delete instance; delete currentScene; std::cout << "Destroying SceneManager"; };

	static SceneManager * instance;
	static GLFWwindow* window;
	static Shader* shader;
	static Scene* currentScene;

	static int viewportWidth, viewportHeight;
};

#endif

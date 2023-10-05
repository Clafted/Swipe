#pragma once

#ifndef SCENE_H
#define SCENE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Collider.h"
#include "Texture.h"
#include "stb_image.h"
#include "Shader.hpp"

/*A class to handle the refreshingand rendering of texures.*/
class Scene
{
public:
	Scene(GLFWwindow* window, Shader* shader, int viewportWidth = 1200, int viewportHeight = 800) 
		: window(*window), shader(*shader), viewportDimensions(viewportWidth, viewportHeight) 
	{ std::cout << "\nDefault Scene created"; }
	virtual ~Scene() { std::cout << "\nDestroying Scene"; }

	virtual Scene* enterScene() { return nullptr; }
	virtual Scene* exitScene() { return nullptr; }
	// The method to render all the content. To be implemented by all subclasses
	virtual Scene* refresh() = 0;

	void calculateUpdatePeriod()
	{
		updatePeriod = (float)glfwGetTime() - previousUpdateTime;
		if (updatePeriod > 0.15f) updatePeriod = 0.15f;

		previousUpdateTime = (float)glfwGetTime();
	}

	void resizeViewport(int width, int height) 
	{ 
		viewportDimensions = glm::vec2(width, height); 
		// Recalculate scaling transformation
		screenScaling = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f / viewportDimensions.x, 2.0f / viewportDimensions.y, 1.0f));
	}

protected:
	GLFWwindow& window;
	Shader& shader;
	glm::vec2 viewportDimensions;
	bool wireframeVisibility = true;
	float updatePeriod = 0.0f, previousUpdateTime = 0.0f;

	// A matrix transformation to keep textures the right size even when rescaling
	glm::mat4 screenScaling = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f / viewportDimensions.x, 2.0f / viewportDimensions.y, 1.0f));

	// Draw a texture with the given transformations, and include the wireframe if preferred
	void drawTexture(Texture& texture, glm::vec3 translate, float rotation, glm::vec3 scale, bool includeWireframe = false)
	{
		// Apply transformations
		glm::mat4 model = glm::translate(glm::mat4(1.0f), translate);
		model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, scale);
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, &model[0][0]);
		// Draw the Texture
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(texture.VAO());
		glBindTexture(GL_TEXTURE_2D, texture.TextureID());
		glDrawArrays(GL_TRIANGLES, 0, 6);		

		// Draw the wireframe on top if needed
		if (wireframeVisibility == true && includeWireframe == true)
		{
			// Change wireframe color to solid green
			glUniform4fv(glGetUniformLocation(shader.ID, "wireframeColor"), 1, &glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)[0]);
			// Make OpenGL draw empty lines
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			// Unbind texture
			glBindTexture(GL_TEXTURE_2D, 0);
			// Draw wireframe
			glDrawArrays(GL_TRIANGLES, 0, 6);

			// Reset uniform variables
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glUniform4fv(glGetUniformLocation(shader.ID, "wireframeColor"), 1, &glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)[0]);
		}
		
		glBindVertexArray(0);
	}
};

#endif
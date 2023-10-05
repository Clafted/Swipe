#pragma once
#ifndef  TEXTURE_H
#define TEXTURE_H

#include "stb_image.h"
#include <glad/glad.h>
#include <string>
#include <iostream>

/*A class to make it easier to import textures*/
class Texture
{
public:
    // Constructor
	Texture(std::string imagePath = "")
		: imagePath(imagePath)
	{
        createVBOAndVAO();
        glBindVertexArray(vao);

        // Generate Texture and bind it for configuration
        glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Set wrapping and mipmap configurations
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Load texture data
        stbi_set_flip_vertically_on_load(true);
        imageData = stbi_load(imagePath.c_str(), &width, &height, &numberOfChannels, 0);

        // Use default texture if given texture is not found.
        if (imageData == NULL)
        {
            std::cout << "Failed to load image texture \"" + imagePath + "\"! Resorting to default texture" << std::endl;
            imageData = stbi_load("Textures/TextureNotFound.png", &width, &height, &numberOfChannels, 0);
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Free data
        stbi_image_free(imageData);

        // Texture coordinates (origin is located at the center of the texture)
        coordinates = new float[] 
        {
                width / -2.0f, height / 2.0f, 0, 0, 1,
                width / 2.0f, height / 2.0f, 0, 1, 1,
                width / -2.0f, height / -2.0f, 0, 0, 0,
                width / -2.0f, height / -2.0f, 0, 0, 0,
                width / 2.0f, height / 2.0f, 0, 1, 1,
                width / 2.0f, height / -2.0f, 0, 1, 0
        };

        glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(float), coordinates, GL_STATIC_DRAW);
        glBindVertexArray(0);
	}

    ~Texture() 
    { 
        delete[] coordinates; 
        std::cout << "\nDeleting Texture";  
    }

    std::string filePath() const { return imagePath; }
    unsigned int TextureID() const { return textureID; }
    unsigned int VAO() const { return vao; }
    unsigned int VBO() const { return vbo; }
    int Height() { return height; }
    int Width() { return width; }

    void createVBOAndVAO()
    {
        // Generate and bind vao and vbo
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // Set attribute 0
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Set attribute 1
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Unbind vao
        glBindVertexArray(0);
    }

protected:
	std::string imagePath;
    unsigned int textureID;
    unsigned int vao = 0, vbo = 0;
	unsigned char* imageData;
    int width, height, numberOfChannels;

    float* coordinates = nullptr;
};

#endif // ! TEXTURE_H
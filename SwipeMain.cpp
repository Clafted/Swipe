#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Headers/Shader.hpp"
#include "Headers/SceneManager.h"
#include "Headers/SoundManager.h"

SceneManager* SceneManager::instance = nullptr;
GLFWwindow* SceneManager::window = nullptr;
Shader* SceneManager::shader = nullptr;
Scene* SceneManager::currentScene = nullptr;
int SceneManager::viewportWidth = 1200, SceneManager::viewportHeight = 800;

SoundManager* SoundManager::instance = nullptr;

void glResizeViewportCallback(GLFWwindow * window, int width, int height);

int main()
{
    SoundManager::Instance()->soundEngine->setSoundVolume(0.2f);

    // Variables
    GLFWwindow* window;
    int windowWidth, windowHeight;

    // Setting up the window -----------------------------------------------------

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Try to create a window
    windowWidth = 1200, windowHeight = 800;
    window = glfwCreateWindow(windowWidth, windowHeight, "Swipe", NULL, NULL);

    // Check if window has been created
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD (Callback linker)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, windowWidth, windowHeight);

    // Set GLAD callbacks
    glfwSetFramebufferSizeCallback(window, glResizeViewportCallback);

    // Finish setting up the window -----------------------------------------------

    //Shaders
    Shader textureShader("Shaders/textureShader.vs", "Shaders/textureShader.fs");
    textureShader.use();
    textureShader.setInt("txtr", 0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set up the SceneManager
    SceneManager::Instance(window, &textureShader);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        SceneManager::refresh();
 
        glfwSwapBuffers(window);
    }
    std::cout << "\nClosing the game...\nGoodbye!";
    glfwTerminate();
    return 0;
}

void glResizeViewportCallback(GLFWwindow* window, int width, int height)
{ 
    glViewport(0, 0, width, height); 
    SceneManager::resizeViewport(width, height);
}
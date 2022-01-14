#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "RenderEngine.h"

//is the gameloop
class GameLoop
{
public:
	void start();
};

// TODO: change display manager manager width, height on callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(Camera& camera);
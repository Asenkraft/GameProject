#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "RenderEngine.h"

//is the gameloop
class GameLoop
{
public:
	void start();

private:
	uint64_t m_time_Accumulated = 0;
	uint64_t m_time_Accumulated_To_One_Seconds = 0;
	uint32_t m_FPS = 0;
	uint32_t m_UPS = 0;
	double FPS_144 = 144.0;
	double FPS_60 = 60.0;
	double FPS_30 = 30.0;
	double FPS_15 = 15.0;
	double frameTime = (1.0 / FPS_60) * 1000;
};

// TODO: change display manager manager width, height on callback
void scroll_callback(GLFWwindow* window, double xpos, double ypos);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(Camera& camera, Player& player, Master_Renderer& renderer);


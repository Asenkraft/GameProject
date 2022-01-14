#pragma once
#include "Shader.h"
#include <unordered_map>
#include <vector>

//glad and GLFW Import for OpenGL and Wrapper stuff
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//creates, updates and closes windows
class DisplayManager
{
public:
	void createDisplay();
	void updateDisplay(GLFWwindow* window);
	void closeDisplay();
	void loadModel();

private:
	uint32_t m_Width = 1280;
	uint32_t m_Height = 720;

	ReturnTimer m_ReturnTimer;
	uint64_t m_time_Accumulated = 0;
	uint64_t m_time_Accumulated_To_One_Seconds = 0;
	uint32_t m_FPS = 0;
	uint32_t m_UPS = 0;
	double frameTime = (1.0 / 144.0) * 1000;
};

//renders stuff
class Entity_Renderer
{
private:
	StaticShader m_Shader;

public:
	Entity_Renderer(StaticShader& Shader, glm::mat4& Projection_Matrix);
	void render(std::unordered_map<int, std::vector<Entity>>& Entities);

private:
	void prepare_Textured_Model(Model_Textured& model);
	void unbind_TexturedModel();
	void prepare_Instances(Entity& entity);

	Entity_Renderer() = delete;
	MatrixMath m_Matrix_Math;
};

class Terrain_Renderer
{
private:
	Terrain_Shader m_Shader;

public:
	Terrain_Renderer(Terrain_Shader& shader, glm::mat4& projectionMatrix);
	void render(std::vector<Terrain>& terrains);

private:
	void prepare_Terrain_Mesh(Terrain& terrain); //Terrain Model with texture
	void unbind_Model_Terrain();
	void prepare_Terrain_Instances(Terrain& terrain);

	MatrixMath m_Matrix_Math;
};


class Master_Renderer
{
private:
	glm::mat4 create_Projection_Matrix();
	
private:

	StaticShader m_Shader;
	Terrain_Shader m_Terrain_Shader;

	std::vector<Terrain> m_Terrains;
	//hashmap with <key, value> pair
	std::unordered_map<int, std::vector<Entity>> m_Entities;

	const float m_FOV = 70.0f;
	const float m_Near_Plane = 0.1f;
	const float m_Far_Plane = 1000.0f;
	glm::mat4 m_Projection_Matrix;

	const float m_Red = 0.5f;
	const float m_Green = 0.5f;
	const float m_Blue = 0.5f;

	Entity_Renderer m_Entity_Renderer;
	Terrain_Renderer m_Terrain_Renderer;

public:
	static void enable_Culling();
	static void disable_Culling();
	void cleanUp();
	void render(Light& sun, Camera& camera);
	void processEntity(Entity& entity);
	void processTerrain(Terrain& terrain);
	void prepare();
	Master_Renderer();
};



static bool was_Size_Callback_Called = false;
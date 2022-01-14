#include "RenderEngine.h"
#include <glm/glm/gtc/matrix_transform.hpp>

void DisplayManager::createDisplay()
{
	GLFWwindow* window = glfwCreateWindow(m_Width, m_Height, "ThinMatrix Tutorial structured", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window\n";
		closeDisplay();
	}
	else
	{
		std::cout << "Created GLFW window...\n";
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD\n";
	}
	else
	{
		std::cout << "Initialized GLAD...\n";
		std::cout << "created window...\n";
	}
}

void DisplayManager::updateDisplay(GLFWwindow* window)
{
	glfwSwapBuffers(window);
	glfwPollEvents();

}

void DisplayManager::closeDisplay()
{
	glfwTerminate();
}

void DisplayManager::loadModel()
{

}

Entity_Renderer::Entity_Renderer(StaticShader& Shader, glm::mat4 &Projection_Matrix)
	:m_Shader(Shader)
{
	m_Shader = Shader;
	m_Shader.start();
	m_Shader.load_Projection_Matrix(Projection_Matrix);
	m_Shader.stop();
}

void Entity_Renderer::render(std::unordered_map<int, std::vector<Entity>>& Entities)
{
	for (auto const& pair : Entities)
	{
		int i = 0;		
		std::vector<Entity> batch = pair.second;
		
		Model_Textured model = batch[i++].get_Textured_Model();
		prepare_Textured_Model(model);
		for (Entity entity : batch)
		{
			prepare_Instances(entity);
			glDrawElements(GL_TRIANGLES, model.get_Model_Raw().get_VertexCount(), GL_UNSIGNED_INT, 0);
		}
		unbind_TexturedModel();
	}
}

void Entity_Renderer::prepare_Textured_Model(Model_Textured& model)
{
	Model_Raw raw_Model = model.get_Model_Raw();
	glBindVertexArray(raw_Model.get_VAO_ID());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glad_glEnableVertexAttribArray(2);
	Texture texture = model.get_Texture();

	if (texture.get_hasTransparency())
	{
		Master_Renderer::disable_Culling();
	}
	m_Shader.load_useFakeLighting(model.get_Texture().get_useFakeLighting());
	m_Shader.load_Shine_Var(texture.get_Shine_Damper(), texture.get_Reflectivity());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model.get_Texture().get_Texture_ID());
}

void Entity_Renderer::unbind_TexturedModel()
{
	Master_Renderer::enable_Culling();
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
}

void Entity_Renderer::prepare_Instances(Entity& entity)
{
	glm::mat4 transformationMatrix = m_Matrix_Math.create_Transformation_Matrix(entity.get_Position(), entity.get_Rot_X(), entity.get_Rot_Y(), entity.get_Rot_Z(), entity.get_Scale());
	m_Shader.load_Transformation_Matrix(transformationMatrix);
}


Terrain_Renderer::Terrain_Renderer(Terrain_Shader& shader, glm::mat4& projection_Matrix)
	:m_Shader(shader)
{
	m_Shader = shader;
	m_Shader.start();
	m_Shader.load_Projection_Matrix(projection_Matrix);
	m_Shader.stop();
}

void Terrain_Renderer::render(std::vector<Terrain>& terrains)
{
	for (Terrain terrain : terrains)
	{
		prepare_Terrain_Mesh(terrain);
		prepare_Terrain_Instances(terrain);
		glDrawElements(GL_TRIANGLES, terrain.get_Model_Raw_Terrain().get_VertexCount(), GL_UNSIGNED_INT, 0);
		unbind_Model_Terrain();

	}
}

void Terrain_Renderer::prepare_Terrain_Mesh(Terrain& terrain)
{
	Model_Raw raw_Model = terrain.get_Model_Raw_Terrain();
	glBindVertexArray(raw_Model.get_VAO_ID());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glad_glEnableVertexAttribArray(2);
	Texture texture = terrain.get_Texture_Raw_terrain();
	m_Shader.load_Shine_Var(texture.get_Shine_Damper(), texture.get_Reflectivity());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.get_Texture_ID());
}

void Terrain_Renderer::unbind_Model_Terrain()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
}

void Terrain_Renderer::prepare_Terrain_Instances(Terrain& terrain)
{
	glm::mat4 transformationMatrix = m_Matrix_Math.create_Transformation_Matrix(glm::vec3(terrain.get_X(), 0.0f, terrain.get_Z()), 0.0f, 0.0f, 0.0f, 1.0f);
	m_Shader.load_Transformation_Matrix(transformationMatrix);
}

glm::mat4 Master_Renderer::create_Projection_Matrix()
{
	GLFWwindow* window = glfwGetCurrentContext();

	glm::mat4 projection_Matrix(0.0);

	//static problems
	float aspectRatio = 1280.0f / 720.0f;
	// aspectRatio = 1.0f;

	float FOV_in_Radians = (m_FOV / 2.0) * glm::pi<float>() / 180;
	float y_scale = (1.0f / glm::tan(glm::radians(m_FOV / 2.0f))) * aspectRatio;
	float x_scale = y_scale / aspectRatio;
	float frustum_length = m_Far_Plane - m_Near_Plane;

	projection_Matrix[0][0] = x_scale;
	projection_Matrix[1][1] = y_scale;
	projection_Matrix[2][2] = -((m_Far_Plane + m_Near_Plane) / frustum_length);
	projection_Matrix[2][3] = -1.0f;
	projection_Matrix[3][2] = -((2.0f * m_Near_Plane * m_Far_Plane) / frustum_length);
	projection_Matrix[3][3] = 0.0f;

	return projection_Matrix;
}

void Master_Renderer::cleanUp()
{
	m_Shader.clean_Up();
	m_Terrain_Shader.clean_Up();
}

void Master_Renderer::processTerrain(Terrain& terrain)
{
	m_Terrains.push_back(terrain);
}

void Master_Renderer::enable_Culling()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void Master_Renderer::disable_Culling()
{
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}


void Master_Renderer::prepare()
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(m_Red, m_Green, m_Blue, 1.0f);
}

Master_Renderer::Master_Renderer()
	:m_Projection_Matrix(create_Projection_Matrix()), m_Entity_Renderer(Entity_Renderer(m_Shader, m_Projection_Matrix)), m_Terrain_Renderer(Terrain_Renderer(m_Terrain_Shader, m_Projection_Matrix))
{
	enable_Culling();
}

void Master_Renderer::render(Light& sun, Camera& camera)
{
	prepare();

	//entities
	m_Shader.start();
	m_Shader.load_skyColor(m_Red, m_Green, m_Blue);
	m_Shader.load_Light(sun);
	m_Shader.load_View_Matrix(camera);
	m_Entity_Renderer.render(m_Entities);
	m_Shader.stop();

	//terrain
	m_Terrain_Shader.start();
	m_Shader.load_skyColor(m_Red, m_Green, m_Blue);
	m_Terrain_Shader.load_Light(sun);
	m_Terrain_Shader.load_View_Matrix(camera);
	m_Terrain_Renderer.render(m_Terrains);

	m_Terrains.clear();
	m_Entities.clear();
}


void Master_Renderer::processEntity(Entity& entity)
{
	Model_Textured entityModel = entity.get_Textured_Model();
	int ID = entityModel.get_Model_Raw().get_VAO_ID();
	if (m_Entities.contains(ID))
	{
		m_Entities[ID].push_back(entity);
	}
	else
	{
		std::vector<Entity> new_Batch;
		new_Batch.push_back(entity);
		m_Entities[ID] = new_Batch;
	}
}


#include "Game.h"

Camera camera;

void GameLoop::start()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	DisplayManager DP;
	DP.createDisplay();

	GLFWwindow* window = glfwGetCurrentContext();
	
	//StaticShader staticShader;
	//Renderer renderer(staticShader);
	
	Loader loader;
	Importer importer;
	Object_Importer object_importer;

	Model_Raw raw_Model_Dragon = object_importer.load_Object_Model("res/Dragon/dragon.obj");
	Texture texture_Dragon(loader.load_Texture("res/Dragon/dragon.png"));

	Model_Raw raw_Model_Cube = object_importer.load_Object_Model("res/Cube/texturedCube.obj");
	Texture texture_Cube(loader.load_Texture("res/Cube/numberedSplitColoredCubeBLENDER.png"));

	Model_Raw raw_Model_Girl = object_importer.load_Object_Model("res/Girl/girl.obj");
	Texture textured_Girl(loader.load_Texture("res/Girl/BOdy Skin Base Color.png"));

	Model_Raw raw_Model_Block = object_importer.load_Object_Model("res/Terrain/gras_Cube.obj");
	Texture basic_Gras_Block_Texture(loader.load_Texture("res/Terrain/basic_gras_block.png"));

	Model_Raw raw_Model_Fern = object_importer.load_Object_Model("res/Terrain/fern.obj");
	Texture textured_Fern(loader.load_Texture("res/Terrain/fern.png"));

	Model_Raw raw_Model_Stall = object_importer.load_Object_Model("res/Stall/stall.obj");
	Texture textured_Stall(loader.load_Texture("res/Stall/stallTexture.png"));


	texture_Dragon.set_Shine_Damper(10);
	texture_Dragon.set_Reflectivity(1);

	texture_Cube.set_Shine_Damper(10);
	texture_Cube.set_Reflectivity(1);

	textured_Girl.set_Shine_Damper(10);
	textured_Girl.set_Reflectivity(0.1f);

	textured_Fern.set_hasTransparency(true);
	textured_Fern.set_useFakeLighting(true);

	Model_Textured textured_Model_Dragon(raw_Model_Dragon, texture_Dragon);
	Model_Textured textured_Model_Cube(raw_Model_Cube, texture_Cube);
	Model_Textured textured_Model_Girl(raw_Model_Girl, textured_Girl);
	Model_Textured textured_Model_Fern(raw_Model_Fern, textured_Fern);
	Model_Textured textured_Model_Stall(raw_Model_Stall, textured_Stall);
	Model_Textured textured_Model_Gras_Block(raw_Model_Block, basic_Gras_Block_Texture);

	
	
	Entity entity_Dragon1(textured_Model_Dragon, glm::vec3(0.0f, 0.0f, -55.0f), 0.0f, 0.0f, 0.0f, 1.0f);
	Entity entity_Dragon2(textured_Model_Dragon, glm::vec3(0.0f, 0.0f, -35.0f), 0.0f, 0.0f, 0.0f, 1.0f);
	Entity entity_cube1(textured_Model_Cube, glm::vec3(0.0f, 0.0f, -25.0f), 0.0f, 0.0f, 0.0f, 1.0f);
	Entity entity_Stall(textured_Model_Stall, glm::vec3(10.0f, 0.0f, -15.0f), 0.0f, 0.0f, 0.0f, 1.0f);

	Entity entity_Gras_Cube1(textured_Model_Gras_Block, glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.0f, 1.0f);
	Entity entity_Gras_Cube2(textured_Model_Gras_Block, glm::vec3(-0.2f, 0.0f, 0.0f), 0.0f, 0.0f, 0.0f, 1.0f);
	Entity entity_Gras_Cube3(textured_Model_Gras_Block, glm::vec3(0.0f, 0.0f, -0.2f), 0.0f, 0.0f, 0.0f, 1.0f);
	Entity entity_Gras_Cube4(textured_Model_Gras_Block, glm::vec3(-0.2f, 0.0f, -0.2f), 0.0f, 0.0f, 0.0f, 1.0f);
	Entity entity_Gras_Cube5(textured_Model_Gras_Block, glm::vec3(0.0f, 0.0f, -0.2f), 0.0f, 0.0f, 0.0f, 1.0f);
	Entity entity_Gras_Cube6(textured_Model_Gras_Block, glm::vec3(0.2f, 0.0f, -0.2f), 0.0f, 0.0f, 0.0f, 1.0f);
	Entity entity_Fern1(textured_Model_Fern, glm::vec3(0.0f, 0.2f, 2.4f), 0.0f, 0.0f, 0.0f, 0.2f);

	Terrain terrain = Terrain(0, -1, loader, Texture(loader.load_Texture("res/Terrain/grass.png")));
	Terrain terrain2 = Terrain(-1, -1, loader, Texture(loader.load_Texture("res/Terrain/grass2.png")));
	Terrain terrain3 = Terrain(-1, 0, loader, Texture(loader.load_Texture("res/Terrain/grass.png")));
	Terrain terrain4 = Terrain(0, 0, loader, Texture(loader.load_Texture("res/Terrain/grass2.png")));

	Light light(glm::vec3(0.0f, 500.0f, -20.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	Player player(textured_Model_Girl, glm::vec3(0.0f, 0.0f, -15.0f), 0.0f, 2.0f, 0.0f, 1.0f);
	

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//glfwSetScrollCallback(window, scroll_callback);

	Master_Renderer renderer;

	while (!glfwWindowShouldClose(window))
	{
		Timer timer;

		renderer.processTerrain(terrain);
		renderer.processTerrain(terrain2);
		renderer.processTerrain(terrain3);
		renderer.processTerrain(terrain4);
		renderer.processEntity(entity_Dragon1);
		renderer.processEntity(entity_Dragon2);
		renderer.processEntity(entity_cube1);
		renderer.processEntity(entity_Fern1);
		renderer.processEntity(entity_Stall);

	

		renderer.processEntity(entity_Gras_Cube1);
		renderer.processEntity(entity_Gras_Cube2);
		renderer.processEntity(entity_Gras_Cube3);
		renderer.processEntity(entity_Gras_Cube4);
		renderer.processEntity(entity_Gras_Cube5);
		renderer.processEntity(entity_Gras_Cube6);
		

		renderer.render(light, camera);

		entity_Dragon1.increase_Rotation(0.0f, 0.0002f, 0.0f);
		entity_Dragon2.increase_Rotation(0.0f, 0.0002f, 0.0f);
		entity_cube1.increase_Rotation(0.0f, 0.0002f, 0.0f);
		
		processInput(camera, player);

		m_UPS++;
		m_time_Accumulated += timer.measureTime();
		m_time_Accumulated_To_One_Seconds += timer.measureTime();

	


		player.move(timer.measureTime());
		renderer.processEntity(player);

		if (m_time_Accumulated_To_One_Seconds >= 1000000)
		{
			std::cout << "(FPS|UPS) (" << m_FPS <<  "|" << m_UPS << ")\r";
			m_time_Accumulated_To_One_Seconds = 0;
			m_FPS = 0;
			m_UPS = 0;
		}
		if (m_time_Accumulated / 1000 > frameTime)
		{
			m_time_Accumulated = 0;
			m_FPS++;



			DP.updateDisplay(window);
		}
		timer.~Timer();
	}
	DP.closeDisplay();
	loader.cleanUp();
	renderer.cleanUp();
}




void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	if (!was_Size_Callback_Called)
	{
		std::cout << "\n";
		was_Size_Callback_Called = true;
	}
std:printf("Viewport is changed: width: %i height: %i\r", width, height);
}

void processInput(Camera& camera, Player& player)
{

// TODO: KeyCallback
	GLFWwindow* window = glfwGetCurrentContext();
	//ESC
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		std::cout << "\n\nEscape pressed. Window will be closed...\n";
		glfwSetWindowShouldClose(window, true);
	}

	//W
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			camera.keyPressW(true);
		}
		else
		{
			camera.keyPressW(false);
		}
		
	}

	//A
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.keyPressA();
	}

	//S
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			camera.keyPressS(true);
		}
		else
		{
			camera.keyPressS(false);
		}
	}

	//D
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.keyPressD();
	}

	//Q
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		camera.keyPressQ();
	}

	//E
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		camera.keyPressE();
	}

	//R
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		camera.keyPressR();
	}

	//F
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		camera.keyPressF();
	}

	//T
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
	{
		camera.keyPressT();
	}

	//G
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
	{
		camera.keyPressG();
	}


	//Arrow Up
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		player.move_UP(true);
	}
	//Arrow Down
	else  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		player.move_Down(true);

	}
	else player.move_Down(false);

	//Arrow Left
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		player.move_Left(true);
	}
	//Arrow Right
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		player.move_Right(true);
	}
	else player.move_Right(false);
	//Space
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		player.move_Jump(true);
	}

}

void scroll_callback(GLFWwindow* window, double xpos, double ypos)
{
	std::cout << "ypos: " << ypos << "\n";
}
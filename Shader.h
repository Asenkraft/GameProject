#pragma once
#include "Model.h"
#include "Utility.h"

#include <iostream>


class Light
{
public:
	Light(glm::vec3 position, glm::vec3 color);

	glm::vec3 get_Position();
	glm::vec3 get_Color();
	void set_Position(glm::vec3 position);
	void set_Color(glm::vec3 color);

private: 
	glm::vec3 m_Position;
	glm::vec3 m_Color;
};

//moving the models
class Camera
{
public:
	void move();
	//move forward (w) or up (shift+w)
	void keyPressW(bool isShiftPressed);
	//move backward (s) or down (shift+s)
	void keyPressS(bool isShiftPressed);
	//move left
	void keyPressA();
	//move right
	void keyPressD();
	//roll left
	void keyPressQ();
	//roll right
	void keyPressE();
	//tilt left
	void keyPressR();
	//tilt right
	void keyPressF();
	//height up
	void keyPressT();
	//height down
	void keyPressG();

public:
	glm::vec3 getPosition();
	float getPitch();
	float getYaw();
	float getRoll();
	Camera();
private:
	float m_Distance_From_Player = 50;
	float m_Angle_Around_Player = 0;
	void calculate_Zoom();



	glm::vec3 m_position = glm::vec3(1.0f);
	//height of camera
	float m_pitch = 0.0f;
	//left/right position of camera
	float m_yaw = 0.0f;
	//tilt of camera
	float m_roll = 0.0f;

	bool isCameraMoving = false;

	//speed of movement
	const float m_movement_speed = 0.02f;
};

//Calculations for projection, transformation, camera movement etc.
class MatrixMath
{
public:
	glm::mat4 create_Transformation_Matrix(glm::vec3 translation, float rot_X, float rot_Y, float rot_Z, float scale);
	glm::mat4 create_View_Matrix(Camera camera);
};

//abstract superclass for Shaders
class Shader
{
public:
	Shader(const std::string& Vertex_Shader_File_Path ,const std::string& Fragment_Shader_File_Path);

public:
	void start();
	void stop();
	void clean_Up();

protected:
	static int load_Shader(const std::string& file, int type);
	void bind_Attribute(int attribute, const char* variableName);

	virtual void bind_All_Attributes() = 0;
	virtual int get_Uniform_Location(const char* uniformName) = 0;
	virtual void get_All_Uniform_Location() = 0;

	void load_Float(int location, float value);
	void load_Vector(int location, glm::vec3 vector);
	void load_Bool(int location, bool value);
	void load_Matrix(int location, glm::mat4 matrix);

protected:
	int m_Program_ID;
	int m_Vertex_Shader_ID;
	int m_Fragment_Shader_ID;

	int m_Location_Transformation_Matrix;
	int m_Location_Projection_Matrix;
	int m_Location_View_Matrix;
	int m_Location_Light_Position;
	int m_Location_Light_Color;
	int m_Location_shineDamper;
	int m_Location_Reflectivity;
	int m_Location_useFakeLighting;
	int m_Location_skyColor;

	Importer importer;
	Loader loader;
	MatrixMath m_Matrix_Math;
};

//class for static models
class StaticShader : public Shader
{
public:
	void load_Transformation_Matrix(glm::mat4 transformationMatrix);
	void load_Projection_Matrix(glm::mat4 m_Projection_Matrix);
	void load_View_Matrix(Camera camera);
	void load_Light(Light light);
	void load_Shine_Var(float damper, float reflectivity);
	void load_useFakeLighting(bool useFakeLighting);
	void load_skyColor(float r, float g, float b);

	void bind_All_Attributes() override;

	StaticShader();

private:
	int get_Uniform_Location(const char* uniformName) override;
	void get_All_Uniform_Location() override;
	std::string vertex_Shader_File_Path = "shader/VertexShader.txt";
	std::string fragment_Shader_File_Path = "shader/FragmentShader.txt";
};

class Terrain_Shader : public Shader
{
public:
	void load_Transformation_Matrix(glm::mat4 transformationMatrix);
	void load_Projection_Matrix(glm::mat4 m_Projection_Matrix);
	void load_View_Matrix(Camera camera);
	void load_Light(Light light);
	void load_Shine_Var(float damper, float reflectivity);
	void load_skyColor(float r, float g, float b);

	void bind_All_Attributes() override;

	Terrain_Shader();

private:
	int get_Uniform_Location(const char* uniformName) override;
	void get_All_Uniform_Location() override;
	std::string vertex_Shader_File_Path = "shader/TerrainVertexShader.txt";
	std::string fragment_Shader_File_Path = "shader/TerrainFragmentShader.txt";
};

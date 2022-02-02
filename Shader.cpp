#include "Shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/gtc/type_ptr.hpp>

void Camera::move()
{
}

void Camera::keyPressW(bool isShiftPressed)
{
	if (isShiftPressed)
	{
		m_position.y += m_movement_speed;
	}
	else
	{
		m_position.z -= m_movement_speed;
	}
	
}

void Camera::keyPressS(bool isShiftPressed)
{
	if (isShiftPressed)
	{
		m_position.y -= m_movement_speed;
	}
	else
	{
		m_position.z += m_movement_speed;
	}
}

void Camera::keyPressA()
{
	m_position.x -= m_movement_speed;
}

void Camera::keyPressD()
{
	m_position.x += m_movement_speed;
}

void Camera::keyPressQ()
{
	m_roll += m_movement_speed*10;
}

void Camera::keyPressE()
{
	m_roll -= m_movement_speed*10;
}

void Camera::keyPressR()
{
	m_yaw += m_movement_speed * 10;
}

void Camera::keyPressF()
{
	m_yaw -= m_movement_speed * 10;
}

void Camera::keyPressT()
{
	m_pitch += m_movement_speed * 10;
}

void Camera::keyPressG()
{
	m_pitch -= m_movement_speed * 10;
}

glm::vec3 Camera::getPosition()
{
	return m_position;
}

float Camera::getPitch()
{
	return m_pitch;
}

float Camera::getYaw()
{
	return m_yaw;
}

float Camera::getRoll()
{
	return m_roll;
}

Camera::Camera()
{
}

void Camera::calculate_Zoom()
{

}


glm::mat4 MatrixMath::create_Transformation_Matrix(glm::vec3 translation, float rot_X, float rot_Y, float rot_Z, float scale)
{
	glm::mat4 matrix(1.0f);
	glm::vec3 xAxis(1.0f, 0.0f, 0.0f);
	glm::vec3 yAxis(0.0f, 1.0f, 0.0f);
	glm::vec3 zAxis(0.0f, 0.0f, 1.0f);
	glm::vec3 scaleAll(scale, scale, scale);
	matrix = glm::translate(matrix, translation);
	matrix = glm::rotate(matrix, rot_X, xAxis);
	matrix = glm::rotate(matrix, rot_Y, yAxis);
	matrix = glm::rotate(matrix, rot_Z, zAxis);
	matrix = glm::scale(matrix, scaleAll);
	return matrix;
}

glm::mat4 MatrixMath::create_View_Matrix(Camera camera)
{
	glm::mat4 viewMatrix(1.0f);
	// height of camera
	viewMatrix = glm::rotate(viewMatrix, (float)glm::radians(camera.getPitch()), glm::vec3(1.0f, 0.0f, 0.0f));
	// left/right of camera
	viewMatrix = glm::rotate(viewMatrix, (float)glm::radians(camera.getYaw()), glm::vec3(0.0f, 1.0f, 0.0f));
	// tilt of camera
	viewMatrix = glm::rotate(viewMatrix, (float)glm::radians(camera.getRoll()), glm::vec3(0.0f, 0.0f, 1.0f));

	glm::vec3 cameraPos = camera.getPosition();
	glm::vec3 negativeCameraPos(-cameraPos.x, -cameraPos.y, -cameraPos.z);
	viewMatrix = glm::translate(viewMatrix, negativeCameraPos);

	return viewMatrix;
}

void Shader::load_Float(int location, float value)
{
	glUniform1f(location, value);
}

void Shader::load_Vector(int location, glm::vec3 vector)
{
	glUniform3f(location, vector.x, vector.y, vector.z);
}

void Shader::load_Bool(int location, bool value)
{
	float toLoad = 0;
	if (value)
	{
		toLoad = 1;
	}
	glUniform1f(location, toLoad);
}

void Shader::load_Matrix(int location, glm::mat4 matrix)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

Shader::Shader(const std::string& Vertex_Shader_File_Path,const std::string& Fragment_Shader_File_Path)
{
	std::string vertexShaderContent;
	std::string fragmentShaderContent;

	vertexShaderContent = importer.importTextFile(Vertex_Shader_File_Path);
	fragmentShaderContent = importer.importTextFile(Fragment_Shader_File_Path);

	m_Vertex_Shader_ID = load_Shader(vertexShaderContent, GL_VERTEX_SHADER);
	m_Fragment_Shader_ID = load_Shader(fragmentShaderContent, GL_FRAGMENT_SHADER);

	m_Program_ID = glCreateProgram();
	glAttachShader(m_Program_ID, m_Vertex_Shader_ID);
	glAttachShader(m_Program_ID, m_Fragment_Shader_ID);
	std::cout << "Program with ID: \"" << m_Program_ID << "\" created and attached VertexShader with ID : \"" << m_Vertex_Shader_ID << "\" and Fragment Shader with ID : \"" << m_Fragment_Shader_ID << "\" to it.\n";
}

void Shader::start()
{
	glUseProgram(m_Program_ID);
}

void Shader::stop()
{
	glUseProgram(0);
}

void Shader::clean_Up()
{
	stop();
	glDetachShader(m_Program_ID, m_Vertex_Shader_ID);
	glDetachShader(m_Program_ID, m_Fragment_Shader_ID);
	glDeleteShader(m_Vertex_Shader_ID);
	glDeleteShader(m_Fragment_Shader_ID);
	glDeleteProgram(m_Program_ID);
}

int Shader::load_Shader(const std::string& file, int type)
{
	int success;
	char infoLog[512];
	const char* source = file.c_str();
	unsigned int shaderID = glCreateShader(type);
	glShaderSource(shaderID, 1, &source, NULL);
	glCompileShader(shaderID);
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;

		std::cout << "\n\nSourcecode of File:\n" << source << "\n";
	}
	else
	{
		std::cout << "Shader: " << shaderID << " successfully loaded...\n";
	}

	return shaderID;
}

void Shader::bind_Attribute(int attribute, const char* variableName)
{
	glBindAttribLocation(m_Program_ID, attribute, variableName);
	std::cout << "Bound attribute (" << attribute << ", \"" << variableName << "\") to Shader_Program with ID: \"" << m_Program_ID << "\n";
}

void StaticShader::load_Transformation_Matrix(glm::mat4 transformationMatrix)
{
	Shader::load_Matrix(m_Location_Transformation_Matrix, transformationMatrix);
}

void StaticShader::load_Projection_Matrix(glm::mat4 m_Projection_Matrix)
{
	Shader::load_Matrix(m_Location_Projection_Matrix, m_Projection_Matrix);
}

void StaticShader::load_View_Matrix(Camera camera)
{
	glm::mat4 viewMatrix = m_Matrix_Math.create_View_Matrix(camera);
	load_Matrix(m_Location_View_Matrix, viewMatrix);
}

void StaticShader::load_Light(Light light)
{
	load_Vector(m_Location_Light_Position, light.get_Position());
	load_Vector(m_Location_Light_Color, light.get_Color());
}

void StaticShader::load_Shine_Var(float damper, float reflectivity)
{
	load_Float(m_Location_shineDamper, damper);
	load_Float(m_Location_Reflectivity, reflectivity);
}

void StaticShader::load_useFakeLighting(bool useFakeLighting)
{
	load_Bool(m_Location_useFakeLighting, useFakeLighting);
}

void StaticShader::load_skyColor(float r, float g, float b)
{
	load_Vector(m_Location_skyColor, glm::vec3(r, g, b));
}

void StaticShader::bind_All_Attributes()
{
	Shader::bind_Attribute(0, "position");
	Shader::bind_Attribute(1, "textureCoords");
	Shader::bind_Attribute(2, "normal");
}

StaticShader::StaticShader()
	:Shader("shader/VertexShader.txt", "shader/FragmentShader.txt")
{
	int success;
	char infoLog[512];
	bind_All_Attributes();
	glLinkProgram(m_Program_ID);
	glGetProgramiv(m_Program_ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_Program_ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	else
	{
		std::cout << "Shader program successfully linked...\n";
	}
	glValidateProgram(m_Program_ID);
	get_All_Uniform_Location();
}

int StaticShader::get_Uniform_Location(const char* uniformName)
{
	std::cout << "Bound Matrix \"" << uniformName << "\" to Shader_Program with ID \"" << m_Program_ID << "\".\n";
	return glGetUniformLocation(m_Program_ID, uniformName);
	
}

void StaticShader::get_All_Uniform_Location()
{
	m_Location_Transformation_Matrix = get_Uniform_Location("transformationMatrix");
	m_Location_Projection_Matrix = get_Uniform_Location("projectionMatrix");
	m_Location_View_Matrix = get_Uniform_Location("viewMatrix");
	m_Location_Light_Position = get_Uniform_Location("lightPosition");
	m_Location_Light_Color = get_Uniform_Location("lightColor");
	m_Location_shineDamper = get_Uniform_Location("shineDamper");
	m_Location_Reflectivity = get_Uniform_Location("reflectivity");
	m_Location_skyColor = get_Uniform_Location("skyColor");
}

Light::Light(glm::vec3 position, glm::vec3 color)
	:m_Position(position), m_Color(color)
{
}

glm::vec3 Light::get_Position()
{
	return m_Position;
}

glm::vec3 Light::get_Color()
{
	return m_Color;
}

void Light::set_Position(glm::vec3 position)
{
	m_Position = position;
}

void Light::set_Color(glm::vec3 color)
{
	m_Color = color;
}

void Terrain_Shader::load_Transformation_Matrix(glm::mat4 transformationMatrix)
{
	Shader::load_Matrix(m_Location_Transformation_Matrix, transformationMatrix);
}

void Terrain_Shader::load_Projection_Matrix(glm::mat4 m_Projection_Matrix)
{
	Shader::load_Matrix(m_Location_Projection_Matrix, m_Projection_Matrix);
}

void Terrain_Shader::load_View_Matrix(Camera camera)
{
	glm::mat4 viewMatrix = m_Matrix_Math.create_View_Matrix(camera);
	load_Matrix(m_Location_View_Matrix, viewMatrix);
}

void Terrain_Shader::load_Light(Light light)
{
	load_Vector(m_Location_Light_Position, light.get_Position());
	load_Vector(m_Location_Light_Color, light.get_Color());
}

void Terrain_Shader::load_Shine_Var(float damper, float reflectivity)
{
	load_Float(m_Location_shineDamper, damper);
	load_Float(m_Location_Reflectivity, reflectivity);
}

void Terrain_Shader::load_skyColor(float r, float g, float b)
{
	load_Vector(m_Location_skyColor, glm::vec3(r, g, b));
}

void Terrain_Shader::bind_All_Attributes()
{
	Shader::bind_Attribute(0, "position");
	Shader::bind_Attribute(1, "textureCoords");
	Shader::bind_Attribute(2, "normal");
}

Terrain_Shader::Terrain_Shader()
	:Shader("shader/TerrainVertexShader.txt", "shader/TerrainFragmentShader.txt")
{
	int success;
	char infoLog[512];
	bind_All_Attributes();
	glLinkProgram(m_Program_ID);
	glGetProgramiv(m_Program_ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_Program_ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED FOR TERRAIN\n" << infoLog << std::endl;
	}
	else
	{
		std::cout << "Terrain Shader program successfully linked...\n";
	}
	glValidateProgram(m_Program_ID);
	get_All_Uniform_Location();
}

int Terrain_Shader::get_Uniform_Location(const char* uniformName)
{
	std::cout << "Bound Matrix \"" << uniformName << "\" to Terrain_Shader_Program with ID \"" << m_Program_ID << "\".\n";
	return glGetUniformLocation(m_Program_ID, uniformName);

}

void Terrain_Shader::get_All_Uniform_Location()
{
	m_Location_Transformation_Matrix = get_Uniform_Location("transformationMatrix");
	m_Location_Projection_Matrix = get_Uniform_Location("projectionMatrix");
	m_Location_View_Matrix = get_Uniform_Location("viewMatrix");
	m_Location_Light_Position = get_Uniform_Location("lightPosition");
	m_Location_Light_Color = get_Uniform_Location("lightColor");
	m_Location_shineDamper = get_Uniform_Location("shineDamper");
	m_Location_Reflectivity = get_Uniform_Location("reflectivity");
	m_Location_useFakeLighting = get_Uniform_Location("useFakeLighting");
	m_Location_skyColor = get_Uniform_Location("skyColor");
}

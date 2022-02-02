#pragma once
//fixed width integer types import
#include <cstdint>

//Math-Functions and Matrix/Vector import
#include <glm/glm/glm.hpp>

//lists
#include <vector>

//strings
#include <string>


//Raw Model Data
class Model_Raw
{
public:
	uint32_t get_VAO_ID();
	uint32_t get_VertexCount();
	Model_Raw(uint32_t vaoID, uint32_t vertexCount);

private:
	uint32_t m_VAO_ID, m_vertexCount;
};

//Texture for Model (image)
class Texture
{
public:
	int get_Texture_ID();
	Texture(int texture_ID);

	float get_Shine_Damper();
	float get_Reflectivity();
	bool get_hasTransparency();
	bool get_useFakeLighting();
	void set_hasTransparency(bool transparency);
	void set_Shine_Damper(float shine_Damper);
	void set_Reflectivity(float reflectivity);
	void set_useFakeLighting(bool useFakeLighting);

private:
	int m_texture_ID;
	float m_Shine_Damper = 1;
	float m_Reflectivity = 0;
	bool m_hasTransparency = 0;
	bool m_useFakeLighting = 0;
};

//Textured Model Data
class Model_Textured
{
public: 
	Model_Raw get_Model_Raw();
	Texture get_Texture();
	Model_Textured(Model_Raw model, Texture texture);

private:
	Model_Raw m_Model_Raw;
	Texture m_Texture;
};

//Entity of an Model for different projections etc.
class Entity
{
public:
	void increase_Position(float dx, float dy, float dz);
	void increase_Rotation(float dx, float dy, float dz);
	void print_Entity_Position();

	//getter & setter
	Model_Textured get_Textured_Model();
	glm::vec3 get_Position();
	float get_Position_X();
	float get_Position_Y();
	float get_Position_Z();
	float get_Rot_X();
	float get_Rot_Y();
	float get_Rot_Z();
	float get_Scale();

	void set_Model_Textured(Model_Textured model);
	void set_Position(glm::vec3 position);
	void set_Rot_X(float rotX);
	void set_Rot_Y(float rotY);
	void set_Rot_Z(float rotZ);
	void set_Scale(float scale);

	Entity(Model_Textured model, glm::vec3 position, float rot_X, float rot_Y, float rot_Z, float scale);

protected:
	Model_Textured m_Model_Textured;
	glm::vec3 m_Position;
	float m_Rot_X, m_Rot_Y, m_Rot_Z;
	float m_Scale;
};

class Loader
{
public:
	void cleanUp();
	Model_Raw load_To_VAO(const float* positions, uint32_t nPositions, const float* textureCoords, uint32_t nTextureCoords, const int* indices, uint32_t nIndices, const float* normals, uint32_t nNormals);
	uint32_t load_Texture(std::string filePath);

private:
	unsigned int create_VAO();
	void store_Data_In_Attribute_List(int32_t attributeNumber, uint32_t coordinateSize, const float* data, uint32_t nData);
	void unbind_VAO();
	void bind_Indices_Buffer(const int* indices, uint32_t nIndices);

private:
	std::vector<unsigned int> vaos;
	std::vector<unsigned int> vbos;
	std::vector<unsigned int> textures;
};

class Object_Importer
{
public:
	static Model_Raw load_Object_Model(std::string filePath);
};

class Terrain
{
public:
	Terrain(int gridX, int gridZ, Loader loader, Texture texture_Terrain);

	Model_Raw generateTerrain(Loader loader);

	Model_Raw get_Model_Raw_Terrain();
	Texture get_Texture_Raw_terrain();
	float get_X();
	float get_Z();

private:
	float m_SIZE = 800.0f;
	int m_VERTEX_COUNT = 128;

	float m_X, m_Z;
	Model_Raw m_Model_Raw_Terrain_Mesh;
	Texture m_Texture_Terrain;
};

class Player : public Entity
{
public:
	Player(Model_Textured model, glm::vec3 position, float rotX, float rotY, float rotZ, float scale);
	void move(uint64_t Frame_Time);

	//will not stop moving after pressing button
	void move_UP(bool isPressed);
	void move_Down(bool isPressed);
	void move_Left(bool isPressed);
	void move_Right(bool isPressed);
	void move_Jump(bool isPressed);
private:

private:
	float m_Run_Speed = 0.2f;
	float m_Turn_Speed = 0.2f;

	float m_Gravity = -0.2f;
	float m_Jump_Power = 1.0f;
	bool is_In_Air = false;

	float m_Current_Speed = 0;
	float m_Current_Turn_Speed = 0;
	float m_UpwardSpeed = 0;

};
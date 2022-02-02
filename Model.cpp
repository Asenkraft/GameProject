#include "Model.h"
#include "Utility.h"
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/gtc/matrix_transform.hpp>



void Entity::increase_Position(float dx, float dy, float dz)
{
	Entity::m_Position.x += dx;
	Entity::m_Position.y += dy;
	Entity::m_Position.z += dz;
}

void Entity::increase_Rotation(float dx, float dy, float dz)
{
	Entity::m_Rot_X += dx;
	Entity::m_Rot_Y += dy;
	Entity::m_Rot_Z += dz;
}

void Entity::print_Entity_Position()
{
	std::cout << "Entity(X|Y|Z): (" << m_Position.x << "|" << m_Position.y << "|" << m_Position.z << ")\n";
}

Model_Textured Entity::get_Textured_Model()
{
	return Entity::m_Model_Textured;
}

glm::vec3 Entity::get_Position()
{
	return Entity::m_Position;
}

float Entity::get_Position_X()
{
	return Entity::m_Position.x;
}

float Entity::get_Position_Y()
{
	return Entity::m_Position.y;
}

float Entity::get_Position_Z()
{
	return Entity::m_Position.z;
}

float Entity::get_Rot_X()
{
	return m_Rot_X;
}

float Entity::get_Rot_Y()
{
	return Entity::m_Rot_Y;
}

float Entity::get_Rot_Z()
{
	return Entity::m_Rot_Z;
}

float Entity::get_Scale()
{
	return Entity::m_Scale;
}

void Entity::set_Model_Textured(Model_Textured model)
{
	Entity::m_Model_Textured = model;
}

void Entity::set_Position(glm::vec3 position)
{
	Entity::m_Position = position;
}

void Entity::set_Rot_X(float rotX)
{
	Entity::m_Rot_X = rotX;
}

void Entity::set_Rot_Y(float rotY)
{
	Entity::m_Rot_Y = rotY;
}

void Entity::set_Rot_Z(float rotZ)
{
	Entity::m_Rot_Z = rotZ;
}

void Entity::set_Scale(float scale)
{
	Entity::m_Scale = scale;
}

Entity::Entity(Model_Textured model, glm::vec3 position, float rot_X, float rot_Y, float rot_Z, float scale)
	:m_Model_Textured(model), m_Position(position), m_Rot_X(rot_X), m_Rot_Y(rot_Y), m_Rot_Z(rot_Z), m_Scale(scale)
{
}

Model_Raw Model_Textured::get_Model_Raw()
{
	return m_Model_Raw;
}

Texture Model_Textured::get_Texture()
{
	return m_Texture;
}

Model_Textured::Model_Textured(Model_Raw model, Texture texture)
	: m_Model_Raw(model), m_Texture(texture)
{
}

int Texture::get_Texture_ID()
{
	return m_texture_ID;
}

Texture::Texture(int texture_ID)
	:m_texture_ID(texture_ID)
{
}

float Texture::get_Shine_Damper()
{
	return m_Shine_Damper;
}

float Texture::get_Reflectivity()
{
	return m_Reflectivity;
}

bool Texture::get_hasTransparency()
{
	return m_hasTransparency;
}

bool Texture::get_useFakeLighting()
{
	return m_useFakeLighting;
}

void Texture::set_hasTransparency(bool transparency)
{
	m_hasTransparency = transparency;
}

void Texture::set_Shine_Damper(float shine_Damper)
{
	m_Shine_Damper = shine_Damper;
}

void Texture::set_Reflectivity(float reflectivity)
{
	m_Reflectivity = reflectivity;
}

void Texture::set_useFakeLighting(bool useFakeLighting)
{
	m_useFakeLighting = useFakeLighting;
}

uint32_t Model_Raw::get_VAO_ID()
{
	return m_VAO_ID;
}

uint32_t Model_Raw::get_VertexCount()
{
	return m_vertexCount;
}

Model_Raw::Model_Raw(uint32_t vaoID, uint32_t vertexCount)
	: m_VAO_ID(vaoID), m_vertexCount(vertexCount)
{
}

void Loader::cleanUp()
{
	std::cout << "CleanUp call. Freeing buffer.\n";
	for (size_t i = 0; i < vaos.size(); i++)
	{
		std::cout << "VAOS[" << i+1 << "/" << vaos.size() << ", (" << vaos[i] << ")] has been emptied.\n";;
		glDeleteVertexArrays(1, &vaos[i]);
	}
	for (size_t i = 0; i < vbos.size(); i++)
	{
		std::cout << "VBOS[" << i + 1 << "/" << vbos.size() << ", (" << vbos[i] << ")] has been emptied.\n";
		glDeleteBuffers(1, &vbos[i]);
		
	}
	for (size_t i = 0; i < textures.size(); i++)
	{
		std::cout << "Textures[" << i + 1 << "/" << textures.size() << ", (" << textures[i] << ")] has been emptied.\n";
		glDeleteBuffers(1, &textures[i]);
	}
}

Model_Raw Loader::load_To_VAO(const float* positions, uint32_t nPositions, const float* textureCoords, uint32_t nTextureCoords, const int* indices, uint32_t nIndices, const float* normals, uint32_t nNormals)
{
	uint32_t vaoID = create_VAO();
	bind_Indices_Buffer(indices, nIndices);
	//3D Vector (model)
	store_Data_In_Attribute_List(0, 3, positions, nPositions);
	//2D Vector (texture)
	store_Data_In_Attribute_List(1, 2, textureCoords, nTextureCoords);
	//3D Vector (normals)
	store_Data_In_Attribute_List(2, 3, normals, nNormals);
	unbind_VAO();
	return Model_Raw(vaoID, nIndices);
}

uint32_t Loader::load_Texture(std::string filePath)
{
	Importer importer;

	uint32_t texture_ID = importer.importTexture(filePath);

	textures.push_back(texture_ID);
	std::cout << "Texture added. Textures (list) has " << textures.size() << " element(s). Texture with ID: \"" << texture_ID << "\" has been pushed back.\n";
	return texture_ID;
}

unsigned int Loader::create_VAO()
{
	unsigned int vaoID;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	vaos.push_back(vaoID);
	std::cout << "VAO created. VAOS (list) has " << vaos.size() << " element(s). VAO with ID: \"" << vaoID << "\" has been pushed back.\n";
	return vaoID;
}

void Loader::store_Data_In_Attribute_List(int32_t attributeNumber, uint32_t coordinateSize, const float* data, uint32_t nData)
{
	std::cout << "\nStore Data In Attribute List:\n";
	uint32_t vboID;
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * nData, data, GL_STATIC_DRAW);
	glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	vbos.push_back(vboID);
	std::cout << "VBO created. VBOS (list) has " << vbos.size() << " element(s). VBO with ID \"" << vboID << "\" has been pushed back.\n";
	std::cout << coordinateSize << "D Vector has been bound to the buffer with the attribute number "<< attributeNumber <<"\n";
	std::cout << nData << " Datapoints have been bound to the buffer with a size of " << sizeof(float) * nData << " bytes.<<\n";
}

void Loader::unbind_VAO()
{
	glBindVertexArray(0);
}

void Loader::bind_Indices_Buffer(const int* indices, uint32_t nIndices)
{
	std::cout << "\nBind Indice Buffer:\n";
	unsigned int vboID;
	glGenBuffers(1, &vboID);
	vbos.push_back(vboID);
	std::cout << "VBO created. VBOS (list) has " << vbos.size() << " element(s). VBO with ID \"" << vboID << "\" has been pushed back.\n";
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * nIndices, indices, GL_STATIC_DRAW);
	std::cout << nIndices << " Indices have been bound to the buffer with a size of " << sizeof(unsigned int) * nIndices << " bytes.\n";
}

 Model_Raw Object_Importer::load_Object_Model(std::string filePath)
{
	 bool wantPrint = false;

	Importer importer;
	std::string obj_data = importer.importTextFile(filePath);
	std::string substring;

	std::vector<float> vertices_Array;
	std::vector<float>  normals_Array;
	std::vector<float>  texture_Array;
	std::vector<uint32_t>  faces_Array;
	

		std::cout << "parsing took: ";
		Timer timer;
		//Parsing through obj_data and converting string to float/int and saving it in the fitting vector
		//Iterate through the object data string
		for (int i = 0; i < obj_data.size(); i++)
		{
			//when "v " is found copy the string afterwards until the end of the line into substring
			if (obj_data[i] == 'v' && obj_data[i + 1] == ' ')
			{
				i = i + 2;
				std::size_t pos = obj_data.find('\n', i);
				substring = obj_data.substr(i, (pos - i));

				//look through the substring for a space ' '
				//when you find it convert the substring of substring from j until the space was found into a float and save it in the vertices array
				//set j to where the space ' ' was found and look for the next space until the end of the substring. 
				//j is the offset for further substrings
				for (int j = 0; j < substring.size(); j++)
				{
					std::size_t pos2 = substring.find(' ', j);
					vertices_Array.push_back(std::stof(substring.substr(j, (pos2 - j)))); 
					//set j to the position of the space ' '
					j = pos2;
					//if no space was found until the end of the line positition is set to 18446744073709551615	which is (2^64 -1) -> 
					//all bits are set: 1111111111111111111111111111111111111111111111111111111111111111
					//EXPLANATION
					//the first bit is the deceides if the integer is signed or unsigned. Unsigned integers have 0, singed ones 1.
					//since c++ uses 2's complenets (TCP) instead of 1's complement (OCP) the following is happening
					//The Compiler sets the bits to 1111111111111111111111111111111111111111111111111111111111111111 which is equal to the number above.
					//first the OCP is happening,  this flips all the bits
					// 1111111111111111111111111111111111111111111111111111111111111111 is converted to 00000000000000000000000000000000000000000000000000000000000000000
					//to make it TCP +1 is added to the binary number making it 
					// 00000000000000000000000000000000000000000000000000000000000000001 which is "-1"
					//so this happens because size_t is an uint64_t and converted to an int, I think.
					if (j == -1) j = substring.size() + 1;

				}
				//set i to pos so we don't look through the same strings again
				i = pos;
			}

			//when "vt " is found copy the string afterwards until the end of the line into substring
			if (obj_data[i] == 'v' && obj_data[i + 1] == 't' && obj_data[i + 2] == ' ')
			{
				i = i + 3;
				std::size_t pos = obj_data.find('\n', i);
				substring = obj_data.substr(i, (pos - i));

				//same as above
				for (int j = 0; j < substring.size(); j++)
				{
					std::size_t pos2 = substring.find(' ', j);
					texture_Array.push_back(std::stof(substring.substr(j, (pos2 - j))));
					if (pos2 > substring.size()) j = substring.size() + 1;
					else j = pos2;
				}
				i = pos;
			}

			//when "vn " is found copy the string afterwards until the end of the line into substring
			if (obj_data[i] == 'v' && obj_data[i + 1] == 'n' && obj_data[i + 2] == ' ')
			{
				i = i + 3;
				std::size_t pos = obj_data.find('\n', i);
				substring = obj_data.substr(i, (pos - i));

				for (int j = 0; j < substring.size(); j++)
				{
					std::size_t pos2 = substring.find(' ', j);
					normals_Array.push_back(std::stof(substring.substr(j, (pos2 - j))));
					if (pos2 > substring.size()) j = substring.size() + 1;
					else j = pos2;
				}
				i = pos;
			}

			//when "f " is found copy the string afterwards until the end of the line into substring
			if (obj_data[i] == 'f' && obj_data[i + 1] == ' ')
			{
				i = i + 2;
				std::size_t pos = obj_data.find('\n', i);
				substring = obj_data.substr(i, (pos - i));


				uint8_t counter = 1;
				for (int k = 0; k < substring.size(); k++)
				{
					std::size_t pos3;
					if (counter < 3)
					{
						pos3 = substring.find('/', k);
					}
					else
					{
						pos3 = substring.find(' ', k);
						counter = 0;
					}
					counter++;
					faces_Array.push_back(std::atoi(substring.substr(k, (pos3 - k)).c_str()));
					if (pos3 > substring.size()) k = substring.size() + 1;
					else k = pos3;
				}
				i = pos;
			}
			//if the end of the string is reached without finding another search string like "v " or "f ", set i to the size of the obj_data string+1 to end the for loop.
			if (i == -1) i = obj_data.size() + 1;
		}
		timer.stop();

		uint32_t nVertices, nIndices, nTextureCoords, nNormals, nFaces;

		nIndices = faces_Array.size() / 3;
		nFaces = faces_Array.size();

		nVertices = nFaces;
		nTextureCoords = nFaces;
		nNormals = nFaces;


		float* vertices = new float[nVertices];
		float* textures = new float[nTextureCoords];
		float* normals = new float[nNormals];
		int* indices = new int[nIndices];
		int8_t counterToThree = 0;
		int32_t counterForArrayPositionVertices = 0;
		int32_t counterForArrayPositionTextures = 0;
		int32_t counterForArrayPositionNormals = 0;
		int32_t counterForArrayPositionIndices = 0;


		//Models consist of triangles which consist of the datapoints of vertices. They also have normal and texture datapoints which are for texturing and lighting
		//The Faces e.g. 2/1/1, 3/1/1, 1/1/1 describe the data used in one triangle
		//The first value is the vertice, the second the texture and the third the normal so it is (v/t/n)
		//Each value represents 3 data points for each categorie.
		//So the first face above the first number "2" stands for the three coordinates of second vertice e.g. (0.5, -0.5, -0.5)
		//This for loop goes through the array of faces and and constructs an vector (list) of the associated vertices, texturecoordinates and normalvectors.
		//e.g. with the faces above the array would continue the values
		//[0] = 2; [1] = 1; [2] = 1; [3] = 3; [4] = 1; [5] = 1; [6] = 1; [7] = 1; [8] = 1;
		//The first value of a face is always the vector, the second is always the texture and the third is always the normal
		//So at the start we would find in the faces_Array[0] the value 2 for the vertice_Array.
		//Since Vertices have always three data points we multiply the value 2 by 3 to get 6.
		//Now we want the 4th, 5th and 6th value to construct our first vertice -> 6-3 = 3 (fourth value of the array since we start with 0), 6-2 = 4 (fifth value), 6-1 (fifth value)
		//We save the data in the array of vertices as the first three values and have therefore our first three coordinates for the triangle.

		for (int i = 0; i < nFaces; i++)
		{
			counterToThree++;
			switch (counterToThree)
			{
			case 1:
			{
				vertices[counterForArrayPositionVertices++] = vertices_Array[faces_Array[i] * 3 - 3];
				vertices[counterForArrayPositionVertices++] = vertices_Array[faces_Array[i] * 3 - 2];
				vertices[counterForArrayPositionVertices++] = vertices_Array[faces_Array[i] * 3 - 1];
				break;
			}
			case 2:
			{
				textures[counterForArrayPositionTextures++] = texture_Array[faces_Array[i] * 2 - 2];
				textures[counterForArrayPositionTextures++] = texture_Array[faces_Array[i] * 2 - 1];
				break;
			}
			case 3:
			{
				normals[counterForArrayPositionNormals++] = normals_Array[faces_Array[i] * 3 - 3];
				normals[counterForArrayPositionNormals++] = normals_Array[faces_Array[i] * 3 - 2];
				normals[counterForArrayPositionNormals++] = normals_Array[faces_Array[i] * 3 - 1];
				indices[counterForArrayPositionIndices++] = counterForArrayPositionIndices;
				counterToThree = 0;
				if (i == 107)
				{
					std::cout << "107\n";
				}
				break;
			}
			}
		}

		// just a small for loop to print out all the data in the vertices, textures and normals array to compare to the obj file and see if everything was done correctly.
		if (wantPrint)
		{
			int counter = 0;
			int counter2 = 0;
			for (int i = 0; i < nIndices * 3; i++)
			{
				counter++;
				if (counter == 1)
				{
					std::cout << "\nV" << counter2 << " = (" << vertices[i];
				}
				if (counter == 2)
				{
					std::cout << "|" << vertices[i];
				}
				if (counter == 3)
				{
					std::cout << "|" << vertices[i] << ")";
					counter = 0;
					counter2++;
				}
			
			}
			std::cout << "\n\n";
			counter = 0;
			counter2 = 0;
			for (int i = 0; i < nIndices * 2; i++)
			{
				counter++;
				if (counter == 1)
				{
					std::cout << "\nT" << counter2 << " = (" << textures[i];
				}
				if (counter == 2)
				{
					std::cout << "|" << textures[i] << ")";
					counter = 0;
					counter2++;
				}
			}
			std::cout << "\n\n";
			counter = 0;
			counter2 = 0;
			for (int i = 0; i < nIndices * 3; i++)
			{
				counter++;
				if (counter == 1)
				{
					std::cout << "\nN" << counter2 << " = (" << normals[i];
				}
				if (counter == 2)
				{
					std::cout << "|" << normals[i];
				}
				if (counter == 3)
				{
					std::cout << "|" << normals[i] << ")";
					counter = 0;
					counter2++;
				}
			}

			std::cout << "\n\n";
			counter = 0;
			counter2 = 0;
			for (int i = 0; i < nIndices; i++)
			{
				counter++;
				if (counter == 1)
				{
					std::cout << "\nI" << counter2 << " = (" << indices[i];
				}
				if (counter == 2)
				{
					std::cout << "|" << indices[i];
				}
				if (counter == 3)
				{
					std::cout << "|" << indices[i] << ")";
					counter = 0;
					counter2++;
				}
			}
			std::cout << "\n\n";

		}
		Loader loader;

		nVertices = nIndices * 3;
		nTextureCoords = nIndices * 2;
		nNormals = nIndices * 3;

		Model_Raw rawModel(loader.load_To_VAO(vertices, nVertices, textures, nTextureCoords, indices, nIndices, normals, nNormals));

		delete[] vertices;
		delete[] textures;
		delete[] indices;
		return rawModel;
}

Terrain::Terrain(int gridX, int gridZ, Loader loader, Texture texture_Terrain)
	:m_X(gridX*m_SIZE), m_Z(gridZ*m_SIZE), m_Texture_Terrain(texture_Terrain), m_Model_Raw_Terrain_Mesh(generateTerrain(loader))
{
	m_Texture_Terrain.set_Reflectivity(1);
	m_Texture_Terrain.set_Shine_Damper(10);
}

#include <glm/glm/gtc/noise.hpp>
Model_Raw Terrain::generateTerrain(Loader loader)
{
	int count = m_VERTEX_COUNT * m_VERTEX_COUNT;
	int nVertices = count * 3;
	int nNormals = count * 3;
	int nTextureCoords = count * 2;
	int nIndices = 6 * (m_VERTEX_COUNT - 1) * (m_VERTEX_COUNT - 1);

	float* vertices = new float[nVertices];
	float* normals = new float[nNormals];
	float* textureCoords = new float[nTextureCoords];
	int* indices = new int[nIndices];

	int vertexPointer = 0;
	for (uint32_t i = 0; i < m_VERTEX_COUNT; i++)
	{
		for (uint32_t j = 0; j < m_VERTEX_COUNT; j++)
		{
			//X
			vertices[vertexPointer * 3] = (float)j / ((float)m_VERTEX_COUNT - 1) * m_SIZE;
			normals[vertexPointer * 3] = 0;
			textureCoords[vertexPointer * 2] = (float)j / ((float)m_VERTEX_COUNT - 1);
			//Y
			////Height Map through simplex function
			//float height_Point_Value = glm::simplex(glm::vec2((float)j / ((float)m_VERTEX_COUNT - 1) * m_SIZE / 200.0f, (float)i / ((float)m_VERTEX_COUNT - 1) * m_SIZE / 200.0f) );
			////normalize between 0 and 1
			//height_Point_Value = (height_Point_Value + 1) / 2;
			//// more height!
			//height_Point_Value = (height_Point_Value * 50) -50;
			//vertices[vertexPointer * 3 + 1] = height_Point_Value;

			vertices[vertexPointer * 3 + 1] = 0;
			normals[vertexPointer * 3 + 1] = 1;
			textureCoords[vertexPointer * 2 + 1] = (float)i / ((float)m_VERTEX_COUNT - 1);
			//Z
			vertices[vertexPointer * 3 + 2] = (float)i / ((float)m_VERTEX_COUNT - 1) * m_SIZE;
			normals[vertexPointer * 3 + 2] = 0;
			
			vertexPointer++;
		}
	}

	int pointer = 0;
	for (int gz = 0; gz < m_VERTEX_COUNT - 1; gz++)
	{
		for (int gx = 0; gx < m_VERTEX_COUNT - 1; gx++)
		{
			int topLeft = (gz * m_VERTEX_COUNT) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1) * m_VERTEX_COUNT) + gx;
			int bottomRight = bottomLeft + 1;

			indices[pointer++] = topLeft;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = topRight;
			indices[pointer++] = topRight;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = bottomRight;

		}
	}

	Model_Raw temp_Terrain = loader.load_To_VAO(vertices, nVertices, textureCoords, nTextureCoords, indices, nIndices, normals, nNormals);
	delete[] vertices, normals, textureCoords, indices;
	return temp_Terrain;
}

Model_Raw Terrain::get_Model_Raw_Terrain()
{
	return m_Model_Raw_Terrain_Mesh;
}

Texture Terrain::get_Texture_Raw_terrain()
{
	return m_Texture_Terrain;
}

float Terrain::get_X()
{
	return m_X;
}

float Terrain::get_Z()
{
	return m_Z;
}

Player::Player(Model_Textured model, glm::vec3 position, float rotX, float rotY, float rotZ, float scale)
	:Entity(model, position, rotX, rotY, rotZ, scale)
{

}

void Player::move(uint64_t Frame_Time)
{
	increase_Rotation(0, m_Current_Turn_Speed / Frame_Time, 0);
	float distance = m_Current_Speed / Frame_Time;
	float dx = distance * glm::sin(get_Rot_Y());
	float dz = distance * glm::cos(get_Rot_Y());
	increase_Position(dx, 0, dz);

	m_UpwardSpeed += m_Gravity / Frame_Time;
	increase_Position(0, m_UpwardSpeed / Frame_Time, 0);
	//Terrain "Collision"
	if (get_Position_Y() < 0)
	{
		m_UpwardSpeed = 0;
		is_In_Air = false;
	}
}

void Player::move_UP(bool isPressed)
{
	if (isPressed ? m_Current_Speed = m_Run_Speed : m_Current_Speed = 0);
}

void Player::move_Down(bool isPressed)
{
	if (isPressed ? m_Current_Speed = -m_Run_Speed : m_Current_Speed = 0);
}

void Player::move_Left(bool isPressed)
{
	if (isPressed ? m_Current_Turn_Speed = m_Turn_Speed : m_Current_Turn_Speed = 0);
}


void Player::move_Right(bool isPressed)
{
	if (isPressed ? m_Current_Turn_Speed = -m_Turn_Speed : m_Current_Turn_Speed = 0);
}

void Player::move_Jump(bool isPressed)
{
	if (!is_In_Air) 
	{
		m_UpwardSpeed = m_Jump_Power;
		is_In_Air = true;
	}

}


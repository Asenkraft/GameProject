#include "Utility.h"
#include "stb_image.h"
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "lodepng.h"

std::string Importer::importTextFile(const std::string& filePath)
{
	std::ifstream readFile;
	readFile.open(filePath);

	if (!readFile.is_open())
	{
		std:: cout << "unable to open file " << filePath << "\n";
	}
	else
	{
		std::cout << "opened file " << filePath << "\n";
	}
	std::istreambuf_iterator<char> begin(readFile), end;
	std::string shader_file(begin, end);
	readFile.close();
	return shader_file;
	
}

uint32_t Importer::importTexture(const std::string& filePath)
{
	stbi_set_flip_vertically_on_load(true);
	uint32_t texture_ID;
	int32_t width, height, nrChannels;
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

	//std::vector<unsigned char> data2;
	//unsigned error = lodepng::decode(data2, width, height, filePath);
	//unsigned error = lodepng::load_file(data2, filePath);

	//if (error)
	//{
	//	std::cout << "Failed to load texture.\n";
	//	std::cout << "Failed to import texture.\n";
	//	return 0;
	//}
	//else
	//{
	//	std::string s(data2.begin(), data2.end());
	//	std::cout << "loaded Texture at :" << filePath << "\n";
	//	glGenTextures(1, &texture_ID);
	//	glBindTexture(GL_TEXTURE_2D, texture_ID);
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, s.c_str());
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//	std::cout << "Texture imported.\n";
	//	return texture_ID;
	//}

	if (!data)
	{
		std::cout << "Failed to load texture.\n";
		std::cout << "Failed to import texture.\n";
		return 0;
	}
	else
	{
		std::cout << "loaded Texture at :" << filePath << "\n";
		glGenTextures(1, &texture_ID);
		glBindTexture(GL_TEXTURE_2D, texture_ID);
		if (nrChannels == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		
		glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture imported.\n";
		return texture_ID;
	}
}

Timer::Timer()
{
	//Constructor start the timer
	m_StartTimepoint = std::chrono::high_resolution_clock::now();

}

Timer::~Timer()
{

}

void Timer::stop()
{
	//Destructor ends the timer
	std::chrono::time_point <std::chrono::high_resolution_clock> endTimepoint = std::chrono::high_resolution_clock::now();
	m_EndTimepoint = endTimepoint;
	//casts the time of start and end of those timers in microseconds and saves it in long long start and long long end
	long long start = std::chrono::time_point_cast<std::chrono::microseconds> (m_StartTimepoint).time_since_epoch().count();
	long long end = std::chrono::time_point_cast<std::chrono::microseconds> (m_EndTimepoint).time_since_epoch().count();

	//difference of end and start is the duration of the runtime/lifetime
	long long duration = end - start;
	std::cout << duration/1000 << " milliseconds.\n";
	Timer::~Timer();
}

ReturnTimer::ReturnTimer()
{
}

void ReturnTimer::start()
{
	//start the timer
	m_StartTimepoint = std::chrono::high_resolution_clock::now();
}

long long ReturnTimer::stop()
{
	std::chrono::time_point <std::chrono::high_resolution_clock> endTimepoint = std::chrono::high_resolution_clock::now();

	//casts the time of start and end of those timers in milliseconds and saves it in long long start and long long end
	long long start = std::chrono::time_point_cast<std::chrono::milliseconds> (m_StartTimepoint).time_since_epoch().count();
	long long end = std::chrono::time_point_cast<std::chrono::milliseconds> (m_EndTimepoint).time_since_epoch().count();

	//difference of end and start is the duration of the runtime/lifetime
	long long duration = end - start;
	return duration;
}

long long ReturnTimer::measure()
{
	std::chrono::time_point <std::chrono::high_resolution_clock> endTimepoint = std::chrono::high_resolution_clock::now();
	//casts the time of start and end of those timers in milliseconds and saves it in long long start and long long end
	long long start = std::chrono::time_point_cast<std::chrono::milliseconds> (m_StartTimepoint).time_since_epoch().count();
	long long end = std::chrono::time_point_cast<std::chrono::milliseconds> (m_EndTimepoint).time_since_epoch().count();

	//difference of end and start is the duration of the runtime/lifetime
	long long duration = end - start;
	return duration;
}


int32_t Utility::stringToInt(const std::string& string, int start, int end)
{
	int32_t sum = 0;
	bool isNegative = false;

	for (int i = start; i <= end; i++)
	{
		if (string[i] == '-')
		{
			isNegative = true;
		}
		else if (string[i] >= '0' && string[i] <= '9')
		{
			sum = sum + (string[i] - 48) * pow(10, (end - i));
		}
	}
	if (isNegative)
	{
		sum = -sum;
	}
	return sum;
}

float Utility::stringToFloat(const std::string& string, int start, int end)
{
	float sum = 0.0f;
	uint32_t pointPosition = 0;
	bool isNegative = false;

	for (int i = start; i < end; i++)
	{
		if (string[i] == '-')
		{
			isNegative = true;
		}
		else if (string[i] == '.')
		{
			pointPosition = i;
			sum = sum / 10;
		}
		else if (string[i] >= '0' && string[i] <= '9')
		{
			sum = sum + (string[i] - 48) * pow(10, (end - i) - 1);
		}
	}
	if (isNegative)
	{
		sum = sum * (-1);
	}
	sum = sum / pow(10, (end - pointPosition) - 1);
	return sum;
}

void StringParser::parseOBJ(const std::string& string)
{
	uint32_t vertex_Size = 0;
	uint32_t texture_Size = 0;
	uint32_t normal_Size = 0;
	uint32_t faces_Size = 0;

	std::cout << "parsing took: ";
	Timer timer;
	//looking for the size of the following arrays
	for (uint32_t i = 0; i < string.size(); i++)
	{
		if (string[i] == 'v' && string[i + 1] == ' ')
		{
			i = i + 2;
			vertex_Size++;
		}
		else if (string[i] == 'v' && string[i + 1] == 't' && string[i + 2] == ' ')
		{
			i = i + 3;
			texture_Size++;
		}
		else if (string[i] == 'v' && string[i + 1] == 'n' && string[i + 2] == ' ')
		{
			i = i + 3;
			normal_Size++;
		}
		else if (string[i] == 'f' && string[i + 1] == ' ')
		{
			i = i + 2;
			faces_Size++;
		}
	}

	uint32_t* vertex_Lines_Begin = new uint32_t[vertex_Size];
	uint32_t* texture_Lines_Begin = new uint32_t[texture_Size];
	uint32_t* normals_Lines_Begin = new uint32_t[normal_Size];
	uint32_t faces_Lines_Begin = 0;
	uint32_t* faces = new uint32_t[faces_Size * 9];

	uint32_t vertex_Index = 0;
	uint32_t texture_Index = 0;
	uint32_t normal_Index = 0;
	uint32_t face_Index = 0;

	// divide OBJ into vertex lines, texture lines, normal lines and face lines
	for (uint32_t i = 0; i < string.size(); i++)
	{
		if (string[i] == 'v' && string[i + 1] == ' ')
		{
			i = i + 2;
			vertex_Lines_Begin[vertex_Index++] = i;
		}
		else if (string[i] == 'v' && string[i + 1] == 't' && string[i + 2] == ' ')
		{
			i = i + 3;
			texture_Lines_Begin[texture_Index++] = i;
		}
		else if (string[i] == 'v' && string[i + 1] == 'n' && string[i + 2] == ' ')
		{
			i = i + 3;
			normals_Lines_Begin[normal_Index++] = i;
		}

		else if (string[i] == 'f' && string[i + 1] == ' ')
		{
			i = i + 2;
			faces_Lines_Begin = i;
			do 
			{
				if (string[i] != 'f')
				{
					i++;
				}
				else if (string[i] == '\0')
				{
					break;
				}
				if (string[i] == '/' || string[i] == ' ' || string[i] == 'f' || string[i] == '\0' || string[i] == '\n')
				{
					faces[face_Index++] = Utility::stringToInt(string, faces_Lines_Begin, i - 1);

					faces_Lines_Begin = i + 1;
				}
			
			} while (string[i] != '\n');
		}
	}

	bool print = false;
	if (print)
	{
		int counter = 1;
		int counter2 = 1;
		for (int i = 0; i < face_Index; i++)
		{
			std::cout << faces[i] << "/";
			if (counter == 3)
			{
				std::cout << " ";
				counter = 0;
				if (counter2 == 3)
				{
					std::cout << "\n";
					counter2 = 0;
				}
				counter2++;
			}
			counter++;
		}
	}



	//faces contain face directions
	//now we construct the arrays of vertices, textures and normals out of the face data

	//float* vertex = new float[vertex_Size * 3];
	float* vertex = new float[face_Index * 3];
	//float* textures = new float[texture_Size * 2];
	float* textures = new float[face_Index * 2];
	//float* normals = new float[normal_Size * 3];
	float* normals = new float[face_Index * 3];
	uint32_t begin;
	uint32_t end;
	vertex_Index = 0;
	bool endOfLine;


	for (int j = 0; j < face_Index; j = j + 3)
	{
		endOfLine = false;
		begin = vertex_Lines_Begin[faces[j] - 1];
		end = begin;
		while (!endOfLine)
		{
			if (string[end] == ' ' || string[end] == 'v' || string[end] == '\n')
			{
				vertex[vertex_Index] = Utility::stringToFloat(string, begin, end);
				vertex_Index++;
				begin = end;
				if (string[end] == 'v' || string[end] == '\n')
				{
					endOfLine = true;
				}
			}
			end++;
		}

	}

	texture_Index = 0;

	for (int j = 1; j < face_Index; j = j + 3)
	{
		endOfLine = false;
		begin = texture_Lines_Begin[faces[j] - 1];
		end = begin;
		while (!endOfLine)
		{
			if (string[end] == ' ' || string[end] == 'v' || string[end] == '\n')
			{
				textures[texture_Index] = Utility::stringToFloat(string, begin, end);
				texture_Index++;
				begin = end;
				if (string[end] == 'v' || string[end] == '\n')
				{
					endOfLine = true;
				}
			}
			end++;
		}

	}

	normal_Index = 0;

	for (int j = 2; j < face_Index; j = j + 3)
	{
		endOfLine = false;
		begin = normals_Lines_Begin[faces[j] - 1];
		end = begin;
		while (!endOfLine)
		{
			if (string[end] == ' ' || string[end] == 'v' || string[end] == '\n')
			{
				normals[normal_Index] = Utility::stringToFloat(string, begin, end);
				texture_Index++;
				begin = end;
				if (string[end] == 'v' || string[end] == '\n')
				{
					endOfLine = true;
				}
			}
			end++;
		}

	}
	timer.stop();

	delete[] vertex_Lines_Begin;
	delete[] texture_Lines_Begin;
	delete[] normals_Lines_Begin;
	delete[] faces;

}

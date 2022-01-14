#pragma once
#include <iostream>
#include <chrono>
#include <string>

//loads images, text, objects, ...
class Importer
{
//alternative: namespace instead of class and call via Namespace::function, e.g. Importer::importTextFile(string path)

public:
	//loads Text File .txt and returns content as std::string (pointer to text data)
	std::string importTextFile(const std::string& filePath);
	//loads pictures and returns content as unsigned char* (pointer to image data)
	uint32_t importTexture(const std::string& filePath);

public:
	//filepaths
	const char* VERTEX_FILE = "shader/VertexShader.txt";
	const char* FRAGMENT_FILE = "shader/FragmentShader.txt";
	const char* TERRAIN_VERTEX_FILE = "shader/VertexShader.txt";
	const char* TERRAIN_FRAGMENT_FILE = "shader/FragmentShader.txt";

	const char* TEXTURE_FILE = "res/Dragon/dragon.png";
	const char* OBJECT_FILE = "res/Dragon/dragon.obj";
};

//Measures the runtime (scope) or until stop is called of a function and outputs the result in microseconds in the console
class Timer
{
public:
	Timer();
	void stop();
	~Timer();
private:
	std::chrono::time_point <std::chrono::high_resolution_clock> m_StartTimepoint;
	std::chrono::time_point <std::chrono::high_resolution_clock> m_EndTimepoint;
	
};


class StringParser
{


public:
	void parseOBJ(const std::string& string);
};

namespace Utility
{

	int32_t stringToInt(const std::string& string, int start, int end);
	float stringToFloat(const std::string& string, int start, int end);
};

//Measures the runtime from start until stop is called and returns the result in milliseconds as long long
class ReturnTimer
{
public:
	ReturnTimer();
	void start();
	long long stop();
	long long measure();
	
private:
	std::chrono::time_point <std::chrono::high_resolution_clock> m_StartTimepoint;
	std::chrono::time_point <std::chrono::high_resolution_clock> m_EndTimepoint;
};

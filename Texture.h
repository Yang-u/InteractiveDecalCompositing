#pragma once
#include <glad/glad.h>
#include <string>
#include <iostream>

class Texture
{
public:
	Texture()=default;
	~Texture()=default;
public:
	static unsigned int genTexture(const std::string& path, int* width, int* height, int* nrChannels);
	static void saveTexture(int w, int h, int n, unsigned char* data, const std::string& path);
	static void saveTexture(int w, int h, int n, unsigned int texture, const std::string& path);
};


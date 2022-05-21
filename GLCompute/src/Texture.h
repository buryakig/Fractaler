#pragma once
#include "Core.h"

class Texture
{
public:
	Texture() = default;
	~Texture();


	unsigned int id = 0;

	int width = 0;
	int height = 0;
	int nrChannels = 0;
	unsigned int format = GL_RGB;
	unsigned int internal_format = GL_RGB8;

	unsigned char* data = nullptr;
};


#pragma once
#include<string>
#include <fstream>
#include "Shader.h"
#include "ComputeShader.h"
#include "Texture.h"

class Resources
{
public:
	Resources() = delete;

	static Shader* LoadShader(const char* vertexPath, const char* fragmentPath);
	static ComputeShader* LoadComputeShader(const char* csPath);
	static Texture* LoadTexture(const char* imagePath, unsigned int  filterMode = GL_LINEAR);

	static void DeallocateMemory();

	static std::vector<Shader*> shaders;
	static std::vector<ComputeShader*> computeShaders;
	static std::vector<Texture*> textures;
};


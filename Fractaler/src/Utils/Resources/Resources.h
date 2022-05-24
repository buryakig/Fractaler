#pragma once
#include<string>
#include <fstream>
#include <memory>
#include "Shader.h"
#include "ComputeShader.h"
#include "Texture.h"

class Resources
{
public:
	Resources() = delete;

	static Shader* LoadShader(const char* vertexPath, const char* fragmentPath);
	static std::shared_ptr<ComputeShader> LoadComputeShader(const char* csPath);
	static Texture* LoadTexture(const char* imagePath, unsigned int  filterMode = GL_LINEAR);

	static void DeallocateMemory();

	static std::vector<Shader*> shaders;
	static std::vector<std::shared_ptr<ComputeShader>> computeShaders;
	static std::vector<Texture*> textures;
};


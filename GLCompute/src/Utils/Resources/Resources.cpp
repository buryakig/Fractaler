#include "Core.h"
#include "Utils/Resources/Resources.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::vector<Shader*>	Resources::shaders = {};
std::vector<Texture*>	Resources::textures = {};
std::vector<ComputeShader*>	Resources::computeShaders = {};

ComputeShader* Resources::LoadComputeShader(const char* csPath)
{
	std::string csCode;
	std::ifstream csFile;

	// ensure ifstream objects can throw exceptions:
	csFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	
	try
	{
		//open files
		csFile.open(csPath);
		std::stringstream csShaderStream;

		csShaderStream << csFile.rdbuf();

		csFile.close();

		csCode = csShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		std::cout << e.what() << std::endl;
	}

	const char* csShaderCode = csCode.c_str();

	unsigned int compute;
	int success;
	char infoLog[512];


	// vertex shader
	compute = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(compute, 1, &csShaderCode, NULL);
	glCompileShader(compute);
	// print compile errors if any
	glGetShaderiv(compute, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(compute, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int programID = glCreateProgram();
	glAttachShader(programID, compute);
	glLinkProgram(programID);

	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		std::cout << csPath << std::endl;
	}

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(compute);

	computeShaders.push_back(new ComputeShader(programID));

	return computeShaders.back();
}

Shader* Resources::LoadShader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		//open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		std::cout << e.what() << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];


	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int programID = glCreateProgram();
	glAttachShader(programID, vertex);
	glAttachShader(programID, fragment);
	glLinkProgram(programID);

	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		std::cout << vertexPath << std::endl;
		std::cout << fragmentPath << std::endl << std::endl;
	}

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	
	shaders.push_back(new Shader(programID));
	
	return shaders.back();
}

Texture* Resources::LoadTexture(const char* imagePath, unsigned int  filterMode)
{
	Texture* texture = new Texture();

	texture->data = stbi_load(imagePath, &texture->width, &texture->height, &texture->nrChannels, 0);
	if (!texture->data)
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	std::cout << std::string("Loaded texture from ") + imagePath << std::endl;
	std::cout << "texture size: " << texture->width << "x" << texture->height << std::endl;

	glCreateTextures(GL_TEXTURE_2D, 1, &texture->id);

	glTextureParameteri(texture->id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(texture->id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(texture->id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(texture->id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (texture->nrChannels == 1)
	{
		texture->format = GL_RED;
		texture->internal_format = GL_R8;
	}
	else if (texture->nrChannels == 3)
	{
		texture->format = GL_RGB;
		texture->internal_format = GL_RGB8;
	}
	else if (texture->nrChannels == 4)
	{
		texture->format = GL_RGBA;
		texture->internal_format = GL_RGBA8;
	}

	glTextureStorage2D(texture->id, 6, texture->internal_format, texture->width, texture->height);
	glTextureSubImage2D(texture->id, 0, 0, 0, texture->width, texture->height, texture->format, GL_UNSIGNED_BYTE, texture->data);
	
	glGenerateTextureMipmap(texture->id);

	textures.push_back(texture);
	return texture;
}


void Resources::DeallocateMemory()
{
	for (auto shader : shaders)	delete shader;
	for (auto texture : textures)	delete texture;
	for (auto computeShader : computeShaders)	delete computeShader;
}


#pragma once

#include "Core.h"

class ComputeShader
{
public:
	unsigned int programID;

	// constructor reads and builds the shader
	ComputeShader() {};
	ComputeShader(const char* csPath);
	~ComputeShader() {
		glDeleteProgram(programID); 
	}
	// use/activate the shader
	void use() const;

private:
	ComputeShader(unsigned int id) : programID(id) {}

	friend class Resources;
};



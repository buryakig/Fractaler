#include "ComputeShader.h"

void ComputeShader::use() const
{
	glUseProgram(programID);
}

#include "RenderTexture.h"

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

RenderTexture::RenderTexture(int w, int h) 
	: Texture(w, h)
{
	glGenTextures(1, &id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, w, h);
	glBindImageTexture(0, id, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}

RenderTexture::RenderTexture(int w, int h, unsigned int format)
	: Texture(w, h)
{

	glGenTextures(1, &id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexStorage2D(GL_TEXTURE_2D, 1, format, w, h);
	glBindImageTexture(0, id, 0, GL_FALSE, 0, GL_READ_WRITE, format);
}

RenderTexture::~RenderTexture()
{
	glDeleteTextures(1, &id);
}

#pragma once
#include "Core.h"
#include "Texture.h"

class RenderTexture : public Texture
{
public:
	RenderTexture() = default;
	RenderTexture(int w, int h);
	RenderTexture(int w, int h, unsigned int format);
	~RenderTexture();
};


#pragma once

#include <gl/GL.h>
#include <gl/GLU.h>

#include "DImage.h"

class CGLTexture
{
private:
	unsigned int m_ID;
public:
	CGLTexture();
	~CGLTexture();

	void LoadFromFile(CString fileName, GLenum wrap = GL_REPEAT);
	void Select();
	void Release();

	static void PrepareTexturing(bool bEnableLighting);
	static void DeselectTexture();
};


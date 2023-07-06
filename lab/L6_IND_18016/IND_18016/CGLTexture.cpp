#include "pch.h"
#include "CGLTexture.h"

CGLTexture::CGLTexture()
{
	m_ID = 0;
}

CGLTexture::~CGLTexture()
{
	Release();
}

void CGLTexture::LoadFromFile(CString fileName, GLenum wrap)
{
	Release();

	DImage img;
	img.Load(fileName);

	glGenTextures(1, &m_ID);
	Select();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_RGBA, GL_UNSIGNED_BYTE, img.GetDIBBits());
}

void CGLTexture::Select()
{
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void CGLTexture::Release()
{
	glDeleteTextures(1, &m_ID);
	m_ID = 0;
}

void CGLTexture::PrepareTexturing(bool bEnableLighting)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	if (bEnableLighting)
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	else
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}
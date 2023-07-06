#pragma once

#include "GL/GL.h"
#include "GL/GLU.h"

class CGLMaterial
{
private:
	float m_vAmbient[4];
	float m_vDiffuse[4];
	float m_vSpecular[4];
	float m_vEmission[4];
	float m_fShininess;
public:
	CGLMaterial();

	void Select(GLenum face = GL_FRONT);
	void SetAmbient(float r, float g, float b, float a);
	void SetDiffuse(float r, float g, float b, float a);
	void SetSpecular(float r, float g, float b, float a);
	void SetEmission(float r, float g, float b, float a);
	void SetShininess(float s);
};
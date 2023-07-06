#pragma once

#include "pch.h"
#include "CGLMaterial.h"

#include <gl/GL.h>
#include <gl/GLU.h>

#define _USE_MATH_DEFINES
#include <math.h>

#define MAX_ZOOM 100
#define MIN_ZOOM 10
#define BOX_SIDE 70.0

class CGLRenderer
{
private:
	HGLRC m_hrc;
	int m_zoomDistance = 35;
	double m_xRot = 0;
	double m_yRot = 0;
	double m_zRot = 0;
	double m_XZrotAngle = 0;
	double m_XYrotAngle = 0;
	CGLMaterial m_vaseMat1, m_vaseMat2, m_wallMat, m_redLightMat, m_purpleLightMat, m_blueLightMat;
public:
	bool m_showNormals = false;
	bool m_redLightOn = false, m_purpleLightOn = false, m_blueLightOn = false;

	CGLRenderer();
	bool CreateGLContext(CDC* pDC);
	void PrepareScene(CDC* pDC);
	void DestroyScene(CDC* pDC);
	void Reshape(CDC* pDC, int w, int h);

	double DegToRad(double angle) { return angle * M_PI / 180; };
	void RotateView(int dX, int dY);
	void Zoom(int val);

	void DrawScene(CDC* pDC);
	void DrawAxis(double width, double lineWidth = 1);
	void DrawRoom();
	void DrawLights();
	void DrawLight(GLenum light, bool on, CGLMaterial& mat, int angle, GLfloat d[], GLfloat pos[], GLfloat dir[]);
	void DrawBase();
	void DrawVase();
	void DrawPart(double h, double r1, double r2, double r3);
	void DrawHalfSphere(double r, int nSegAlpha, int nSegBeta);
	void DrawCylinder(double h, double r1, double r2, int nSeg, bool drawBase = false);
	void DrawRectangle(double h, double w, int nSegH, int nSegW);
	void DrawCuboid(double h, double w, double d, int nSegH, int nSegW, int nSegD);
	void DrawCube(double side, int nSeg);
};
#pragma once

#include "pch.h"

#include "CGLTexture.h"

#include <gl/GL.h>
#include <gl/GLU.h>

#define _USE_MATH_DEFINES
#include <math.h>

#define MAX_ZOOM 20
#define MIN_ZOOM 10

class CGLRenderer
{
private:
	HGLRC m_hrc;
	int m_zoomDistance = MAX_ZOOM;
	double m_xRot = 0;
	double m_yRot = 0;
	double m_zRot = 0;
	double m_XZrotAngle = 0;
	double m_XYrotAngle = 0;
	CGLTexture m_tGround, m_tTruck;
public:
	CGLRenderer();
	bool CreateGLContext(CDC* pDC);
	void PrepareScene(CDC* pDC);
	void DestroyScene(CDC* pDC);
	void Reshape(CDC* pDC, int w, int h);

	double DegToRad(double angle) { return angle * M_PI / 180; };
	void RotateView(int dX, int dY);
	void Zoom(int val);
	void SetLighting();

	void DrawScene(CDC* pDC);
	void DrawAxis(double width, double lineWidth = 1);

	void DrawGround(double w, double h, double s = 1, double t = 1);
	void DrawTruck();
	void DrawTruckSide();
	void DrawTruckCover();
	void DrawWheel(double dX, double dY, double dZ);
	void DrawTrunk();
};
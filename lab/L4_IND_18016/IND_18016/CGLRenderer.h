#pragma once

#include <gl/GL.h>
#include <gl/GLU.h>

class CGLRenderer
{
private:
	HGLRC m_hrc;
	double m_zoomDistance = 15;
	double m_xRot = 0;
	double m_yRot = 0;
	double m_zRot = 0;
	double m_armRotAngle = 0;
	double m_XZrotAngle = 0;
	double m_XYrotAngle = 0;
public:
	CGLRenderer();

	bool CreateGLContext(CDC* pDC);
	void PrepareScene(CDC* pDC);
	void DestroyScene(CDC* pDC);
	void Reshape(CDC* pDC, int w, int h);

	double DegToRad(double angle);
	void RotateArm(double angle);
	void RotateView(int dX, int dY);
	void Zoom(int val);

	void DrawScene(CDC* pDC);
	void DrawSphere(double r, int nSegAlpha, int nSegBeta);
	void DrawCylinder(double h, double r1, double r2, int nSeg);
	void DrawCone(double h, double r, int nSeg);
	void DrawAxis(double width);
	void DrawGrid(double width, double height, int nSegW, int nSegH);
	void DrawFigure(double angle);
};
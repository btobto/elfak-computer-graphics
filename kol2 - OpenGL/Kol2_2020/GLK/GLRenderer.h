#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#define MAX_ZOOM 50
#define MIN_ZOOM 8

class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	double DegToRad(double angle) { return angle * M_PI / 180; }
	void DrawAxes(int l = 50, int w = 1);
	void RotateView(int dX, int dY);
	void Zoom(int dX);
	UINT LoadTexture(char* fileName);
	void DrawEnvCube(double a);
	void DrawSphere(double r, int nSeg, double texU, double texV, double texR);
	void DrawCone(double r, double h, int nSeg, double texU, double texV, double texR);
	void DrawLegSegment(double r, double h, int nSeg);
	void DrawLeg();
	void DrawSpiderBody();
	void DrawSpider();

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

	UINT m_texSpider;
	UINT m_texEnv[6];

	int m_zoomDistance = MAX_ZOOM;
	double m_xRot = 0;
	double m_yRot = 0;
	double m_zRot = 0;
	double m_XZrotAngle = 0;
	double m_XYrotAngle = 0;
};

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
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,
	void DrawScene(CDC* pDC);				// iscrtava scenu

	double DegToRad(double angle) { return angle * M_PI / 180; }
	void DrawAxis(int l = 10, int w = 1);
	void RotateView(int dX, int dY);
	void Zoom(int d);
	UINT LoadTexture(char* fileName);

	void DrawPolygon(POINTF* points, POINTF* texCoords, int n);
	void DrawExtrudedPolygon(POINTF* points, POINTF* texCoords, int n, float zh, float r, float g, float b);
	
	void DrawBase();
	void DrawBody();
	void DrawArm(double zh);
	void DrawFork();
	void DrawExcavator();

	void DrawEnvCube(double a);
protected:
	HGLRC m_hrc; //OpenGL Rendering Context 
	UINT m_texExcavator;
	UINT m_texEnv[6];

	double m_XZangle = 90, m_XYangle = 0;
	double m_eyeX, m_eyeY, m_eyeZ;
	double m_zoomDistance = 20;

public:
	double m_cabinRot = 0;
	double m_arm1Rot = 90, m_arm2Rot = -90, m_forkRot = -90;
};

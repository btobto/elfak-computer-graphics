#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

#include "DImage.h";

CGLRenderer::CGLRenderer(void)
{
	RotateView(0, 0);
}

CGLRenderer::~CGLRenderer(void)
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd ;
   	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
   	pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
   	pfd.nVersion   = 1; 
   	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;   
   	pfd.iPixelType = PFD_TYPE_RGBA; 
   	pfd.cColorBits = 32;
   	pfd.cDepthBits = 24; 
   	pfd.iLayerType = PFD_MAIN_PLANE;
	
	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);
	
	if (nPixelFormat == 0) return false; 

	BOOL bResult = SetPixelFormat (pDC->m_hDC, nPixelFormat, &pfd);
  	
	if (!bResult) return false; 

   	m_hrc = wglCreateContext(pDC->m_hDC); 

	if (!m_hrc) return false; 

	return true;	
}

void CGLRenderer::PrepareScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	
	glClearColor(1, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	m_texExcavator = LoadTexture("excavator.png");
	
	m_texEnv[0] = LoadTexture("front.jpg");
	m_texEnv[1] = LoadTexture("back.jpg");
	m_texEnv[2] = LoadTexture("left.jpg");
	m_texEnv[3] = LoadTexture("right.jpg");
	m_texEnv[4] = LoadTexture("top.jpg");
	m_texEnv[5] = LoadTexture("bot.jpg");

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}


void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(55, (double)w / (double)h, 1, 1000);
	glMatrixMode(GL_MODELVIEW);

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 

	glDeleteTextures(1, &m_texExcavator);
	glDeleteTextures(6, m_texEnv);

	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawAxis(int l, int w)
{
	glLineWidth(w);

	glBegin(GL_LINES);
	{
		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(l, 0, 0);

		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, l, 0);

		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, l);
	}
	glEnd();

	glLineWidth(1);
}

void CGLRenderer::RotateView(int dX, int dY)
{
	m_XZangle += dX;
	m_XYangle += dY;

	if (m_XYangle > 90) m_XYangle = 90;
	else if (m_XYangle < 0) m_XYangle = 0;

	double alpha = DegToRad(m_XYangle);
	double beta = DegToRad(m_XZangle);

	m_eyeX = m_zoomDistance * cos(alpha) * cos(beta);
	m_eyeY = m_zoomDistance * sin(alpha);
	m_eyeZ = m_zoomDistance * cos(alpha) * sin(beta);
}

void CGLRenderer::Zoom(int d)
{
	m_zoomDistance += d;
	if (m_zoomDistance > MAX_ZOOM) m_zoomDistance = MAX_ZOOM;
	else if (m_zoomDistance < MIN_ZOOM) m_zoomDistance = MIN_ZOOM;
	RotateView(0, 0);
}

UINT CGLRenderer::LoadTexture(char* fileName)
{
	UINT texID;
	DImage img;
	img.Load(CString(fileName));

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texID;
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(m_eyeX, m_eyeY, m_eyeZ, 0, 0, 0, 0, 1, 0);

	glPushMatrix();
	{

		glTranslated(0, -5, 0);

		DrawEnvCube(100);
		DrawAxis();

		glTranslated(-4, 0, -2.5);
		DrawExcavator();
	}
	glPopMatrix();

	glFlush();
	SwapBuffers(pDC->m_hDC);

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawPolygon(POINTF* points, POINTF* texCoords, int n)
{
	glColor3f(1, 1, 1);
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < n; i++)
	{
		glTexCoord2f(texCoords[i].x, texCoords[i].y);
		glVertex3f(points[i].x, points[i].y, 0);
	}
	glEnd();
}

void CGLRenderer::DrawExtrudedPolygon(POINTF* points, POINTF* texCoords, int n, float zh, float r, float g, float b)
{
	DrawPolygon(points, texCoords, n);

	glDisable(GL_TEXTURE_2D);

	glColor3f(r, g, b);
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i < n; i++)
	{
		glVertex3f(points[i].x, points[i].y, 0);
		glVertex3f(points[i].x, points[i].y, zh);
	}
		glVertex3f(points[0].x, points[0].y, 0);
		glVertex3f(points[0].x, points[0].y, zh);
	glEnd();

	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	{
		glTranslated(0, 0, zh);
		DrawPolygon(points, texCoords, n);
	}
	glPopMatrix();
}

void CGLRenderer::DrawExcavator()
{
	glBindTexture(GL_TEXTURE_2D, m_texExcavator);

	glPushMatrix();
	{
		DrawBase();

		glTranslated((8 - 4) / 2.0, 2, (5 - 4) / 2.0);
		glTranslated(2, 0, 2);
		glRotated(m_cabinRot, 0, 1, 0);
		glTranslated(-2, 0, -2);
		DrawBody();

		glTranslated(2, 0, -1);
		glTranslated(1, 1, 0);
		glRotated(m_arm1Rot, 0, 0, 1);
		glTranslated(-1, -1, 0);
		DrawArm(1.0);

		glTranslated(7, 0, -0.25);
		glTranslated(1, 1, 0);
		glRotated(m_arm2Rot, 0, 0, 1);
		glTranslated(-1, -1, 0);
		DrawArm(1.5);

		glTranslated(6.5, 0, 0.25);
		glTranslated(1, 1, 0);
		glRotated(m_forkRot, 0, 0, 1);
		glTranslated(-1, -1, 0);
		DrawFork();
	}
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);
}

void CGLRenderer::DrawBase()
{
	POINTF coords[] = {
		{ 0, 0.5 }, { 0.5, 0 }, { 7.5, 0 }, { 8, 0.5 },
		{ 8, 1.5 }, { 7.5, 2 }, { 0.5, 2 }, { 0, 1.5 }
	};

	POINTF texCoords[8];
	for (int i = 0; i < 8; i++)
	{
		texCoords[i] = { coords[i].x / 8.f, 1 - coords[i].y / 8.f };
	}

	DrawExtrudedPolygon(coords, texCoords, 8, 5, 0, 0, 0);
}

void CGLRenderer::DrawBody()
{
	POINTF coords[] = { { 0, 0 }, { 4, 0 }, { 4, 2 }, { 2, 4}, { 0, 4} };

	POINTF texCoords[5];
	for (int i = 0; i < 5; i++)
	{
		texCoords[i] = { 0.5f + coords[i].x / 8, 0.5f - coords[i].y / 8 };
	}

	DrawExtrudedPolygon(coords, texCoords, 5, 4, 0.96, 0.5, 0.12);
}

void CGLRenderer::DrawArm(double zh)
{
	POINTF coords[] = { 
		{ 0, 0.5 }, { 0.5, 0 }, { 2, 0 }, { 8, 0.5 }, 
		{ 8, 1.5 }, { 2, 2 }, { 0.5, 2 }, { 0, 1.5 } 
	};

	POINTF texCoords[8];
	for (int i = 0; i < 8; i++)
	{
		texCoords[i] = { coords[i].x / 8, 0.75f - coords[i].y / 8 };
	}

	DrawExtrudedPolygon(coords, texCoords, 8, zh, 0.96, 0.5, 0.12);
}

void CGLRenderer::DrawFork()
{
	POINTF coords[] = {
		{ 0, 0 }, { 4, 0 }, { 4, 2.5 }, 
		{ 3.5, 3 }, { 0.5, 3 }, { 0, 2.5 }
	};

	POINTF texCoords[6];
	for (int i = 0; i < 6; i++)
	{
		texCoords[i] = { coords[i].x / 8, 0.375f - coords[i].y / 8 };
	}

	DrawExtrudedPolygon(coords, texCoords, 6, 1, 0.7, 0.7, 0.7);
}

void CGLRenderer::DrawEnvCube(double a)
{
	// front
	glBindTexture(GL_TEXTURE_2D, m_texEnv[0]);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex3f(a / 2, a, a / 2);

		glTexCoord2f(0, 1);
		glVertex3f(a / 2, 0, a / 2);

		glTexCoord2f(1, 1);
		glVertex3f(-a / 2, 0, a / 2);

		glTexCoord2f(1, 0);
		glVertex3f(-a / 2, a, a / 2);
	}
	glEnd();

	// back
	glBindTexture(GL_TEXTURE_2D, m_texEnv[1]);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex3f(-a / 2, a, -a / 2);

		glTexCoord2f(0, 1);
		glVertex3f(-a / 2, 0, -a / 2);

		glTexCoord2f(1, 1);
		glVertex3f(a / 2, 0, -a / 2);

		glTexCoord2f(1, 0);
		glVertex3f(a / 2, a, -a / 2);
	}
	glEnd();

	// left
	glBindTexture(GL_TEXTURE_2D, m_texEnv[2]);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex3f(-a / 2, a, a / 2);

		glTexCoord2f(0, 1);
		glVertex3f(-a / 2, 0, a / 2);

		glTexCoord2f(1, 1);
		glVertex3f(-a / 2, 0, -a / 2);

		glTexCoord2f(1, 0);
		glVertex3f(-a / 2, a, -a / 2);
	}
	glEnd();

	// right
	glBindTexture(GL_TEXTURE_2D, m_texEnv[3]);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex3f(a / 2, a, -a / 2);

		glTexCoord2f(0, 1);
		glVertex3f(a / 2, 0, -a / 2);

		glTexCoord2f(1, 1);
		glVertex3f(a / 2, 0, a / 2);

		glTexCoord2f(1, 0);
		glVertex3f(a / 2, a, a / 2);
	}
	glEnd();

	// top
	glBindTexture(GL_TEXTURE_2D, m_texEnv[4]);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex3f(a / 2, a, -a / 2);

		glTexCoord2f(0, 1);
		glVertex3f(a / 2, a, a / 2);

		glTexCoord2f(1, 1);
		glVertex3f(-a / 2, a, a / 2);

		glTexCoord2f(1, 0);
		glVertex3f(-a / 2, a, -a / 2);
	}
	glEnd();

	// bottom
	glBindTexture(GL_TEXTURE_2D, m_texEnv[5]);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex3f(a / 2, 0, -a / 2);

		glTexCoord2f(0, 1);
		glVertex3f(a / 2, 0, a / 2);

		glTexCoord2f(1, 1);
		glVertex3f(-a / 2, 0, a / 2);

		glTexCoord2f(1, 0);
		glVertex3f(-a / 2, 0, -a / 2);
	}
	glEnd();
}
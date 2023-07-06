#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

#include "DImage.h"

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

	m_texSpider = LoadTexture("spider.png");

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
	gluPerspective(50, (double)w / (double)h, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 

	glDeleteTextures(1, &m_texSpider);
	glDeleteTextures(6, m_texEnv);

	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawAxes(int l, int w)
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
	m_XZrotAngle += dX;
	m_XYrotAngle += dY;

	if (m_XYrotAngle >= 90) m_XYrotAngle = 89;
	else if (m_XYrotAngle <= 10) m_XYrotAngle = 10;

	double alphaRad = DegToRad(m_XYrotAngle);
	double betaRad = DegToRad(m_XZrotAngle);

	m_xRot = m_zoomDistance * cos(betaRad) * cos(alphaRad);
	m_yRot = m_zoomDistance * sin(alphaRad);
	m_zRot = m_zoomDistance * sin(betaRad) * cos(alphaRad);
}

void CGLRenderer::Zoom(int x)
{
	m_zoomDistance += x;
	if (m_zoomDistance < MIN_ZOOM) m_zoomDistance = MIN_ZOOM;
	else if (m_zoomDistance > MAX_ZOOM) m_zoomDistance = MAX_ZOOM;
	RotateView(0, 0);
}

UINT CGLRenderer::LoadTexture(char* fileName)
{
	UINT texId = 1;

	DImage img;
	img.Load(CString(fileName));

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texId;
}

void CGLRenderer::DrawEnvCube(double a)
{
	glColor3f(1, 1, 1);

	glEnable(GL_TEXTURE_2D);

	// front
	glBindTexture(GL_TEXTURE_2D, m_texEnv[0]);
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

	// back
	glBindTexture(GL_TEXTURE_2D, m_texEnv[1]);
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
	glPushMatrix();
	glRotated(90, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, m_texEnv[4]);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex3f(-a / 2, a, a / 2);

		glTexCoord2f(0, 1);
		glVertex3f(-a / 2, a, -a / 2);

		glTexCoord2f(1, 1);
		glVertex3f(a / 2, a, -a / 2);

		glTexCoord2f(1, 0);
		glVertex3f(a / 2, a, a / 2);
	}
	glEnd();
	glPopMatrix();

	// bottom
	glPushMatrix();
	glRotated(90, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, m_texEnv[5]);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex3f(a / 2, 0, a / 2);

		glTexCoord2f(0, 1);
		glVertex3f(a / 2, 0, -a / 2);

		glTexCoord2f(1, 1);
		glVertex3f(-a / 2, 0, -a / 2);

		glTexCoord2f(1, 0);
		glVertex3f(-a / 2, 0, a / 2);
	}
	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(m_xRot, m_yRot, m_zRot, 0, 0, 0, 0, 1, 0);

	glPushMatrix();
	{
		glTranslated(0, -10, 0);

		DrawAxes();
		DrawEnvCube(100);
		glTranslated(0, 5, 0);
		DrawSpider();
	}
	glPopMatrix();

	glFlush();
	SwapBuffers(pDC->m_hDC);

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}


void CGLRenderer::DrawSphere(double r, int nSeg, double texU, double texV, double texR)
{
	glColor3f(1, 1, 1);

	int nSegW = 2 * nSeg;
	
	int stepA = 180.0 / nSeg;
	int stepB = 360.0 / nSegW;

	for (double a = 90; a > -90; a -= stepA)
	{
		double alpha1 = DegToRad(a);
		double alpha2 = DegToRad(a - stepA);

		double y1 = r * sin(alpha1);
		double y2 = r * sin(alpha2);;

		glBegin(GL_QUAD_STRIP);
		for (double b = 0; b <= 360; b += stepB)
		{
			double beta = DegToRad(b);

			double x1 = r * cos(alpha1) * cos(beta);
			double z1 = r * cos(alpha1) * sin(beta);

			double x2 = r * cos(alpha2) * cos(beta);
			double z2 = r * cos(alpha2) * sin(beta);
			
			double tx1 = texU + texR * x1 / r;
			double ty1 = texV + texR * z1 / r;

			double tx2 = texU + texR * x2 / r;
			double ty2 = texV + texR * z2 / r;

			glTexCoord2f(tx1, ty1);
			glVertex3f(x1, y1, z1);

			glTexCoord2f(tx2, ty2);
			glVertex3f(x2, y2, z2);
		}
		glEnd();
	}
}

void CGLRenderer::DrawCone(double r, double h, int nSeg, double texU, double texV, double texR)
{
	glColor3f(1, 1, 1);

	double step = 360.0 / nSeg;

	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(texU, texV);
	glVertex3f(0, h, 0);
	for (double alpha = 0; alpha <= 360; alpha += step)
	{
		double x = r * cos(DegToRad(alpha));
		double z = r * sin(DegToRad(alpha));

		double tx = texU + x / r * texR;
		double ty = texV + z / r * texR;

		glTexCoord2f(tx, ty);
		glVertex3f(x, 0, z);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0, 0);
	for (double alpha = 0; alpha <= 360; alpha += step)
	{
		double x = r * cos(DegToRad(alpha));
		double z = r * sin(DegToRad(alpha));

		glVertex3f(x, 0, z);
	}
	glEnd();
}

void CGLRenderer::DrawLegSegment(double r, double h, int nSeg)
{
	glPushMatrix();
	{
		glTranslatef(0, r, 0);
		DrawSphere(r, 2 * nSeg, 0.25, 0.25, 0.24);
		DrawCone(r, h, nSeg, 0.75, 0.75, 0.24);
	}
	glPopMatrix();
}

void CGLRenderer::DrawLeg()
{
	glPushMatrix();
	{
		DrawLegSegment(1, 10, 5);
		glTranslated(0, 10, 0);
		glRotated(85, 1, 0, 0);
		DrawLegSegment(1, 15, 5);
	}
	glPopMatrix();
}

void CGLRenderer::DrawSpiderBody()
{
	// upper body
	glPushMatrix();
	{
		glScaled(1, 0.5, 1);
		DrawSphere(3, 10, 0.25, 0.25, 0.24);
	}
	glPopMatrix();

	// lower body
	glPushMatrix();
	{
		glTranslated(6, 0, 0);
		glScaled(1, 0.8, 1);
		DrawSphere(5, 10, 0.25, 0.25, 0.24);
	}
	glPopMatrix();

	// head
	glPushMatrix();
	{
		glTranslated(-4, 0, 0);
		glScaled(1, 0.5, 2);
		DrawSphere(2, 10, 0.75, 0.25, 0.24);
	}
	glPopMatrix();
}

void CGLRenderer::DrawSpider()
{
	glColor3f(1, 1, 1);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texSpider);

	DrawSpiderBody();

	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
		{
			glRotated(i * 30 - 45, 0, 1, 0);
			glRotated(45, 1, 0, 0);
			DrawLeg();
		}
		glPopMatrix();
	}

	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
		{
			glRotated(i * 30 - 45 + 180, 0, 1, 0);
			glRotated(45, 1, 0, 0);
			DrawLeg();
		}
		glPopMatrix();
	}
}

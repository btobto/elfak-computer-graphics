#include "pch.h"
#include "CGLRenderer.h"

#define SQUARE 1.0 / 16
#define WIDTH 6

CGLRenderer::CGLRenderer()
{
	RotateView(0, 20);
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);

    if (nPixelFormat == 0) return false;

    BOOL bResult = SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);

    if (!bResult) return false;

    m_hrc = wglCreateContext(pDC->m_hDC);

    if (!m_hrc) return false;

    return true;
}

void CGLRenderer::PrepareScene(CDC* pDC)
{
    wglMakeCurrent(pDC->m_hDC, m_hrc);

    glClearColor(0.8, 0.8, 0.8, 0.0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    CGLTexture::PrepareTexturing(true);
    m_tGround.LoadFromFile(L"./grass.jpg");
    m_tTruck.LoadFromFile(L"./truck.png");

    float lmAmbient[] = { 0.9, 0.9, 0.9, 1.0 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmAmbient);

    GLfloat dirAmbient[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat dirDiffuse[] = { 1, 1, 1, 0 };
    GLfloat dirSpecular[] = { 0.2, 0.2, 0.2, 0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, dirAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, dirDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, dirSpecular);

    wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC* pDC)
{
    wglMakeCurrent(pDC->m_hDC, m_hrc);

    wglMakeCurrent(NULL, NULL);
    if (m_hrc) {
        wglDeleteContext(m_hrc);
        m_hrc = NULL;
    }
}

void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
    wglMakeCurrent(pDC->m_hDC, m_hrc);

    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40, (double)w / (double)h, 1, 100);
    glMatrixMode(GL_MODELVIEW);

    wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::RotateView(int dX, int dY)
{
    m_XZrotAngle += dX;
    m_XYrotAngle += dY;

    if (m_XYrotAngle >= 90) m_XYrotAngle = 89.9;
    else if (m_XYrotAngle <= -90) m_XYrotAngle = -89.9;

    double alphaRad = DegToRad(m_XYrotAngle);
    double betaRad = DegToRad(m_XZrotAngle);

    m_xRot = cos(betaRad) * cos(alphaRad) * m_zoomDistance;
    m_yRot = sin(alphaRad) * m_zoomDistance;
    m_zRot = sin(betaRad) * cos(alphaRad) * m_zoomDistance;
}

void CGLRenderer::Zoom(int val)
{
    m_zoomDistance += val;
    if (m_zoomDistance < MIN_ZOOM) m_zoomDistance = MIN_ZOOM;
    else if (m_zoomDistance > MAX_ZOOM) m_zoomDistance = MAX_ZOOM;
    RotateView(0, 0);
}

void CGLRenderer::SetLighting()
{
    GLfloat pos[] = { 0.5, 1, 0.5, 0 };
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glEnable(GL_LIGHT0);
}

void CGLRenderer::DrawScene(CDC* pDC)
{
    wglMakeCurrent(pDC->m_hDC, m_hrc);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(m_xRot, m_yRot, m_zRot, 0, 0, 0, 0, 1, 0);
    SetLighting();

    glPushMatrix();
    {
        glTranslated(0, -2.5, 0);

        DrawAxis(15);

        DrawTruck();
        glTranslated(0, -1.5, 0);
        DrawGround(50, 50, 4, 4);
    }
    glPopMatrix();

    glFlush();

    SwapBuffers(pDC->m_hDC);
    wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawGround(double w, double h, double s, double t)
{
    m_tGround.Select();

    glBegin(GL_QUADS);
    {
        glNormal3f(0, 1, 0);

        glTexCoord2f(0, t);
        glVertex3f(-w / 2, 0, -h / 2);

        glTexCoord2f(0, 0);
        glVertex3f(-w / 2, 0, h / 2);
        
        glTexCoord2f(s, 0);
        glVertex3f(w / 2, 0, h / 2);

        glTexCoord2f(s, t);
        glVertex3f(w / 2, 0, -h / 2);
    }
    glEnd();
}

void CGLRenderer::DrawTruck()
{
    m_tTruck.Select();

    glPushMatrix();
    {
        glTranslated(-4.5, 0, -WIDTH / 2);

        DrawTruckSide();
        glPushMatrix();
        {
            glScaled(1, 1, -1);
            glTranslated(0, 0, -WIDTH);
            DrawTruckSide();
        }
        glPopMatrix();

        DrawTruckCover();

        DrawWheel(2.5, 0.5, 0);
        DrawWheel(2.5, 4.5, 0);

        DrawWheel(6.5, 0.5, 0);
        DrawWheel(6.5, 4.5, 0);

        DrawTrunk();
    }
    glPopMatrix();
}

void CGLRenderer::DrawTruckSide()
{
    glBegin(GL_POLYGON);
    {
        glNormal3f(0, 0, 1);

        glTexCoord2f(4 * SQUARE, 4 * SQUARE);
        glVertex3f(4, 3, 0);

        glTexCoord2f(4 * SQUARE, 0);
        glVertex3f(4, 7, 0);

        glTexCoord2f(2 * SQUARE, 0);
        glVertex3f(2, 7, 0);

        glTexCoord2f(0, 2 * SQUARE);
        glVertex3f(0, 5, 0);

        glTexCoord2f(0, 5 * SQUARE);
        glVertex3f(0, 2, 0);

        glTexCoord2f(9 * SQUARE, 5 * SQUARE);
        glVertex3f(9, 2, 0);

        glTexCoord2f(9 * SQUARE, 4 * SQUARE);
        glVertex3f(9, 3, 0);
    }
    glEnd();

    glBegin(GL_POLYGON);
    {
        glTexCoord2f(0, 5 * SQUARE);
        glVertex3f(0, 2, 0);

        glTexCoord2f(0, 7 * SQUARE);
        glVertex3f(0, 0, 0);

        glTexCoord2f(SQUARE, 7 * SQUARE);
        glVertex3f(1, 0, 0);

        glTexCoord2f(SQUARE, 6 * SQUARE);
        glVertex3f(1, 1, 0);

        glTexCoord2f(2 * SQUARE, 5 * SQUARE);
        glVertex3f(2, 2, 0);
    }
    glEnd();

    glBegin(GL_POLYGON);
    {
        glTexCoord2f(5 * SQUARE, 6 * SQUARE);
        glVertex3f(5, 1, 0);

        glTexCoord2f(6 * SQUARE, 5 * SQUARE);
        glVertex3f(6, 2, 0);

        glTexCoord2f(3 * SQUARE, 5 * SQUARE);
        glVertex3f(3, 2, 0);

        glTexCoord2f(4 * SQUARE, 6 * SQUARE);
        glVertex3f(4, 1, 0);

        glTexCoord2f(4 * SQUARE, 7 * SQUARE);
        glVertex3f(4, 0, 0);

        glTexCoord2f(5 * SQUARE, 7 * SQUARE);
        glVertex3f(5, 0, 0);
    }
    glEnd();

    glBegin(GL_POLYGON);
    {
        glTexCoord2f(8 * SQUARE, 6 * SQUARE);
        glVertex3f(8, 1, 0);

        glTexCoord2f(8 * SQUARE, 7 * SQUARE);
        glVertex3f(8, 0, 0);

        glTexCoord2f(9 * SQUARE, 7 * SQUARE);
        glVertex3f(9, 0, 0);

        glTexCoord2f(9 * SQUARE, 5 * SQUARE);
        glVertex3f(9, 2, 0);

        glTexCoord2f(7 * SQUARE, 5 * SQUARE);
        glVertex3f(7, 2, 0);
    }
    glEnd();
}

void CGLRenderer::DrawTruckCover()
{
    glBegin(GL_QUADS);
    {
        glNormal3f(-1, 0, 0);
        glVertex3f(0, 5, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, WIDTH);
        glVertex3f(0, 5, WIDTH);

        glNormal3f(-1, 1, 0);
        glVertex3f(2, 7, 0);
        glVertex3f(0, 5, 0);
        glVertex3f(0, 5, WIDTH);
        glVertex3f(2, 7, WIDTH);

        glNormal3f(0, 1, 0);
        glVertex3f(4, 7, 0);
        glVertex3f(2, 7, 0);
        glVertex3f(2, 7, WIDTH);
        glVertex3f(4, 7, WIDTH);

        glNormal3f(-1, 0, 0);
        glVertex3f(4, 3, 0);
        glVertex3f(4, 7, 0);
        glVertex3f(4, 7, WIDTH);
        glVertex3f(4, 3, WIDTH);

        glNormal3f(0, 1, 0);
        glVertex3f(9, 3, 0);
        glVertex3f(4, 3, 0);
        glVertex3f(4, 3, WIDTH);
        glVertex3f(9, 3, WIDTH);

        glNormal3f(1, 0, 0);
        glVertex3f(9, 0, 0);
        glVertex3f(9, 3, 0);
        glVertex3f(9, 3, WIDTH);
        glVertex3f(9, 0, WIDTH);

        glNormal3f(0, -1, 0);
        glVertex3f(9, 0, 0);
        glVertex3f(9, 0, WIDTH);
        glVertex3f(8, 0, WIDTH);
        glVertex3f(8, 0, 0);

        glNormal3f(-1, 0, 0);
        glVertex3f(8, 1, WIDTH);
        glVertex3f(8, 1, 0);
        glVertex3f(8, 0, 0);
        glVertex3f(8, 0, WIDTH);

        glNormal3f(-1, -1, 0);
        glVertex3f(7, 2, WIDTH);
        glVertex3f(7, 2, 0);
        glVertex3f(8, 1, 0);
        glVertex3f(8, 1, WIDTH);

        glNormal3f(0, -1, 0);
        glVertex3f(7, 2, 0);
        glVertex3f(7, 2, WIDTH);
        glVertex3f(6, 2, WIDTH);
        glVertex3f(6, 2, 0);

        glNormal3f(1, -1, 0);
        glVertex3f(6, 2, 0);
        glVertex3f(6, 2, WIDTH);
        glVertex3f(5, 1, WIDTH);
        glVertex3f(5, 1, 0);

        glNormal3f(1, 0, 0);
        glVertex3f(5, 1, 0);
        glVertex3f(5, 1, WIDTH);
        glVertex3f(5, 0, WIDTH);
        glVertex3f(5, 0, 0);

        glNormal3f(0, -1, 0);
        glVertex3f(4, 0, WIDTH);
        glVertex3f(4, 0, 0);
        glVertex3f(5, 0, 0);
        glVertex3f(5, 0, WIDTH);

        glNormal3f(-1, 0, 0);
        glVertex3f(4, 1, WIDTH);
        glVertex3f(4, 1, 0);
        glVertex3f(4, 0, 0);
        glVertex3f(4, 0, WIDTH);

        glNormal3f(-1, -1, 0);
        glVertex3f(3, 2, WIDTH);
        glVertex3f(3, 2, 0);
        glVertex3f(4, 1, 0);
        glVertex3f(4, 1, WIDTH);

        glNormal3f(0, -1, 0);
        glVertex3f(3, 2, 0);
        glVertex3f(3, 2, WIDTH);
        glVertex3f(2, 2, WIDTH);
        glVertex3f(2, 2, 0);

        glNormal3f(1, -1, 0);
        glVertex3f(2, 2, 0);
        glVertex3f(2, 2, WIDTH);
        glVertex3f(1, 1, WIDTH);
        glVertex3f(1, 1, 0);

        glNormal3f(1, 0, 0);
        glVertex3f(1, 1, 0);
        glVertex3f(1, 1, WIDTH);
        glVertex3f(1, 0, WIDTH);
        glVertex3f(1, 0, 0);

        glNormal3f(0, -1, 0);
        glVertex3f(0, 0, WIDTH);
        glVertex3f(0, 0, 0);
        glVertex3f(1, 0, 0);
        glVertex3f(1, 0, WIDTH);
    }
    glEnd();
}

void CGLRenderer::DrawWheel(double dX, double dY, double dZ)
{
    glDisable(GL_LIGHTING);
    glColor3f(1, 1, 1);
    glPushMatrix();

    glRotated(90, 1, 0, 0);
    glTranslated(dX, dY, dZ);
    glRotated(90, 0, 1, 0);

    double h = 1;
    double r = 1.5;
    int nSeg = 16;
    double step = 360.0 / nSeg;

    glBegin(GL_TRIANGLE_FAN);
    glNormal3d(0, 1, 0);
    for (double alpha = 0; alpha <= 360; alpha += step) {
        double x = r * cos(DegToRad(alpha));
        double z = r * sin(DegToRad(alpha));
        glTexCoord2f((x + 9) * SQUARE, (z + 1.5) * SQUARE);
        glVertex3f(x, h, z);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glNormal3d(0, -1, 0);
    for (double alpha = 0; alpha <= 360; alpha += step) {
        double x = r * cos(DegToRad(alpha));
        double z = r * sin(DegToRad(alpha));
        glTexCoord2f((x + 9) * SQUARE, (z + 1.5) * SQUARE);
        glVertex3f(x, 0, z);
    }
    glEnd();

    glBegin(GL_QUAD_STRIP);
    for (double alpha = 0; alpha <= 360; alpha += step) {
        double alphaRad = DegToRad(alpha);

        double nx = cos(alphaRad);
        double nz = sin(alphaRad);
        double x = r * nx;
        double z = r * nz;

        glVertex3f(x, h, z);
        glVertex3f(x, 0, z);
    }
    glEnd();

    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void CGLRenderer::DrawTrunk()
{
    glPushMatrix();
    glEnable(GL_NORMALIZE);

    glTranslatef(6.5, 5, 3);
    glScaled(1.2, 1, 1.5);
    glRotated(180, 0, 1, 0);

    int nSegAlpha = 8;
    int nSegBeta = 16;
    int r = 2;

    double stepA = 180.0 / nSegAlpha;
    double stepB = 360.0 / nSegBeta;

    int dS = 16, dT = 8;
    glBegin(GL_QUAD_STRIP);
    for (double a = 90; a > -90; a -= stepA) {
        double alpha1 = DegToRad(a);
        double alpha2 = DegToRad(a - stepA);

        double ny1 = sin(alpha1);
        double ny2 = sin(alpha2);
        double y1 = r * ny1;
        double y2 = r * ny2;

        dS = 16;
        for (double b = 0; b <= 360; b += stepB) {

            double beta = DegToRad(b);

            double nx1 = cos(alpha1) * cos(beta);
            double nz1 = cos(alpha1) * sin(beta);
            double x1 = r * nx1;
            double z1 = r * nz1;

            double nx2 = cos(alpha2) * cos(beta);
            double nz2 = cos(alpha2) * sin(beta);
            double x2 = r * nx2;
            double z2 = r * nz2;

            glNormal3f(nx1, ny1, nz1);
            glTexCoord2f(dS * SQUARE, dT * SQUARE);
            glVertex3f(x1, y1, z1);

            glNormal3f(nx2, ny2, nz2);
            glTexCoord2f(dS * SQUARE, (dT + 1) * SQUARE);
            glVertex3f(x2, y2, z2);

            dS--;
        }

        dT++;
    }
    glEnd();

    glPopMatrix();
}

void CGLRenderer::DrawAxis(double width, double lineWidth)
{
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    glLineWidth(lineWidth);

    glBegin(GL_LINES);
    {
        glColor3f(1, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(width, 0, 0);

        glColor3f(0, 1, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, width, 0);

        glColor3f(0, 0, 1);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, width);
    }
    glEnd();

    glLineWidth(1);

    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
}
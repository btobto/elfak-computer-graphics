#include "pch.h"
#include "CGLRenderer.h"

#define _USE_MATH_DEFINES
#include <math.h>

CGLRenderer::CGLRenderer()
{
    RotateView(0, 30);
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

    glClearColor(1.0, 1.0, 1.0, 0.0);


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

inline double CGLRenderer::DegToRad(double angle)
{
    return angle * M_PI / 180;
}

void CGLRenderer::RotateArm(double angle)
{
    m_armRotAngle += angle;
}

void CGLRenderer::RotateView(int dX, int dY)
{
    m_XZrotAngle += dX;
    m_XYrotAngle += dY;

    double alphaRad = DegToRad(m_XYrotAngle);
    double betaRad = DegToRad(m_XZrotAngle);

    m_xRot = cos(betaRad) * cos(alphaRad) * m_zoomDistance;
    m_yRot = sin(alphaRad) * m_zoomDistance;
    m_zRot = sin(betaRad) * cos(alphaRad) * m_zoomDistance;

    m_upY = signbit(cos(alphaRad)) ? -1 : 1;
}

void CGLRenderer::Zoom(int val)
{
    m_zoomDistance += val;
    if (m_zoomDistance < 0) m_zoomDistance = 0;
    else if (m_zoomDistance > 20) m_zoomDistance = 20;
    RotateView(0, 0);
}

void CGLRenderer::DrawScene(CDC* pDC)
{
    wglMakeCurrent(pDC->m_hDC, m_hrc);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(m_xRot, m_yRot, m_zRot, 0, 4, 0, 0, m_upY, 0);
    DrawGrid(10, 10, 10, 10);
    DrawAxis(15);
    DrawFigure(m_armRotAngle);

    glFlush();

    SwapBuffers(pDC->m_hDC);
    wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawSphere(double r, int nSegAlpha, int nSegBeta)
{
    double stepA = 180.0 / nSegAlpha;
    double stepB = 360.0 / nSegBeta;

    glBegin(GL_QUAD_STRIP);
    for (double a = 0; a < 180; a += stepA) {
        double alpha1 = DegToRad(90 - a);
        double alpha2 = DegToRad(90 - a - stepA);
        double y1 = r * sin(alpha1);
        double y2 = r * sin(alpha2);
        
        for (double b = 0; b <= 360; b += stepB) {
            double beta = DegToRad(b);
            double x1 = r * cos(alpha1) * cos(beta);
            double z1 = r * cos(alpha1) * sin(beta);

            double x2 = r * cos(alpha2) * cos(beta);
            double z2 = r * cos(alpha2) * sin(beta);

            glVertex3f(x1, y1, z1);
            glVertex3f(x2, y2, z2);
        }
    }
    glEnd();
}

void CGLRenderer::DrawCylinder(double h, double r1, double r2, int nSeg)
{
    double step = 360.0 / nSeg;

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, h, 0);
    for (double alpha = 0; alpha <= 360; alpha += step) {
        double x = r1 * cos(DegToRad(alpha));
        double z = r1 * sin(DegToRad(alpha));
        glVertex3f(x, h ,z);
    }
    glEnd();

    glBegin(GL_QUAD_STRIP);
    for (double alpha = 0; alpha <= 360; alpha += step) {
        double x1 = r1 * cos(DegToRad(alpha));
        double z1 = r1 * sin(DegToRad(alpha));
        double x2 = r2 * cos(DegToRad(alpha));
        double z2 = r2 * sin(DegToRad(alpha));
        glVertex3f(x1, h, z1);
        glVertex3f(x2, 0, z2);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, 0);
    for (double alpha = 0; alpha <= 360; alpha += step) {
        double x = r2 * cos(DegToRad(alpha));
        double z = r2 * sin(DegToRad(alpha));
        glVertex3f(x, 0, z);
    }
    glEnd();
}

void CGLRenderer::DrawCone(double h, double r, int nSeg)
{
    //DrawCylinder(h, 0, r, nSeg);

    double step = 360 / nSeg;

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, h, 0);
    for (double alpha = 0; alpha <= 360; alpha += step) {
        double x = r * cos(DegToRad(alpha));
        double z = r * sin(DegToRad(alpha));
        glVertex3f(x, 0, z);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, 0);
    for (double alpha = 0; alpha <= 360; alpha += step) {
        double x = r * cos(DegToRad(alpha));
        double z = r * sin(DegToRad(alpha));
        glVertex3f(x, 0, z);
    }
    glEnd();
}

void CGLRenderer::DrawFigure(double angle)
{
    // cone glColor3f(0.00, 0.80, 0.00);
    // ball glColor3f(0.50, 0.75, 1.00);

    double vaseBottomH = 1.4;
    double vaseTopH = 0.70;
    double sphereR = 0.33;
    double cylR = 0.5;

    glPushMatrix();
    {
        // vase
        glColor3f(0.47, 0.00, 0.00);
        DrawCylinder(vaseBottomH, 1.25, 1, 8);
        glTranslated(0, vaseBottomH, 0);
        DrawCylinder(vaseTopH, 1.6, 1.6, 8);

        // first cyl
        glTranslated(0, vaseTopH, 0);
        glColor3f(0.00, 0.80, 0.00);
        DrawCylinder(vaseBottomH, cylR, cylR, 8);

        // first ball
        glColor3f(0.50, 0.75, 1.00);
        glTranslated(0, vaseBottomH + sphereR, 0);
        DrawSphere(sphereR, 10, 28);

        // middle rotating branch
        glPushMatrix();
        {
            // yellow cone
            glRotated(angle, 1, 0, 0);
            glTranslated(0, sphereR, 0);
            glColor3f(1, 1, 0.00);
            DrawCone(vaseBottomH, cylR, 8);

            // ball above
            glTranslated(0, vaseBottomH + sphereR, 0);
            glColor3f(0.50, 0.75, 1.00);
            DrawSphere(sphereR, 10, 28);
            
            // left branch
            glPushMatrix();
            {
                glRotated(45, 1, 0, 0);
                glTranslated(0, sphereR, 0);
                glColor3f(0.00, 0.80, 0.00);
                DrawCylinder(vaseBottomH, cylR, cylR, 8);

                glTranslated(0, vaseBottomH + sphereR, 0);
                glColor3f(0.50, 0.75, 1.00);
                DrawSphere(sphereR, 10, 28);
            }
            glPopMatrix();

            // right branch
            glPushMatrix();
            {
                glRotated(-45, 1, 0, 0);
                glTranslated(0, sphereR, 0);
                glColor3f(0.00, 0.80, 0.00);
                DrawCylinder(vaseBottomH, cylR, cylR, 8);

                glTranslated(0, vaseBottomH + sphereR, 0);
                glColor3f(0.50, 0.75, 1.00);
                DrawSphere(sphereR, 10, 28);

                glTranslated(0, sphereR, 0);
                glColor3f(0.00, 0.80, 0.00);
                DrawCylinder(vaseBottomH, cylR, cylR, 8);

                glTranslated(0, vaseBottomH + sphereR, 0);
                glColor3f(0.50, 0.75, 1.00);
                DrawSphere(sphereR, 10, 28);
            }
            glPopMatrix();

        }
        glPopMatrix();

        // right branch
        glPushMatrix();
        {
            glRotated(-45, 1, 0, 0);
            glTranslated(0, sphereR, 0);
            glColor3f(0.00, 0.80, 0.00);
            DrawCylinder(vaseBottomH, cylR, cylR, 8);

            glTranslated(0, vaseBottomH + sphereR, 0);
            glColor3f(0.50, 0.75, 1.00);
            DrawSphere(sphereR, 10, 28);
        }
        glPopMatrix();

        // left branch
        glPushMatrix();
        {
            // cone
            glRotated(45, 1, 0, 0);
            glTranslated(0, sphereR, 0);
            glColor3f(0.00, 0.80, 0.00);
            DrawCone(vaseBottomH, cylR, 8);

            glTranslated(0, vaseBottomH + sphereR, 0);
            glColor3f(0.50, 0.75, 1.00);
            DrawSphere(sphereR, 10, 28);

            // left branch
            glPushMatrix();
            {
                glRotated(45, 1, 0, 0);
                glTranslated(0, sphereR, 0);
                glColor3f(0.00, 0.80, 0.00);
                DrawCone(vaseBottomH, cylR, 8);

                glTranslated(0, vaseBottomH + sphereR, 0);
                glColor3f(0.50, 0.75, 1.00);
                DrawSphere(sphereR, 10, 28);
                glTranslated(0, sphereR, 0);

                glColor3f(0.00, 0.80, 0.00);
                DrawCylinder(vaseBottomH, cylR, cylR, 8);

                glTranslated(0, vaseBottomH + sphereR, 0);
                glColor3f(0.50, 0.75, 1.00);
                DrawSphere(sphereR, 10, 28);
            }
            glPopMatrix();


            // right branch
            glPushMatrix();
            {
                glRotated(-45, 1, 0, 0);
                glTranslated(0, sphereR, 0);
                glColor3f(0.00, 0.80, 0.00);
                DrawCylinder(vaseBottomH, cylR, cylR, 8);

                glTranslated(0, vaseBottomH + sphereR, 0);
                glColor3f(0.50, 0.75, 1.00);
                DrawSphere(sphereR, 10, 28);

                glTranslated(0, sphereR, 0);
                glColor3f(0.00, 0.80, 0.00);
                DrawCone(vaseBottomH, cylR, 8);

                glTranslated(0, vaseBottomH + sphereR, 0);
                glColor3f(0.50, 0.75, 1.00);
                DrawSphere(sphereR, 10, 28);
            }
            glPopMatrix();
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void CGLRenderer::DrawAxis(double width)
{   
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
}

void CGLRenderer::DrawGrid(double width, double height, int nSegW, int nSegH)
{
    float x = width / 2;
    float z = height / 2;

    float segWidth = width / nSegW;
    float segHeight = height / nSegH;

    glBegin(GL_LINES);
    {
        glColor3f(1.0, 0.6, 0.0);

        for (int i = -z; i <= z; i += segHeight) {
            glVertex3f(-x, 0, i);
            glVertex3f(x, 0, i);
        }

        for (int i = -x; i <= x; i += segWidth) {
            glVertex3f(i, 0, -z);
            glVertex3f(i, 0, z);
        }
    }
    glEnd();
}

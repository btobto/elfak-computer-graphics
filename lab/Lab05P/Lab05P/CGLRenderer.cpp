#include "pch.h"
#include "CGLRenderer.h"

CGLRenderer::CGLRenderer()
{
    // 1.00, 0.00, 1.00
    // 0.00, 1.00, 1.00

    m_vaseMat1.SetAmbient(0.5, 0, 0.5, 1.0);
    m_vaseMat1.SetDiffuse(0.8, 0.00, 0.8, 1.0);
    m_vaseMat1.SetSpecular(0.5, 0, 0.5, 1.0);

    m_vaseMat2.SetAmbient(0.00, 0.5, 0.5, 1.0);
    m_vaseMat2.SetDiffuse(0.00, 0.8, 0.8, 1.0);
    m_vaseMat2.SetSpecular(0, 0.5, 0.5, 1.0);

    m_wallMat.SetAmbient(0.5, 0.5, 0.5, 1);
    m_wallMat.SetDiffuse(0.6, 0.6, 0.6, 1);
    m_wallMat.SetSpecular(0, 0, 0, 1);

    m_blueLightMat.SetEmission(0, 0, 1, 1);
    m_redLightMat.SetEmission(1, 0, 0, 1);
    m_purpleLightMat.SetEmission(1, 0, 1, 1);

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

    glClearColor(1.0, 1.0, 1.0, 0.0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    float lmAmbient[] = { 0.15, 0.15, 0.15, 1.0 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmAmbient);
    //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    GLfloat lightAmbient[] = { 0.3, 0.3, 0.3, 1.0 };

    GLfloat directionalDiffuse[] = { 0.8, 0.8, 0.8, 0 };
    GLfloat directionalSpecular[] = { 0.2, 0.2, 0.2, 0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, directionalDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, directionalSpecular);

    GLfloat blueDiffuse[] = { 0, 0, 1, 1 };
    GLfloat blueSpecular[] = { 0, 0, 0.4, 1 };
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, blueDiffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, blueSpecular);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25);

    GLfloat redDiffuse[] = { 1, 0, 0, 1 };
    GLfloat redSpecular[] = { 0.4, 0, 0, 1 };
    glLightfv(GL_LIGHT2, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, redDiffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, redSpecular);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 25);

    GLfloat purpleDiffuse[] = { 1, 0, 1, 1 };
    GLfloat purpleSpecular[] = { 0.4, 0, 0.4, 1 };
    glLightfv(GL_LIGHT3, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, purpleDiffuse);
    glLightfv(GL_LIGHT3, GL_SPECULAR, purpleSpecular);
    glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 25);

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
    gluPerspective(60, (double)w / (double)h, 1, 150);
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

void CGLRenderer::DrawScene(CDC* pDC)
{
    wglMakeCurrent(pDC->m_hDC, m_hrc);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(m_xRot, m_yRot, m_zRot, 0, 0, 0, 0, 1, 0);

    GLfloat position[] = { 0.3, 0.5, 0.9, 0 };
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glEnable(GL_LIGHT0);

    glPushMatrix();
    {
        glTranslated(0, -20, 0);

        DrawLights();
        DrawAxis(30);
        DrawRoom();
        DrawBase();
        DrawVase();
    }
    glPopMatrix();

    glFlush();

    SwapBuffers(pDC->m_hDC);
    wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawAxis(double width, double lineWidth)
{
    glDisable(GL_LIGHTING);

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
}

void CGLRenderer::DrawRoom()
{
    m_wallMat.Select();

    for (int i = 90; i >= -180; i -= 90) {
        glPushMatrix();
        {
            glRotated(i, 0, 1, 0);
            glTranslated(-BOX_SIDE / 2, 0, -BOX_SIDE / 2);
            DrawRectangle(BOX_SIDE, BOX_SIDE, 100, 100);
        }
        glPopMatrix();
    }

    glPushMatrix();
    {
        glRotated(90, 1, 0, 0);
        glTranslated(-BOX_SIDE / 2, -BOX_SIDE / 2, -BOX_SIDE);
        DrawRectangle(BOX_SIDE, BOX_SIDE, 100, 100);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glRotated(-90, 1, 0, 0);
        glTranslated(-BOX_SIDE / 2, -BOX_SIDE / 2, 0);
        DrawRectangle(BOX_SIDE, BOX_SIDE, 100, 100);
    }
    glPopMatrix();
}

void CGLRenderer::DrawLight(GLenum light, bool on, CGLMaterial& mat, int angle, GLfloat d[], GLfloat pos[], GLfloat dir[])
{
    if (on) {
        mat.Select();

        glPushMatrix();
        {
            glRotatef(angle, 1, 0, 0);
            glTranslatef(d[0], d[1], d[2]);
            DrawHalfSphere(3, 10, 28);
        }
        glPopMatrix();

        glLightfv(light, GL_POSITION, pos);
        glLightfv(light, GL_SPOT_DIRECTION, dir);
        
        glEnable(light);
    }
    else {
        glDisable(light);
    }
}

void CGLRenderer::DrawLights()
{
    GLfloat d1[] = { 0, -BOX_SIDE, 0 };
    GLfloat pos1[] = { 0, BOX_SIDE, 0, 1 };
    GLfloat dir1[] = { 0, -1, 0 };
    DrawLight(GL_LIGHT1, m_blueLightOn, m_blueLightMat, 180, d1, pos1, dir1);

    GLfloat d2[] = { 0, -BOX_SIDE / 2, -BOX_SIDE / 2 };
    GLfloat pos2[] = { 0, BOX_SIDE / 2, -BOX_SIDE / 2, 1 };
    GLfloat dir2[] = { 0, 0, 1 };
    DrawLight(GL_LIGHT2, m_redLightOn, m_redLightMat, 90, d2, pos2, dir2);

    GLfloat d3[] = { 0, -BOX_SIDE / 2, BOX_SIDE / 2 };
    GLfloat pos3[] = { 0, BOX_SIDE / 2, BOX_SIDE / 2, 1 };
    GLfloat dir3[] = { 0, 0, -1 };
    DrawLight(GL_LIGHT3, m_purpleLightOn, m_purpleLightMat, -90, d3, pos3, dir3);
}

void CGLRenderer::DrawBase()
{
    glPushMatrix();
    {
        m_wallMat.Select();
        DrawHalfSphere(5, 10, 28);
        DrawCylinder(9, 2.5, 2.5, 8);

        glTranslated(0, 9, 0);
        DrawCuboid(3, 11, 11, 1, 1, 1);
    }
    glPopMatrix();
}

void CGLRenderer::DrawVase()
{
    // 1.00, 0.00, 1.00
    // 0.00, 1.00, 1.00

    double h = 1;
    double r1 = 2, r2 = 3.25, r3 = 4.5;

    glPushMatrix();
    {
        glTranslated(0, 12, 0);

        DrawPart(h, r1, r2, r3);
        DrawPart(h, r1, r1, r1);
        DrawPart(h, r1, r2, r1);
        DrawPart(h, r3, r2, r1);
        DrawPart(h, r1, r2, r3);
        DrawPart(h, r1, r2, r1);
        DrawPart(h, r3, r2, r1);
    }
    glPopMatrix();
}

void CGLRenderer::DrawPart(double h, double r1, double r2, double r3)
{
    m_vaseMat2.Select(GL_FRONT_AND_BACK);
    DrawCylinder(h, r2, r3, 30);
    glTranslated(0, h, 0);

    m_vaseMat1.Select(GL_FRONT_AND_BACK);
    DrawCylinder(h, r1, r2, 30);
    glTranslated(0, h, 0);
}

void CGLRenderer::DrawHalfSphere(double r, int nSegAlpha, int nSegBeta)
{
    double stepA = 180.0 / nSegAlpha;
    double stepB = 360.0 / nSegBeta;

    glBegin(GL_QUAD_STRIP);
    for (double a = 90; a > 0; a -= stepA) {
        double alpha1 = DegToRad(a);
        double alpha2 = DegToRad(a - stepA);

        double ny1 = sin(alpha1);
        double ny2 = sin(alpha2);
        double y1 = r * ny1;
        double y2 = r * ny2;

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
            glVertex3f(x1, y1, z1);

            glNormal3f(nx2, ny2, nz2);
            glVertex3f(x2, y2, z2);
        }
    }
    glEnd();
}

void CGLRenderer::DrawCylinder(double h, double r1, double r2, int nSeg, bool drawBase)
{
    double step = 360.0 / nSeg;

    if (drawBase) {
        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0, 1, 0);
        glVertex3f(0, h, 0);
        for (double alpha = 0; alpha <= 360; alpha += step) {
            double x = r1 * cos(DegToRad(alpha));
            double z = r1 * sin(DegToRad(alpha));
            glVertex3f(x, h, z);
        }
        glEnd();

        glBegin(GL_TRIANGLE_FAN);
        glNormal3d(0, -1, 0);
        glVertex3f(0, 0, 0);
        for (double alpha = 0; alpha <= 360; alpha += step) {
            double x = r2 * cos(DegToRad(alpha));
            double z = r2 * sin(DegToRad(alpha));
            glVertex3f(x, 0, z);
        }
        glEnd();
    }

    double r = (r2 - r1) / 2;
    double L = sqrt(r * r + h * h);
    double ny = r / L;
    double nr = h / L;

    glBegin(GL_QUAD_STRIP);
    for (double alpha = 0; alpha <= 360; alpha += step) {
        double alphaRad = DegToRad(alpha);
        double x1 = r1 * cos(alphaRad);
        double z1 = r1 * sin(alphaRad);
        double x2 = r2 * cos(alphaRad);
        double z2 = r2 * sin(alphaRad);
        double nx = nr * cos(alphaRad);
        double nz = nr * sin(alphaRad);

        glNormal3f(nx, ny, nz);
        glVertex3f(x1, h, z1);
        glVertex3f(x2, 0, z2);
    }
    glEnd();

    if (m_showNormals) {
        glDisable(GL_LIGHTING);

        if (drawBase) {
            // todo
        }

        glBegin(GL_LINES);
        glColor3f(0, 1, 0);
        for (double alpha = 0; alpha <= 360; alpha += step) {
            double alphaRad = DegToRad(alpha);
            double x1 = r1 * cos(alphaRad);
            double z1 = r1 * sin(alphaRad);
            double x2 = r2 * cos(alphaRad);
            double z2 = r2 * sin(alphaRad);
            double nx = nr * cos(alphaRad);
            double nz = nr * sin(alphaRad);

            glVertex3f(x1, h, z1);
            glVertex3f(x1 + nx, h + ny, z1 + nz);

            glVertex3f(x2, 0, z2);
            glVertex3f(x2 + nx, ny, z2 + nz);
        }
        glEnd();

        glEnable(GL_LIGHTING);
    }
}

void CGLRenderer::DrawRectangle(double h, double w, int nSegH, int nSegW)
{
    double segW = w / nSegW;
    double segH = h / nSegH;

    glBegin(GL_QUADS);
    glNormal3d(0, 0, 1);
    for (double i = 0; i < h; i += segH) {
        for (double j = 0; j < w; j += segW) {
            glVertex3f(j, i, 0);
            glVertex3f(j, i + segH, 0);
            glVertex3f(j + segW, i + segH, 0);
            glVertex3f(j + segW, i, 0);
        }
    }
    glEnd();
}

void CGLRenderer::DrawCuboid(double h, double w, double d, int nSegH, int nSegW, int nSegD)
{
    // prednja strana
    glPushMatrix();
    {
        glTranslated(-w / 2, 0, d / 2);
        DrawRectangle(h, w, nSegH, nSegW);
    }
    glPopMatrix();

    // desna strana
    glPushMatrix();
    {
        glRotated(90, 0, 1, 0);
        glTranslated(-d / 2, 0, w / 2);
        DrawRectangle(h, d, nSegH, nSegD);
    }
    glPopMatrix();

    // zadnja strana
    glPushMatrix();
    {
        glRotated(180, 0, 1, 0);
        glTranslated(-w / 2, 0, d / 2);
        DrawRectangle(h, w, nSegH, nSegW);
    }
    glPopMatrix();

    // leva strana
    glPushMatrix();
    {
        glRotated(-90, 0, 1, 0);
        glTranslated(-d / 2, 0, w / 2);
        DrawRectangle(h, d, nSegH, nSegD);
    }
    glPopMatrix();

    // gornja strana
    glPushMatrix();
    {
        glRotated(-90, 1, 0, 0);
        glTranslated(-w / 2, -d / 2, h);
        DrawRectangle(d, w, nSegD, nSegW);
    }
    glPopMatrix();

    // donja strana
    glPushMatrix();
    {
        glRotated(90, 1, 0, 0);
        glTranslated(-w / 2, -d / 2, 0);
        DrawRectangle(d, w, nSegD, nSegW);
    }
    glPopMatrix();
}

void CGLRenderer::DrawCube(double side, int nSeg)
{
    DrawCuboid(side, side, side, nSeg, nSeg, nSeg);
}

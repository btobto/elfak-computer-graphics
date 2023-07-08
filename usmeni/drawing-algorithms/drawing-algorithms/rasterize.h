#pragma once

#include "framework.h"

namespace line
{
	void nagibni(CDC* pDC, POINT p1, POINT p2, COLORREF clr);
	void incremental(CDC* pDC, POINT p1, POINT p2, COLORREF clr);
	void bresenham(CDC* pDC, POINT p1, POINT p2, COLORREF clr);
}

namespace circle
{
	void trigonometric(CDC* pDC, int r, COLORREF clr, POINT c = POINT{0, 0});
	void polynomial(CDC* pDC, int r, COLORREF clr, POINT c = POINT{0, 0});
	void bresenham(CDC* pDC, int r, COLORREF clr, POINT c = POINT{0, 0});
}

namespace ellipse
{

}
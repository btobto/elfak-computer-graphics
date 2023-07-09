#include "pch.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "rasterize.h"

void writePixel8(CDC* pDC, int x, int y, COLORREF clr, POINT p = POINT{ 0, 0 })
{
	pDC->SetPixel(p.x + x, p.y + y, clr);
	pDC->SetPixel(p.x - x, p.y + y, clr);
	pDC->SetPixel(p.x + x, p.y - y, clr);
	pDC->SetPixel(p.x - x, p.y - y, clr);
	pDC->SetPixel(p.x + y, p.y + x, clr);
	pDC->SetPixel(p.x - y, p.y + x, clr);
	pDC->SetPixel(p.x + y, p.y - x, clr);
	pDC->SetPixel(p.x - y, p.y - x, clr);
}

void writePixel4(CDC* pDC, int x, int y, COLORREF clr, POINT p = { 0, 0 })
{
	pDC->SetPixel(p.x + x, p.y + y, clr);
	pDC->SetPixel(p.x - x, p.y + y, clr);
	pDC->SetPixel(p.x + x, p.y - y, clr);
	pDC->SetPixel(p.x - x, p.y - y, clr);
}

namespace line
{
	void nagibni(CDC* pDC, POINT p1, POINT p2, COLORREF clr)
	{
		double dx = p2.x - p1.x;
		double dy = p2.y - p1.y;
		double m = dy / dx;
		double b = p1.y - m * p1.x;
		int start, end;

		if (abs(m) <= 1) 
		{
			if (p1.x > p2.x)
			{
				start = p2.x;
				end = p1.x;
			} 
			else
			{
				start = p1.x;
				end = p2.x;
			}

			for (int x = start; x <= end; x++)
			{
				double y = m * x + b;
				pDC->SetPixel(x, int(y + 0.5), clr);
			}
		}
		else 
		{
			if (p1.y > p2.y)
			{
				start = p2.y;
				end = p1.y;
			}
			else
			{
				start = p1.y;
				end = p2.y;
			}

			for (int y = start; y <= end; y++)
			{
				double x = (y - b) / m;
				pDC->SetPixel(int(x + 0.5), y, clr);
			}
		}
	}	

	void incremental(CDC* pDC, POINT p1, POINT p2, COLORREF clr)
	{
		double dy = p2.y - p1.y;
		double dx = p2.x - p1.x;

		double step = max(abs(dx), abs(dy));
		double xIncr = dx / step, yIncr = dy / step;

		double x = p1.x;
		double y = p1.y;

		for (int i = 0; i < step; i++)
		{
			pDC->SetPixel(int(x + 0.5), int(y + 0.5), clr);
			x += xIncr;
			y += yIncr;
		}
	}

	void bresenham(CDC* pDC, POINT p1, POINT p2, COLORREF clr)
	{
		int dx = abs(p2.x - p1.x);
		int dy = abs(p2.y - p1.y);

		int d, incr1, incr2, incr3;
		POINT start, end;

		if (dx > dy)
		{
			incr1 = 2 * dy;
			incr2 = 2 * (dy - dx);
			d = 2 * dy - dx;

			if (p1.x > p2.x)
			{
				start = p2;
				end = p1;
			}
			else
			{
				start = p1;
				end = p2;
			}

			incr3 = start.y < end.y ? 1 : -1;

			pDC->SetPixel(start.x, start.y, clr);
			while (start.x < end.x)
			{
				start.x++;

				if (d < 0)
				{
					d += incr1;
				}
				else
				{
					d += incr2;
					start.y += incr3;
				}

				pDC->SetPixel(start.x, start.y, clr);
			}
		}
		else
		{
			incr1 = 2 * dx;
			incr2 = 2 * (dx - dy);
			d = 2 * dx - dy;

			if (p1.y > p2.y)
			{
				start = p2;
				end = p1;
			}
			else
			{
				start = p1;
				end = p2;
			}

			incr3 = start.x < end.x ? 1 : -1;

			pDC->SetPixel(start.x, start.y, clr);
			while (start.y < end.y)
			{
				start.y++;

				if (d < 0)
				{
					d += incr1;
				}
				else
				{
					d += incr2;
					start.x += incr3;
				}

				pDC->SetPixel(start.x, start.y, clr);
			}
		}
	}
}

namespace circle
{
	void trigonometric(CDC* pDC, int r, COLORREF clr, POINT c)
	{
		constexpr double twoPi = 2 * M_PI, fourthPi = M_PI / 4;
		double step = twoPi / (7 * r);

		for (double alpha = 0; alpha < fourthPi; alpha += step)
		{
			int x = int(r * cos(alpha) + 0.5);
			int y = int(r * sin(alpha) + 0.5);
			writePixel8(pDC, x, y, clr, c);
		}
	}

	void polynomial(CDC* pDC, int r, COLORREF clr, POINT c)
	{
		int y = r;
		writePixel8(pDC, 0, y, clr, c);
		for (int x = 1; x <= y; x++)
		{
			y = int(sqrt(r * r - x * x) + 0.5);
			writePixel8(pDC, x, y, clr, c);
		}
	}

	void bresenham(CDC* pDC, int r, COLORREF clr, POINT c)
	{
		int x = r, y = 0;
		int d = 3 - 2 * r;

		while (x > y)
		{
			writePixel8(pDC, x, y, clr, c);

			if (d < 0)
			{
				d += 4 * y + 6;
			}
			else
			{
				d += 4 * (y - x) + 10;
				x--;
			}

			y++;
		}

		if (x == y) writePixel8(pDC, x, y, clr, c);
	}
}

namespace ellipse
{
	void trigonometric(CDC* pDC, int a, int b, COLORREF clr, POINT c)
	{
		constexpr double twoPi = M_PI * 2, end = M_PI / 2;
		double step = twoPi / (7 * max(a, b));

		for (double t = 0.0; t <= end; t += step)
		{
			int x = int(a * cos(t) + 0.5);
			int y = int(b * sin(t) + 0.5);
			writePixel4(pDC, x, y, clr, c);
		}
	}

	void polynomial(CDC* pDC, int a, int b, COLORREF clr, POINT c)
	{
		double a2 = a * a, b2 = b * b;
		int x = 0, y = b;

		double boundary = a2 / sqrt(a2 + b2);

		for (x = 0; x <= boundary; x++) {
			y = (int)(b * sqrt(1 - x * x / a2) + 0.5);
			writePixel4(pDC, x, y, clr, c);
		}

		for (; y >= 0; y--) {
			x = (int)(a * sqrt(1 - y * y / b2) + 0.5);
			writePixel4(pDC, x, y, clr, c);
		}
	}

	void differential1(CDC* pDC, int a, int b, COLORREF clr, POINT c)
	{
		constexpr double dphi = M_PI / 180.0;
		double ba = (double)b / a * dphi, ab = (double)a / b * dphi;
		double x0 = a, y0 = 0, x1, y1;

		for (int i = 0; i < 90; i++)
		{
			x1 = x0 - ab * y0;
			y1 = y0 + ba * x0;
			
			line::bresenham(pDC, 
				POINT{ c.x + int(x0 + 0.5), c.y + int(y0 + 0.5) }, 
				POINT{ c.x + int(x1 + 0.5), c.y + int(y1 + 0.5) }, 
				clr);
			line::bresenham(pDC, 
				POINT{ c.x - int(x0 + 0.5), c.y + int(y0 + 0.5) }, 
				POINT{ c.x - int(x1 + 0.5), c.y + int(y1 + 0.5) }, 
				clr);
			line::bresenham(pDC, 
				POINT{ c.x + int(x0 + 0.5), c.y - int(y0 + 0.5) }, 
				POINT{ c.x + int(x1 + 0.5), c.y - int(y1 + 0.5) }, 
				clr);
			line::bresenham(pDC, 
				POINT{ c.x - int(x0 + 0.5), c.y - int(y0 + 0.5) }, 
				POINT{ c.x - int(x1 + 0.5), c.y - int(y1 + 0.5) }, 
				clr);
			
			x0 = x1;
			y0 = y1;
		}
	}

	void differential2(CDC* pDC, int a, int b, COLORREF clr, POINT c)
	{
		constexpr double k = M_PI / 180.0, f = 2 - k * k;
		double x0 = a, y0 = 0, x1 = a * cos(k), y1 = b * sin(k), x2, y2;

		for (int i = 0; i < 90; i++)
		{
			line::bresenham(pDC,
				POINT{ c.x + int(x0 + 0.5), c.y + int(y0 + 0.5) },
				POINT{ c.x + int(x1 + 0.5), c.y + int(y1 + 0.5) },
				clr);
			line::bresenham(pDC,
				POINT{ c.x - int(x0 + 0.5), c.y + int(y0 + 0.5) },
				POINT{ c.x - int(x1 + 0.5), c.y + int(y1 + 0.5) },
				clr);
			line::bresenham(pDC,
				POINT{ c.x + int(x0 + 0.5), c.y - int(y0 + 0.5) },
				POINT{ c.x + int(x1 + 0.5), c.y - int(y1 + 0.5) },
				clr);
			line::bresenham(pDC,
				POINT{ c.x - int(x0 + 0.5), c.y - int(y0 + 0.5) },
				POINT{ c.x - int(x1 + 0.5), c.y - int(y1 + 0.5) },
				clr);

			x2 = f * x1 - x0;
			y2 = f * y1 - y0;
			x0 = x1;
			y0 = y1;
			x1 = x2;
			y1 = y2;
		}
	}
}


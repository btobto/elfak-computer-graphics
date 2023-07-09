#include "pch.h"

#include "fill.h"

void fill::flood4(CDC* pDC, POINT pt, COLORREF oldVal, COLORREF newVal)
{
	if (pDC->GetPixel(pt) == oldVal)
	{
		pDC->SetPixel(pt, newVal);
		fill::flood4(pDC, POINT{ pt.x, pt.y - 1 }, oldVal, newVal);
		fill::flood4(pDC, POINT{ pt.x, pt.y + 1 }, oldVal, newVal);
		fill::flood4(pDC, POINT{ pt.x + 1, pt.y }, oldVal, newVal);
		fill::flood4(pDC, POINT{ pt.x - 1, pt.y }, oldVal, newVal);
	}
}

void fill::flood8(CDC* pDC, POINT pt, COLORREF oldVal, COLORREF newVal)
{
	if (pDC->GetPixel(pt) == oldVal)
	{
		pDC->SetPixel(pt, newVal);
		fill::flood8(pDC, POINT{ pt.x, pt.y - 1 }, oldVal, newVal);
		fill::flood8(pDC, POINT{ pt.x, pt.y + 1 }, oldVal, newVal);
		fill::flood8(pDC, POINT{ pt.x + 1, pt.y }, oldVal, newVal);
		fill::flood8(pDC, POINT{ pt.x - 1, pt.y }, oldVal, newVal);
		fill::flood8(pDC, POINT{ pt.x - 1, pt.y - 1 }, oldVal, newVal);
		fill::flood8(pDC, POINT{ pt.x - 1, pt.y + 1 }, oldVal, newVal);
		fill::flood8(pDC, POINT{ pt.x + 1, pt.y - 1 }, oldVal, newVal);
		fill::flood8(pDC, POINT{ pt.x + 1, pt.y + 1}, oldVal, newVal);
	}
}

void fill::boundary4(CDC* pDC, POINT pt, COLORREF boundaryVal, COLORREF newVal)
{
	COLORREF ptVal = pDC->GetPixel(pt);
	if (ptVal != boundaryVal && ptVal != newVal)
	{
		pDC->SetPixel(pt, newVal);
		fill::boundary4(pDC, POINT{ pt.x, pt.y - 1 }, boundaryVal, newVal);
		fill::boundary4(pDC, POINT{ pt.x, pt.y + 1 }, boundaryVal, newVal);
		fill::boundary4(pDC, POINT{ pt.x + 1, pt.y }, boundaryVal, newVal);
		fill::boundary4(pDC, POINT{ pt.x - 1, pt.y }, boundaryVal, newVal);
	}
}

void fill::boundary8(CDC* pDC, POINT pt, COLORREF boundaryVal, COLORREF newVal)
{
	COLORREF ptVal = pDC->GetPixel(pt);
	if (ptVal != boundaryVal && ptVal != newVal)
	{
		pDC->SetPixel(pt, newVal);
		fill::boundary8(pDC, POINT{ pt.x, pt.y - 1 }, boundaryVal, newVal);
		fill::boundary8(pDC, POINT{ pt.x, pt.y + 1 }, boundaryVal, newVal);
		fill::boundary8(pDC, POINT{ pt.x + 1, pt.y }, boundaryVal, newVal);
		fill::boundary8(pDC, POINT{ pt.x - 1, pt.y }, boundaryVal, newVal);
		fill::boundary8(pDC, POINT{ pt.x - 1, pt.y - 1 }, boundaryVal, newVal);
		fill::boundary8(pDC, POINT{ pt.x - 1, pt.y + 1 }, boundaryVal, newVal);
		fill::boundary8(pDC, POINT{ pt.x + 1, pt.y - 1 }, boundaryVal, newVal);
		fill::boundary8(pDC, POINT{ pt.x + 1, pt.y + 1 }, boundaryVal, newVal);
	}
}

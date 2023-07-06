
// Lab01View.cpp : implementation of the CLab01View class
//

#include "pch.h"
#include "framework.h"
#define _USE_MATH_DEFINES
#include <math.h>
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Lab01.h"
#endif

#include "Lab01Doc.h"
#include "Lab01View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLab01View

IMPLEMENT_DYNCREATE(CLab01View, CView)

BEGIN_MESSAGE_MAP(CLab01View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CLab01View construction/destruction

CLab01View::CLab01View() noexcept
{
	// TODO: add construction code here

}

CLab01View::~CLab01View()
{
}

BOOL CLab01View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CLab01View drawing

void CLab01View::DrawPolygon(CDC* pDC, POINT points[], int pointNum, COLORREF fill, int hatch = -1)
{
	CBrush newBrush;
	if (hatch == -1) {
		newBrush.CreateSolidBrush(fill);
	}
	else {
		newBrush.CreateHatchBrush(hatch, fill);
	}
	CBrush* oldBrush = pDC->SelectObject(&newBrush);

	pDC->Polygon(points, pointNum);

	pDC->SelectObject(oldBrush);
}

void CLab01View::DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle)
{
	POINT* points = new POINT[n];
	double radRotAngle = rotAngle * M_PI / 180;

	for (int i = 0; i < n; i++) {
		points[i].x = cx + r * cos(radRotAngle + i * 2 * M_PI / n);
		points[i].y = cy + r * sin(radRotAngle + i * 2 * M_PI / n);
	}

	pDC->Polygon(points, n);
	delete[] points;
}

void CLab01View::DrawGrid(CDC* pDC, COLORREF color)
{
	CPen newPen(PS_SOLID, 0, color);
	CPen* oldPen = pDC->SelectObject(&newPen);

	for (int i = 0; i < m_squareNum + 1; i++) {
		int pos = m_squareSize * i;

		// horizontal
		pDC->MoveTo(0, pos);
		pDC->LineTo(m_areaDim, pos);

		// vertical
		pDC->MoveTo(pos, 0);
		pDC->LineTo(pos, m_areaDim);
	}

	pDC->SelectObject(oldPen);
}

void CLab01View::OnDraw(CDC* pDC)
{
	CLab01Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// background
	CRect bg(0, 0, m_areaDim, m_areaDim);
	CBrush bgBrush(RGB(221, 221, 221));
	pDC->FillRect(bg, &bgBrush);

	CPen newPen(PS_SOLID | PS_GEOMETRIC, 5, RGB(255, 0, 0));
	CPen* oldPen = pDC->SelectObject(&newPen);

	static const int side = m_squareSize * 9;

	// green rectangle
	POINT points[] = { 
		{ m_squareSize + side / 2, m_squareSize }, 
		{ m_squareSize + side, m_squareSize }, 
		{ m_squareSize + side, m_squareSize + side / 2 }, 
		{ m_squareSize + side / 2, m_squareSize + side / 2 } };
	this->DrawPolygon(pDC, points, 4, RGB(50, 205, 50));

	// purple triangle
	POINT points2[] = { 
		{ m_squareSize + side / 2, m_squareSize + side }, 
		{ m_squareSize + side, m_squareSize + side }, 
		{ m_squareSize + side, m_squareSize + side / 2 } };
	this->DrawPolygon(pDC, points2, 3, RGB(156, 4, 208));

	// hatched triangle
	POINT points3[] = { 
		{ m_squareSize, m_squareSize }, 
		{ m_squareSize + side / 2, m_squareSize + side / 2 }, 
		{ m_squareSize, m_squareSize + side } };
	this->DrawPolygon(pDC, points3, 3, RGB(102, 153, 255), HS_CROSS);

	// orange rhomboid
	POINT points4[] = {
		{ m_squareSize + side / 2, m_squareSize + side / 2 },
		{ m_squareSize + side, m_squareSize + side / 2 },
		{ m_squareSize + side / 2, m_squareSize + side },
		{ m_squareSize, m_squareSize + side}
	};
	this->DrawPolygon(pDC, points4, 4, RGB(255, 156, 52));

	// red triangle
	POINT points5[] = {
		{ m_squareSize, m_squareSize },
		{ m_squareSize + side / 2, m_squareSize },
		{ m_squareSize + side / 2, m_squareSize + side / 2 }
	};
	this->DrawPolygon(pDC, points5, 3, RGB(255, 0, 0));

	// pink triangle
	POINT points6[] = {
		{ m_squareSize + side, m_squareSize + side },
		{ m_squareSize + side * 2, m_squareSize + side },
		{ m_squareSize + side * 2, m_squareSize + side * 2 }
	};
	this->DrawPolygon(pDC, points6, 3, RGB(255, 156, 204));

	// yellow triangle
	POINT points7[] = {
		{ m_squareSize + side, m_squareSize + side },
		{ m_squareSize + side * 2, m_squareSize + side * 2 },
		{ m_squareSize + side, m_squareSize + side * 2 }
	};
	this->DrawPolygon(pDC, points7, 3, RGB(255, 252, 4));

	CPen polyPen(PS_SOLID | PS_GEOMETRIC, 3, RGB(255, 0, 0));
	pDC->SelectObject(&polyPen);

	//CBrush polyBrush;
	//LOGBRUSH lpStyle = { BS_HOLLOW };
	//polyBrush.CreateBrushIndirect(&lpStyle);
	//CBrush* oldBrush = pDC->SelectObject(&polyBrush);
	CBrush* oldBrush = (CBrush*)pDC->SelectStockObject(HOLLOW_BRUSH);

	// hatched area
	this->DrawRegularPolygon(pDC, m_squareSize + side / 5, m_squareSize + side / 2, m_squareSize, 4, 0);

	// purple area
	this->DrawRegularPolygon(pDC, m_squareSize + side - side / 7, m_squareSize + side - side / 7, m_squareSize * 0.75, 6, 0);

	// yellow area
	this->DrawRegularPolygon(pDC, m_squareSize * 3.5 + side, side * 2 - 1.5 * m_squareSize, m_squareSize * 1.4, 5, 0);

	// pink area
	this->DrawRegularPolygon(pDC, side * 2 - 1.5 * m_squareSize, m_squareSize * 3.5 + side, m_squareSize * 1.4, 7, 0);

	if (m_gridEnabled) this->DrawGrid(pDC, RGB(255, 255, 255));

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}


// CLab01View printing

BOOL CLab01View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLab01View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLab01View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CLab01View diagnostics

#ifdef _DEBUG
void CLab01View::AssertValid() const
{
	CView::AssertValid();
}

void CLab01View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLab01Doc* CLab01View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLab01Doc)));
	return (CLab01Doc*)m_pDocument;
}
#endif //_DEBUG


// CLab01View message handlers


void CLab01View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_SPACE) {
		this->m_gridEnabled = !this->m_gridEnabled;
		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

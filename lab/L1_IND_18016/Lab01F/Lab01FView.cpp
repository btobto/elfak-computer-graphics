
// Lab01FView.cpp : implementation of the CLab01FView class
//

#include "pch.h"
#include "framework.h"
#define _USE_MATH_DEFINES
#include <math.h>
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Lab01F.h"
#endif

#include "Lab01FDoc.h"
#include "Lab01FView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLab01FView

IMPLEMENT_DYNCREATE(CLab01FView, CView)

BEGIN_MESSAGE_MAP(CLab01FView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CLab01FView construction/destruction

CLab01FView::CLab01FView() noexcept
{
	// TODO: add construction code here

}

CLab01FView::~CLab01FView()
{
}

BOOL CLab01FView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CLab01FView drawing

void CLab01FView::DrawPolygon(CDC* pDC, POINT points[], int pointNum, COLORREF fill, int hatch = -1)
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

void CLab01FView::DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle)
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

void CLab01FView::DrawGrid(CDC* pDC, COLORREF color)
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

void CLab01FView::OnDraw(CDC* pDC)
{
	CLab01FDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	CRect bg(0, 0, m_areaDim, m_areaDim);
	CBrush bgBrush(RGB(221, 221, 221));
	pDC->FillRect(bg, &bgBrush);

	CPen newPen(PS_SOLID | PS_GEOMETRIC, 5, RGB(0, 255, 0));
	CPen* oldPen = pDC->SelectObject(&newPen);

	// green triangle
	POINT points1[] = {
		{ 14.5 * m_squareSize, m_squareSize },
		{ 10 * m_squareSize, 5.5 * m_squareSize },
		{ 14.5 * m_squareSize, 10 * m_squareSize }
	};
	DrawPolygon(pDC, points1, 3, RGB(50, 205, 50));

	// orange triangle
	POINT points2[] = {
		{ 10 * m_squareSize, 5.5 * m_squareSize },
		{ 10 * m_squareSize, 10 * m_squareSize },
		{ m_squareSize * 5.5 + 4.5 * m_squareSize / 2, 5.5 * m_squareSize + 4.5 * m_squareSize / 2 }
	};
	DrawPolygon(pDC, points2, 3, RGB(255, 153, 51));

	// yellow square
	POINT points3[] = {
		{ 5.5 * m_squareSize, 10 * m_squareSize },
		{ m_squareSize * 5.5 + 4.5 * m_squareSize / 2, 5.5 * m_squareSize + 4.5 * m_squareSize / 2 },
		{ 10 * m_squareSize, 10 * m_squareSize },
		{ m_squareSize * 5.5 + 4.5 * m_squareSize / 2, 10 * m_squareSize + 4.5 * m_squareSize / 2 }
	};
	DrawPolygon(pDC, points3, 4, RGB(255, 252, 4));

	// hatched triangle
	POINT points4[] = {
		{ 10 * m_squareSize, 10 * m_squareSize },
		{ 14.5 * m_squareSize, 10 * m_squareSize },
		{ 10 * m_squareSize, 5.5 * m_squareSize }
	};
	DrawPolygon(pDC, points4, 3, RGB(102, 153, 255, 255), HS_FDIAGONAL);

	// red triangle
	POINT points5[] = {
		{ 14.5 * m_squareSize, 10 * m_squareSize },
		{ 14.5 * m_squareSize, 19 * m_squareSize },
		{ 10 * m_squareSize, 14.5 * m_squareSize }
	};
	DrawPolygon(pDC, points5, 3, RGB(255, 4, 4));

	// pink rhomboid
	POINT points6[] = {
		{ 10 * m_squareSize, 10 * m_squareSize },
		{ 14.5 * m_squareSize, 10 * m_squareSize },
		{ m_squareSize * 10 + 4.5 * m_squareSize / 2, 10 * m_squareSize + 4.5 * m_squareSize / 2 },
		{ m_squareSize * 5.5 + 4.5 * m_squareSize / 2, 10 * m_squareSize + 4.5 * m_squareSize / 2 },
	};
	DrawPolygon(pDC, points6, 4, RGB(255, 156, 204));

	// purple triangle
	POINT points7[] = {
		{ m_squareSize * 5.5 + 4.5 * m_squareSize / 2, 10 * m_squareSize + 4.5 * m_squareSize / 2 },
		{ m_squareSize * 10 + 4.5 * m_squareSize / 2, 10 * m_squareSize + 4.5 * m_squareSize / 2 },
		{ 10 * m_squareSize, 14.5 * m_squareSize }
	};
	DrawPolygon(pDC, points7, 3, RGB(153, 0, 204));

	CPen polyPen(PS_SOLID | PS_GEOMETRIC, 3, RGB(0, 255, 0));
	pDC->SelectObject(&polyPen);
	CBrush* oldBrush = (CBrush*)pDC->SelectStockObject(HOLLOW_BRUSH);

	// green area polygon
	DrawRegularPolygon(pDC, 12.5 * m_squareSize, 5.5 * m_squareSize, m_squareSize, 5, 0);

	// hatched area polygon
	DrawRegularPolygon(pDC, 11.25 * m_squareSize, 8.7 * m_squareSize, 0.7 * m_squareSize, 6, 0);

	// orange area polygon
	DrawRegularPolygon(pDC, 9 * m_squareSize, 7.8 * m_squareSize, m_squareSize / 2, 8, 0);

	// red area polygon
	DrawRegularPolygon(pDC, 12.5 * m_squareSize, 14.5 * m_squareSize, m_squareSize, 4, 0);

	// purple area polygon
	DrawRegularPolygon(pDC, 10 * m_squareSize, 13.2 * m_squareSize, 0.5 * m_squareSize, 7, 0);

	if (m_gridEnabled) this->DrawGrid(pDC, RGB(255, 255, 255));

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}


// CLab01FView printing

BOOL CLab01FView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLab01FView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLab01FView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CLab01FView diagnostics

#ifdef _DEBUG
void CLab01FView::AssertValid() const
{
	CView::AssertValid();
}

void CLab01FView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLab01FDoc* CLab01FView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLab01FDoc)));
	return (CLab01FDoc*)m_pDocument;
}
#endif //_DEBUG


// CLab01FView message handlers


void CLab01FView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_SPACE) {
		this->m_gridEnabled = !this->m_gridEnabled;
		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

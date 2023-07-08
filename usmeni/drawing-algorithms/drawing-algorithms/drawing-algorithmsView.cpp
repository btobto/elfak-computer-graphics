
// drawing-algorithmsView.cpp : implementation of the CdrawingalgorithmsView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "drawing-algorithms.h"
#endif

#include "drawing-algorithmsDoc.h"
#include "drawing-algorithmsView.h"

#include "rasterize.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CdrawingalgorithmsView

IMPLEMENT_DYNCREATE(CdrawingalgorithmsView, CView)

BEGIN_MESSAGE_MAP(CdrawingalgorithmsView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CdrawingalgorithmsView construction/destruction

CdrawingalgorithmsView::CdrawingalgorithmsView() noexcept
{
	// TODO: add construction code here

}

CdrawingalgorithmsView::~CdrawingalgorithmsView()
{
}

BOOL CdrawingalgorithmsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CdrawingalgorithmsView drawing

void CdrawingalgorithmsView::translate(CDC* pDC, float dx, float dy, bool isRightMultiply)
{
	XFORM m = {
		1, 0,
		0, 1,
		dx, dy
	};
	pDC->ModifyWorldTransform(&m, isRightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CdrawingalgorithmsView::scale(CDC* pDC, float sx, float sy, bool isRightMultiply)
{
	XFORM m = {
		sx, 0,
		0, sy,
		0, 0
	};
	pDC->ModifyWorldTransform(&m, isRightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CdrawingalgorithmsView::drawGrid(CDC* pDC, int tileNum, int tileLen, COLORREF clr = RGB(200, 200, 200))
{
	POINT currPos = pDC->GetCurrentPosition();

	CPen pen(PS_SOLID, 0, clr);
	CPen* oldPen = pDC->SelectObject(&pen);

	int lineLen = tileNum * tileLen;
	for (int i = 0; i <= tileNum; i++)
	{
		int pos = i * tileLen;

		pDC->MoveTo(0, pos);
		pDC->LineTo(lineLen, pos);

		pDC->MoveTo(pos, 0);
		pDC->LineTo(pos, lineLen);
	}

	pDC->SelectObject(oldPen);
	pDC->MoveTo(currPos);
}

void CdrawingalgorithmsView::drawAxes(CDC* pDC, int axisLen = 50)
{
	constexpr int lineWidth = 2;

	POINT currPos = pDC->GetCurrentPosition();
	CPen* oldPen = pDC->GetCurrentPen();

	CPen xPen(PS_SOLID, lineWidth, RGB(255, 0, 0));
	pDC->SelectObject(&xPen);
	pDC->LineTo(currPos.x + axisLen, currPos.y);
	
	pDC->MoveTo(currPos.x, currPos.y);

	CPen yPen(PS_SOLID, lineWidth, RGB(0, 0, 255));
	pDC->SelectObject(&yPen);
	pDC->LineTo(currPos.x, currPos.y + axisLen);

	pDC->MoveTo(currPos);
	pDC->SelectObject(oldPen);
}

void CdrawingalgorithmsView::drawLines(CDC* pDC)
{
	XFORM old;
	pDC->GetWorldTransform(&old);

	POINT pt1{ 5 ,5 }, pt2{ 40, 20 }, pt3{ 10, 30 }, 
		  pt4{ -10, 30 }, pt5{ -10, 20 }, pt6{ 20, -10 },
		  pt7{ 30, -20 }, pt8{ 10, 10 };
	COLORREF red = RGB(255, 0, 0), green = RGB(0, 255, 0), blue = RGB(0, 0, 255);
	int dx = pt2.x + 10, dy = pt2.y + 10;

	// nagibni
	
	line::nagibni(pDC, pt1, pt2, red);
	translate(pDC, dx, 0);

	line::nagibni(pDC, pt1, pt3, red);
	translate(pDC, dx, 0);

	drawAxes(pDC);
	line::nagibni(pDC, pt1, pt4, red);
	translate(pDC, dx, 15);

	drawAxes(pDC);
	line::nagibni(pDC, pt5, pt6, red);
	translate(pDC, dx + 5, 10);

	drawAxes(pDC);
	line::nagibni(pDC, pt7, pt8, red);
	translate(pDC, dx, -25);

	// incremental

	line::incremental(pDC, pt1, pt2, green);
	translate(pDC, dx, 0);

	line::incremental(pDC, pt1, pt3, green);
	translate(pDC, dx, 0);

	drawAxes(pDC);
	line::incremental(pDC, pt1, pt4, green);
	translate(pDC, dx, 15);

	drawAxes(pDC);
	line::incremental(pDC, pt5, pt6, green);
	translate(pDC, dx + 5, 10);

	drawAxes(pDC);
	line::incremental(pDC, pt7, pt8, green);
	translate(pDC, dx, -25);

	// bresenham

	line::bresenham(pDC, pt1, pt2, blue);
	translate(pDC, dx, 0);

	line::bresenham(pDC, pt1, pt3, blue);
	translate(pDC, dx, 0);

	drawAxes(pDC);
	line::bresenham(pDC, pt1, pt4, blue);
	translate(pDC, dx, 15);

	drawAxes(pDC);
	line::bresenham(pDC, pt5, pt6, blue);
	translate(pDC, dx + 5, 10);

	drawAxes(pDC);
	line::bresenham(pDC, pt7, pt8, blue);
	translate(pDC, dx, -25);

	pDC->SetWorldTransform(&old);
}

void CdrawingalgorithmsView::drawCircles(CDC* pDC)
{
	XFORM old;
	pDC->GetWorldTransform(&old);

	POINT c{ 40, 40 };
	int r = 30;
	int dx = 2 * c.x;

	drawAxes(pDC);
	circle::trigonometric(pDC, r, RGB(255, 0, 0), c);
	translate(pDC, dx, 0);

	circle::polynomial(pDC, r, RGB(0, 255, 0), c);
	translate(pDC, dx, 0);

	circle::bresenham(pDC, r, RGB(0, 0, 255), c);

	pDC->SetWorldTransform(&old);
}

void CdrawingalgorithmsView::drawEllipses(CDC* pDC)
{
	XFORM old;
	pDC->GetWorldTransform(&old);

	drawAxes(pDC);


	pDC->SetWorldTransform(&old);
}

void CdrawingalgorithmsView::OnDraw(CDC* pDC)
{
	CdrawingalgorithmsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	drawGrid(pDC, 100, 10);

	int prevMode = pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM old;
	pDC->GetWorldTransform(&old);

	drawLines(pDC);
	translate(pDC, 0, 100);
	drawCircles(pDC);
	translate(pDC, 0, 100);
	drawEllipses(pDC);

	pDC->SetWorldTransform(&old);
	pDC->SetGraphicsMode(prevMode);
}


// CdrawingalgorithmsView printing

BOOL CdrawingalgorithmsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CdrawingalgorithmsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CdrawingalgorithmsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CdrawingalgorithmsView diagnostics

#ifdef _DEBUG
void CdrawingalgorithmsView::AssertValid() const
{
	CView::AssertValid();
}

void CdrawingalgorithmsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CdrawingalgorithmsDoc* CdrawingalgorithmsView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CdrawingalgorithmsDoc)));
	return (CdrawingalgorithmsDoc*)m_pDocument;
}
#endif //_DEBUG


// CdrawingalgorithmsView message handlers


// Lab02PView.cpp : implementation of the CLab02PView class
//

#include "pch.h"
#include "framework.h"
#define _USE_MATH_DEFINES
#include <math.h>
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Lab02P.h"
#endif

#include "Lab02PDoc.h"
#include "Lab02PView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLab02PView

IMPLEMENT_DYNCREATE(CLab02PView, CView)

BEGIN_MESSAGE_MAP(CLab02PView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CLab02PView construction/destruction

CLab02PView::CLab02PView() noexcept
{
	m_cactusHeightFactor = 3;
	m_thickCactusWidthFactor = 2.5;
	m_mediumCactusWidthFactor = 2;
	m_thinCactusWidthFactor = 1;

	m_cactusMF = GetEnhMetaFile(L"cactus_part.emf");
	m_lightCactusMF = GetEnhMetaFile(L"cactus_part_light.emf");
}

CLab02PView::~CLab02PView()
{
	DeleteEnhMetaFile(m_cactusMF);
	DeleteEnhMetaFile(m_lightCactusMF);
}

BOOL CLab02PView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CLab02PView drawing

void CLab02PView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM m = {
		1, 0,
		0, 1,
		dX, dY
	};

	pDC->ModifyWorldTransform(&m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CLab02PView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM m = {
		sX, 0,
		0, sY,
		0, 0
	};

	pDC->ModifyWorldTransform(&m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CLab02PView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	double radAngle = angle * M_PI / 180;

	XFORM m = {
		cos(radAngle), -sin(radAngle),
		sin(radAngle), cos(radAngle)
	};

	pDC->ModifyWorldTransform(&m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CLab02PView::RotateAroundPoint(CDC* pDC, float x, float y, float angle)
{
	Translate(pDC, x, y, false);
	Rotate(pDC, angle, false);
	Translate(pDC, -x, -y, false);
}

void CLab02PView::DrawPolygon(CDC* pDC, POINT points[], int pointNum, COLORREF fill)
{
	CBrush newBrush(fill);
	CBrush* oldBrush = pDC->SelectObject(&newBrush);

	pDC->Polygon(points, pointNum);

	pDC->SelectObject(oldBrush);
}

inline void CLab02PView::DrawCircle(CDC* pDC, int x, int y, int radius)
{
	pDC->Ellipse(x - radius, y - radius, x + radius, y + radius);
}

void CLab02PView::DrawGrid(CDC* pDC, COLORREF color)
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

void CLab02PView::DrawPot(CDC* pDC)
{
	CBrush potBrush(RGB(222, 148, 0));
	CBrush* oldBrush = pDC->SelectObject(&potBrush);

	POINT bottomPart[] = {
		{ 8.5 * m_squareSize, m_squareNum * m_squareSize },
		{ 11.5 * m_squareSize, m_squareNum * m_squareSize },
		{ 12 * m_squareSize, 18 * m_squareSize },
		{ 8 * m_squareSize, 18 * m_squareSize }
	};
	pDC->Polygon(bottomPart, 4);

	POINT topPart[] = {
		{ 7.6 * m_squareSize, 18 * m_squareSize },
		{ 12.4 * m_squareSize, 18 * m_squareSize },
		{ 12.4 * m_squareSize, 17.25 * m_squareSize },
		{ 7.6 * m_squareSize, 17.25 * m_squareSize }
	};
	pDC->Polygon(topPart, 4);

	pDC->SelectObject(oldBrush);
}

void CLab02PView::DrawCactus(CDC* pDC, float x, float y, float widthFactor, float heightFactor, int rotAngle, const HENHMETAFILE& mf)
{
	XFORM oldXForm;
	pDC->GetWorldTransform(&oldXForm);

	Translate(pDC, x, y, false);
	Rotate(pDC, rotAngle, false);
	Scale(pDC, widthFactor, heightFactor, false);

	POINT pt = { -m_squareSize / 2, -m_squareSize };
	SIZE sz = { m_squareSize, m_squareSize };
	PlayEnhMetaFile(pDC->m_hDC, mf, CRect(pt, sz));

	pDC->SetWorldTransform(&oldXForm);
}	

void CLab02PView::DrawFigure(CDC* pDC)
{
	CPen newPen(PS_SOLID, 0, RGB(0, 0, 0));
	CPen* oldPen = pDC->SelectObject(&newPen);

	CBrush greenBrush(RGB(8, 204, 4));
	CBrush* oldBrush = pDC->SelectObject(&greenBrush);

	//pDC->SetViewportOrg(50, 50);
	//DrawCircle(pDC, 0, 0, m_squareSize / 2);


	int prevMode = pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM xFormInit;
	pDC->GetWorldTransform(&xFormInit);

	DrawCircle(pDC, 10 * m_squareSize, 17 * m_squareSize, m_squareSize / 2);
	
	RotateAroundPoint(pDC, 10 * m_squareSize, 17 * m_squareSize, m_bigCactusRotAngle);

	DrawCactus(
		pDC, 
		10 * m_squareSize, 17 * m_squareSize, 
		m_thickCactusWidthFactor, m_cactusHeightFactor, 
		0, 
		m_lightCactusMF
	);

	XFORM xFormOld;

	// 3 kaktusa
	pDC->GetWorldTransform(&xFormOld);
	DrawCircle(pDC, 10 * m_squareSize, 14 * m_squareSize, m_squareSize / 2);

	Translate(pDC, 10 * m_squareSize, 14 * m_squareSize, false);
	//Rotate(pDC, 45, false);
	Scale(pDC, 1, m_cactusHeightFactor, false);
	POINT pt = { -m_squareSize / 2, -m_squareSize };
	SIZE sz = { m_squareSize, m_squareSize };
	PlayEnhMetaFile(pDC->m_hDC, m_cactusMF, CRect(pt, sz));

	pDC->SetWorldTransform(&xFormOld);

	///////////////////////////////////

	pDC->SetWorldTransform(&xFormInit);
	pDC->SetGraphicsMode(prevMode);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void CLab02PView::OnDraw(CDC* pDC)
{
	CLab02PDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect bg(0, 0, m_areaDim + 1, m_areaDim + 1);
	CBrush bgBrush(RGB(135, 206, 235));
	pDC->FillRect(bg, &bgBrush);

	DrawFigure(pDC);
	DrawPot(pDC);

	if (m_gridEnabled) DrawGrid(pDC, RGB(255, 255, 255));
}


// CLab02PView printing

BOOL CLab02PView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLab02PView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLab02PView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CLab02PView diagnostics

#ifdef _DEBUG
void CLab02PView::AssertValid() const
{
	CView::AssertValid();
}

void CLab02PView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLab02PDoc* CLab02PView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLab02PDoc)));
	return (CLab02PDoc*)m_pDocument;
}
#endif //_DEBUG


// CLab02PView message handlers


void CLab02PView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_SPACE:
		m_gridEnabled = !m_gridEnabled;
		Invalidate();
		break;
	case 'Q':
		m_smallCactusRotAngle += 5;
		Invalidate();
		break;
	case 'W':
		m_smallCactusRotAngle -= 5;
		Invalidate();
		break;
	case 'E':
		m_bigCactusRotAngle += 5;
		Invalidate();
		break;
	case 'R':
		m_bigCactusRotAngle -= 5;
		Invalidate();
		break;
	default:
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

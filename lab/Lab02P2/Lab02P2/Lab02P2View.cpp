
// Lab02P2View.cpp : implementation of the CLab02P2View class
//

#include "pch.h"
#include "framework.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <strsafe.h>
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Lab02P2.h"
#endif

#include "Lab02P2Doc.h"
#include "Lab02P2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define AREA_SIZE 500
#define SQUARE_NUM 20
#define SQUARE_SIZE (AREA_SIZE / SQUARE_NUM)
#define CACT_HEIGHT_F 3
#define BIG_CACT_WIDTH_F 2.5
#define MED_CACT_WIDTH_F 1.8
#define SM_CACT_WIDTH_F 1.0

// CLab02P2View

IMPLEMENT_DYNCREATE(CLab02P2View, CView)

BEGIN_MESSAGE_MAP(CLab02P2View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CLab02P2View construction/destruction

CLab02P2View::CLab02P2View() noexcept
{
	m_cactusMF = GetEnhMetaFile(L"cactus_part.emf");
	m_lightCactusMF = GetEnhMetaFile(L"cactus_part_light.emf");
}

CLab02P2View::~CLab02P2View()
{
	DeleteEnhMetaFile(m_cactusMF);
	DeleteEnhMetaFile(m_lightCactusMF);
}

BOOL CLab02P2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CLab02P2View drawing

void CLab02P2View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM m = {
		1, 0,
		0, 1,
		dX, dY
	};

	pDC->ModifyWorldTransform(&m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CLab02P2View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM m = {
		sX, 0,
		0, sY,
		0, 0
	};

	pDC->ModifyWorldTransform(&m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CLab02P2View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	double radAngle = angle * M_PI / 180;

	XFORM m = {
		cos(radAngle), -sin(radAngle),
		sin(radAngle), cos(radAngle)
	};

	pDC->ModifyWorldTransform(&m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CLab02P2View::RotateAroundPoint(CDC* pDC, float tX, float tY, int angle, bool rightMultiply)
{
	int x = rightMultiply ? tX : -tX;
	int y = rightMultiply ? tY : -tY;

	Translate(pDC, -x, -y, rightMultiply);
	Rotate(pDC, angle, rightMultiply);
	Translate(pDC, x, y, rightMultiply);
}

inline void CLab02P2View::DrawCircle(CDC* pDC, int x, int y, int radius)
{
	pDC->Ellipse(x - radius, y - radius, x + radius, y + radius);
}

void CLab02P2View::DrawGrid(CDC* pDC, COLORREF color)
{
	CPen newPen(PS_SOLID, 0, color);
	CPen* oldPen = pDC->SelectObject(&newPen);

	for (int i = 0; i < SQUARE_NUM + 1; i++) {
		int pos = SQUARE_SIZE * i;

		// horizontal
		pDC->MoveTo(0, pos);
		pDC->LineTo(AREA_SIZE, pos);

		// vertical
		pDC->MoveTo(pos, 0);
		pDC->LineTo(pos, AREA_SIZE);
	}

	pDC->SelectObject(oldPen);
}

void CLab02P2View::DrawPot(CDC* pDC)
{
	CBrush potBrush(RGB(222, 148, 0));
	CBrush* oldBrush = pDC->SelectObject(&potBrush);

	const static POINT bottomPart[] = {
		{ 8.5 * SQUARE_SIZE, SQUARE_NUM * SQUARE_SIZE },
		{ 11.5 * SQUARE_SIZE, SQUARE_NUM * SQUARE_SIZE },
		{ 12 * SQUARE_SIZE, 18 * SQUARE_SIZE },
		{ 8 * SQUARE_SIZE, 18 * SQUARE_SIZE }
	};
	pDC->Polygon(bottomPart, 4);

	const static POINT topPart[] = {
		{ 7.6 * SQUARE_SIZE, 18 * SQUARE_SIZE },
		{ 12.4 * SQUARE_SIZE, 18 * SQUARE_SIZE },
		{ 12.4 * SQUARE_SIZE, 17.25 * SQUARE_SIZE },
		{ 7.6 * SQUARE_SIZE, 17.25 * SQUARE_SIZE }
	};
	pDC->Polygon(topPart, 4);

	pDC->SelectObject(oldBrush);
}

void CLab02P2View::DrawTextWithShadow(CDC* pDC, CString text, POINT location, LOGFONT logFont, COLORREF color, COLORREF shadowColor, int offset)
{
	CFont font;
	font.CreateFontIndirectW(&logFont);
	CFont* oldFont = pDC->SelectObject(&font);
	int oldMode = pDC->SetBkMode(TRANSPARENT);

	COLORREF oldColor = pDC->SetTextColor(shadowColor);
	pDC->TextOutW(location.x + offset, location.y + offset, text);
	pDC->SetTextColor(color);
	pDC->TextOutW(location.x, location.y, text);

	pDC->SetBkMode(oldMode);
	pDC->SetTextColor(oldColor);
	pDC->SelectObject(oldFont);
	font.DeleteObject();
}

void CLab02P2View::DrawCactus(CDC* pDC, float x, float y, float widthFactor, float heightFactor, int rotAngle, const HENHMETAFILE& mf)
{
	XFORM oldXForm;
	pDC->GetWorldTransform(&oldXForm);

	Translate(pDC, x, y, false);
	Rotate(pDC, rotAngle, false);
	Scale(pDC, widthFactor, heightFactor, false);

	POINT pt = { -SQUARE_SIZE / 2, -SQUARE_SIZE };
	SIZE sz = { SQUARE_SIZE, SQUARE_SIZE };
	PlayEnhMetaFile(pDC->m_hDC, mf, CRect(pt, sz));

	pDC->SetWorldTransform(&oldXForm);
}

void CLab02P2View::DrawFigure(CDC* pDC)
{
	CPen newPen(PS_SOLID, 0, RGB(0, 0, 0));
	CPen* oldPen = pDC->SelectObject(&newPen);

	CBrush greenBrush(RGB(8, 204, 4));
	CBrush* oldBrush = pDC->SelectObject(&greenBrush);

	int prevMode = pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM oldXForm;
	pDC->GetWorldTransform(&oldXForm);

	RotateAroundPoint(pDC, 10 * SQUARE_SIZE, 17 * SQUARE_SIZE, m_bigCactusRotAngle, false);

	constexpr float radius = SQUARE_SIZE / 2;

	// prvi light kaktus
	constexpr POINT root = { 10 * SQUARE_SIZE, 17 * SQUARE_SIZE };
	DrawCactus(pDC, root.x, root.y, BIG_CACT_WIDTH_F, CACT_HEIGHT_F, 0, m_lightCactusMF);

	// tri kaktusa
	constexpr POINT threeCactPt = { 10 * SQUARE_SIZE, 14 * SQUARE_SIZE };
	DrawCactus(pDC, threeCactPt.x, threeCactPt.y, SM_CACT_WIDTH_F, CACT_HEIGHT_F, 45, m_cactusMF);
	DrawCactus(pDC, threeCactPt.x, threeCactPt.y, SM_CACT_WIDTH_F, CACT_HEIGHT_F, 0, m_cactusMF);
	DrawCactus(pDC, threeCactPt.x, threeCactPt.y, SM_CACT_WIDTH_F, CACT_HEIGHT_F, -45, m_cactusMF);

	// leva grana
	constexpr POINT leftTreePt = { 7.85 * SQUARE_SIZE, 11.85 * SQUARE_SIZE };
	DrawCactus(pDC, leftTreePt.x, leftTreePt.y, MED_CACT_WIDTH_F, CACT_HEIGHT_F, 90, m_cactusMF);
	DrawCactus(pDC, leftTreePt.x, leftTreePt.y, MED_CACT_WIDTH_F, CACT_HEIGHT_F, 0, m_cactusMF);

	constexpr POINT topCactPt = { 7.85 * SQUARE_SIZE, 8.85 * SQUARE_SIZE };
	DrawCactus(pDC, topCactPt.x, topCactPt.y, BIG_CACT_WIDTH_F, CACT_HEIGHT_F, 0, m_cactusMF);

	// desna grana
	constexpr POINT rightTreePt = { 12.15 * SQUARE_SIZE, 11.85 * SQUARE_SIZE };
	DrawCactus(pDC, rightTreePt.x, rightTreePt.y, MED_CACT_WIDTH_F, CACT_HEIGHT_F, m_smallCactusRotAngle, m_lightCactusMF);
	DrawCactus(pDC, rightTreePt.x, rightTreePt.y, MED_CACT_WIDTH_F, CACT_HEIGHT_F, -90, m_cactusMF);
	
	constexpr POINT rightCactPt = { 15.15 * SQUARE_SIZE, 11.85 * SQUARE_SIZE };
	DrawCactus(pDC, rightCactPt.x, rightCactPt.y, MED_CACT_WIDTH_F, CACT_HEIGHT_F, -135, m_cactusMF);
	DrawCactus(pDC, rightCactPt.x, rightCactPt.y, MED_CACT_WIDTH_F, CACT_HEIGHT_F, -45, m_cactusMF);

	// circles
	DrawCircle(pDC, root.x, root.y, radius);
	DrawCircle(pDC, threeCactPt.x, threeCactPt.y, radius);
	DrawCircle(pDC, leftTreePt.x, leftTreePt.y, radius);
	DrawCircle(pDC, topCactPt.x, topCactPt.y, radius);
	DrawCircle(pDC, rightTreePt.x, rightTreePt.y, radius);
	DrawCircle(pDC, rightCactPt.x, rightCactPt.y, radius);

	pDC->SetWorldTransform(&oldXForm);
	pDC->SetGraphicsMode(prevMode);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void CLab02P2View::OnDraw(CDC* pDC)
{
	CLab02P2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect bg(0, 0, AREA_SIZE + 1, AREA_SIZE + 1);
	CBrush bgBrush(RGB(135, 206, 235));
	pDC->FillRect(bg, &bgBrush);

	DrawFigure(pDC);
	DrawPot(pDC);

	LOGFONT logFont{};
	logFont.lfEscapement = -900;
	logFont.lfHeight = 30;
	logFont.lfCharSet = DEFAULT_CHARSET;

	DrawTextWithShadow(pDC,
		CString("18016 Bozidar Tosic"),
		POINT{ SQUARE_SIZE * 19, SQUARE_SIZE },
		logFont, RGB(255, 255, 0), RGB(0, 0, 0), 2
	);

	if (m_gridEnabled) DrawGrid(pDC, RGB(255, 255, 255));
}


// CLab02P2View printing

BOOL CLab02P2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLab02P2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLab02P2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CLab02P2View diagnostics

#ifdef _DEBUG
void CLab02P2View::AssertValid() const
{
	CView::AssertValid();
}

void CLab02P2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLab02P2Doc* CLab02P2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLab02P2Doc)));
	return (CLab02P2Doc*)m_pDocument;
}
#endif //_DEBUG


// CLab02P2View message handlers


void CLab02P2View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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

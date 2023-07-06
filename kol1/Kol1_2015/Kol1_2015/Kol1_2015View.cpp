
// Kol1_2015View.cpp : implementation of the CKol12015View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kol1_2015.h"
#endif

#include "Kol1_2015Doc.h"
#include "Kol1_2015View.h"

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKol12015View

IMPLEMENT_DYNCREATE(CKol12015View, CView)

BEGIN_MESSAGE_MAP(CKol12015View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CKol12015View construction/destruction

CKol12015View::CKol12015View() noexcept
{	
	m_background.Load(L"blue.png");
}

CKol12015View::~CKol12015View()
{
}

BOOL CKol12015View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKol12015View drawing

void CKol12015View::Mirror(CDC* pDC, bool mX, bool mY, bool isRightMultiply = false)
{
	XFORM m = {
		mX ? -1 : 1, 0,
		0, mY ? -1 : 1,
	};
	pDC->ModifyWorldTransform(&m, isRightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKol12015View::Rotate(CDC* pDC, float angle, bool isRightMultiply = false)
{
	float radAngle = angle * M_PI / 180;
	XFORM m = {
		cos(radAngle), sin(radAngle),
		-sin(radAngle), cos(radAngle)
	};
	pDC->ModifyWorldTransform(&m, isRightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKol12015View::Translate(CDC* pDC, float dX, float dY, bool isRightMultiply = false)
{
	XFORM m = {
		1, 0,
		0, 1,
		dX, dY
	};
	pDC->ModifyWorldTransform(&m, isRightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKol12015View::DrawBackground(CDC* pDC, CRect rect)
{
	m_background.Draw(pDC,
		CRect(0, 0, m_background.Width(), m_background.Height()),
		rect
	);
}

bool CKol12015View::Collision(CRect rect1, CRect rect2)
{
	int r1 = rect1.Width() / 2;
	int r2 = rect2.Width() / 2;

	POINT c1 = { rect1.left + r1, rect1.top + r1 };
	POINT c2 = { rect2.left + r2, rect2.top + r2 };

	return pow(r1 - r2, 2) <= pow(c1.x - c2.x, 2) + pow(c1.y - c2.y, 2) &&
		pow(r1 + r2, 2) >= pow(c1.x - c2.x, 2) + pow(c1.y - c2.y, 2);
}

void CKol12015View::DrawTextAt(CDC* pDC, POINT pt, CString font, int size, CString text, COLORREF color)
{
	CFont f;
	f.CreateFontW(size, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, font);
	CFont* oldF = pDC->SelectObject(&f);
	int oldMode = pDC->SetBkMode(TRANSPARENT);
	UINT prevAlign = pDC->SetTextAlign(TA_CENTER);
	COLORREF oldColor = pDC->SetTextColor(color);

	pDC->TextOutW(pt.x, pt.y, text);

	pDC->SetTextColor(oldColor);
	pDC->SetTextAlign(prevAlign);
	pDC->SetBkMode(oldMode);
	pDC->SelectObject(oldF);
}

void CKol12015View::DrawPacman(CDC* pDC, CRect rect, float angle)
{
	int prevMode = pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM old;
	pDC->GetWorldTransform(&old);

	Translate(pDC, rect.left + rect.Width() / 2, rect.top + rect.Height() / 2);
	Rotate(pDC, m_angle);
	Mirror(pDC, m_mirrored, false);
	Translate(pDC, -(rect.left + rect.Width() / 2), -(rect.top + rect.Height() / 2));

	CBrush yellowBrush(RGB(255, 255, 0));
	CBrush* oldBrush = pDC->SelectObject(&yellowBrush);

	int dY = m_open ? sin(angle * M_PI / 180) * rect.Height() : 0;
	pDC->Pie(rect, 
		POINT { rect.right, rect.bottom - rect.Height() / 2 - dY },
		POINT { rect.right, rect.bottom - rect.Height() / 2 + dY }
	);

	CBrush blackBrush(RGB(0, 0, 0));
	pDC->SelectObject(&blackBrush);
	pDC->Ellipse(CRect(POINT{ rect.left + 60, rect.top + 20 }, SIZE{ 10, 10 }));

	pDC->SelectObject(oldBrush);

	pDC->SetWorldTransform(&old);
	pDC->SetGraphicsMode(prevMode);
}

void CKol12015View::DrawGhost(CDC* pDC, CRect rect)
{
	CBrush redBrush(RGB(255, 0, 0));
	CBrush* oldBrush = pDC->SelectObject(&redBrush);

	int d = rect.Width();
	int dSmall = d / 6;
	int rSmall = dSmall / 2;

	pDC->BeginPath();
	{
		int oldAD = pDC->SetArcDirection(AD_CLOCKWISE);

		pDC->MoveTo(rect.left, rect.top + d / 2);	
		pDC->ArcTo(
			rect,
			POINT{ rect.left, rect.top + d / 2 },
			POINT{ rect.right, rect.top + d / 2 }
		);
		pDC->LineTo(rect.right, rect.bottom);

		pDC->ArcTo(
			CRect(POINT{ rect.right - rSmall, rect.bottom - dSmall }, SIZE{ dSmall, dSmall }),
			POINT{ rect.right, rect.bottom },
			POINT{ rect.right - rSmall, rect.bottom - rSmall }
		);
		for (int i = 0; i < 5; i++) {
			pDC->SetArcDirection(i % 2 == 0 ? AD_COUNTERCLOCKWISE : AD_CLOCKWISE);

			pDC->ArcTo(
				CRect(
					POINT{rect.right - rSmall - (i + 1) * dSmall, rect.bottom - dSmall},
					SIZE{ dSmall, dSmall }
				),
				POINT{rect.right - rSmall - i * dSmall, rect.bottom - rSmall },
				POINT{rect.right - rSmall - (i + 1) * dSmall, rect.bottom - rSmall}
			);
		}
		pDC->SetArcDirection(AD_CLOCKWISE);
		pDC->ArcTo(
			CRect(POINT{ rect.left - rSmall, rect.bottom - dSmall }, SIZE{ dSmall, dSmall }),
			POINT{ rect.left + rSmall, rect.bottom - rSmall },
			POINT{ rect.left, rect.bottom }
		);

		pDC->LineTo(rect.left, rect.bottom - rect.Height() / 2);

		pDC->SetArcDirection(oldAD);
	}
	pDC->EndPath();
	pDC->StrokeAndFillPath();

	CBrush whiteBrush(RGB(255, 255, 255));
	pDC->SelectObject(&whiteBrush);


	pDC->SelectObject(oldBrush);
}

void CKol12015View::OnDraw(CDC* pDC)
{
	CKol12015Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect client;
	GetClientRect(&client);

	CBitmap memBM;
	memBM.CreateCompatibleBitmap(pDC, client.Width(), client.Height());

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(&memBM);

	CRect pacmanRect = CRect(POINT{ m_x, m_y }, SIZE{ 100, 100 });
	CRect ghostRect = CRect(100, 120, 196, 216);
	DrawBackground(&memDC, client);
	DrawPacman(&memDC, pacmanRect, 15);
	DrawGhost(&memDC, ghostRect);
	if (Collision(pacmanRect, ghostRect)) {
		DrawTextAt(&memDC,
			POINT{ client.left + client.Width() / 2, client.top + client.Height() / 2 - 72 / 2 },
			L"Arial",
			72,
			L"The End",
			RGB(255, 255, 255)
		);	
	}

	pDC->BitBlt(0, 0, client.Width(), client.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.DeleteDC();
	memBM.DeleteObject();
}


// CKol12015View printing

BOOL CKol12015View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKol12015View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKol12015View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKol12015View diagnostics

#ifdef _DEBUG
void CKol12015View::AssertValid() const
{
	CView::AssertValid();
}

void CKol12015View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKol12015Doc* CKol12015View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKol12015Doc)));
	return (CKol12015Doc*)m_pDocument;
}
#endif //_DEBUG


// CKol12015View message handlers


void CKol12015View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_UP:
		m_y -= 10;
		m_open = !m_open;
		m_mirrored = false;
		m_angle = -90;
		Invalidate();
		break;
	case VK_DOWN:
		m_y += 10;
		m_open = !m_open;
		m_mirrored = true;
		m_angle = -90;
		Invalidate();
		break;
	case VK_LEFT:
		m_x -= 10;
		m_open = !m_open;
		m_mirrored = true;
		m_angle = 0;
		Invalidate();
		break;
	case VK_RIGHT:
		m_x += 10;
		m_angle = 0;
		m_open = !m_open;
		m_mirrored = false;
		Invalidate();
		break;
	default:
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CKol12015View::OnEraseBkgnd(CDC* pDC)
{
	//return CView::OnEraseBkgnd(pDC);
	return true;
}

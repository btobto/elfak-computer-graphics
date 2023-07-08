
// Kol1_2016View.cpp : implementation of the CKol12016View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kol1_2016.h"
#endif

#include "Kol1_2016Doc.h"
#include "Kol1_2016View.h"

#include <array>
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKol12016View

IMPLEMENT_DYNCREATE(CKol12016View, CView)

BEGIN_MESSAGE_MAP(CKol12016View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CKol12016View construction/destruction

CKol12016View::CKol12016View() noexcept
{
	m_felt.Load(L"felt2.jpg");
	m_border.Load(L"wood.jpg");
}

CKol12016View::~CKol12016View()
{
}

BOOL CKol12016View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKol12016View drawing

void CKol12016View::DrawAxes(CDC* pDC) {
	POINT curPos = pDC->GetCurrentPosition();
	CPen* prevPen = pDC->GetCurrentPen();

	CPen xAxisPen(PS_SOLID, 5, RGB(255, 0, 0));
	pDC->SelectObject(&xAxisPen);
	pDC->LineTo(curPos.x, curPos.y + 100);
	pDC->MoveTo(curPos.x, curPos.y);
	CPen yAxisPen(PS_SOLID, 5, RGB(0, 0, 255));
	pDC->SelectObject(&yAxisPen);
	pDC->LineTo(curPos.x + 100, curPos.y);
	pDC->MoveTo(curPos.x, curPos.y);

	pDC->SelectObject(prevPen);
}

inline float CKol12016View::ToRadAngle(int angle) {
	return angle * M_PI / 180;
}

void CKol12016View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply = false)
{
	XFORM m = {
		1, 0,
		0, 1,
		dX, dY
	};

	pDC->ModifyWorldTransform(&m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKol12016View::Rotate(CDC* pDC, float angle, bool rightMultiply = false)
{
	float radAngle = ToRadAngle(angle);

	XFORM m = {
		cos(radAngle), -sin(radAngle),
		sin(radAngle), cos(radAngle)
	};

	pDC->ModifyWorldTransform(&m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKol12016View::DrawStick(CDC* pDC, int w)
{
	CPen brownPen(PS_SOLID, 0, RGB(92, 64, 51));
	CPen* oldPen = pDC->SelectObject(&brownPen);

	CBrush lightBrownBrush(RGB(255, 190, 128));
	CBrush* oldBrush = pDC->SelectObject(&lightBrownBrush);

	
	pDC->MoveTo(0, - w / 200);
	pDC->BeginPath();
	{
		pDC->LineTo(2 * w / 3, -w / 120);
		pDC->LineTo(2 * w / 3, w / 120);
		pDC->LineTo(0, w / 200);
		pDC->LineTo(0, -w / 200);
	}
	pDC->EndPath();
	pDC->StrokeAndFillPath();

	CBrush brownBrush(RGB(64, 32, 0));
	pDC->SelectObject(&brownBrush);

	pDC->MoveTo(2 * w / 3, -w / 120);
	pDC->BeginPath();
	{
		pDC->LineTo(w, -w / 100);
		//pDC->LineTo(w, w / 100);
		int oldAD = pDC->SetArcDirection(AD_CLOCKWISE);
		pDC->ArcTo(
			CRect(w - w / 100, -w / 100, w + w / 100, w / 100),
			POINT{ w, -w / 100 },
			POINT{ w, w / 100 }
		);
		pDC->SetArcDirection(oldAD);
		pDC->LineTo(2 * w / 3, w / 120);
		pDC->LineTo(2 * w / 3, -w / 120);
	}
	pDC->EndPath();
	pDC->StrokeAndFillPath();

	CPen whitePen(PS_SOLID, 0, RGB(255, 255, 255));
	pDC->SelectObject(&whitePen);
	pDC->MoveTo(1, -w / 200 + 3);
	pDC->LineTo(w, -w / 100 + 3);

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);
}

void CKol12016View::DrawBall(CDC* pDC, int w)
{
	CPen darkRedPen(PS_SOLID, 0, RGB(61, 11, 7));
	CPen* oldPen = pDC->SelectObject(&darkRedPen);
	CBrush redBrush(RGB(128, 0, 0));
	CBrush* oldBrush = pDC->SelectObject(&redBrush);
	pDC->Ellipse(CRect(POINT{ -w / 2, -w / 2 }, SIZE{ w, w }));

	pDC->SelectStockObject(NULL_PEN);
	CBrush whiteBrush(RGB(255, 255, 255));
	pDC->SelectObject(&whiteBrush);
	pDC->Ellipse(CRect(POINT{ -2, -10 }, SIZE{ w / 3, w / 3 }));

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void CKol12016View::DrawTable(CDC* pDC, CRect rect)
{
	int col = std::ceil(rect.Width() / (float)m_felt.Width());
	int row = std::ceil(rect.Height() / (float)m_felt.Height());

	for (int i = 0; i < row; i++) {
		int yOffset = rect.top + i * m_felt.Height();

		int feltHeight = m_felt.Height();
		if (yOffset + m_felt.Height() > rect.bottom) {
			feltHeight = rect.bottom - yOffset;
		}
		
		for (int j = 0; j < col; j++) {
			int xOffset = rect.left + j * m_felt.Width();

			int feltWidth = m_felt.Width();
			if (xOffset + m_felt.Width() > rect.right) {
				feltWidth = rect.right - xOffset;
			}
			
			m_felt.Draw(pDC,
				CRect(0, 0, feltWidth, feltHeight),
				CRect(POINT { xOffset, yOffset }, SIZE { feltWidth, feltHeight })
			);
		}
	}
}

void CKol12016View::DrawBorder(CDC* pDC, CRect rect, int w)
{
	m_border.Draw(pDC, CRect(0, 0, m_border.Width(), m_border.Height()), &rect);
	DrawTable(pDC, CRect(rect.left + w, rect.top + w, rect.right - w, rect.bottom - w));
}

void CKol12016View::DrawHoles(CDC* pDC, CRect rect, int size)
{
	CBrush blackBrush(RGB(0, 0, 0));
	CBrush* oldBrush = pDC->SelectObject(&blackBrush);
	CPen* oldPen = (CPen*)pDC->SelectStockObject(NULL_PEN);

	SIZE ballSize = { size, size };
	std::array<POINT, 6> upperLeftPoints = { 
		POINT { rect.left + size / 2, rect.top + size / 2 },
		{ rect.left + rect.Width() / 2 - size / 2, rect.top + size / 2},
		{ rect.left + rect.Width() - size - size / 2, rect.top + size / 2 },

		{ rect.left + size / 2, rect.bottom - size - size / 2 },
		{ rect.left + rect.Width() / 2 - size / 2, rect.bottom - size - size / 2},
		{ rect.left + rect.Width() - size - size / 2, rect.bottom - size - size / 2 },
	};

	for (auto& pt : upperLeftPoints) {
		pDC->Ellipse(CRect(pt, ballSize));
	}
	
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void CKol12016View::OnDraw(CDC* pDC)
{
	CKol12016Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect clientRect;
	GetClientRect(&clientRect);

	CBitmap memBM;
	memBM.CreateCompatibleBitmap(pDC, clientRect.Width(), clientRect.Height());

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(&memBM);

	int prevMode = memDC.SetGraphicsMode(GM_ADVANCED);
	XFORM oldXForm;
	memDC.GetWorldTransform(&oldXForm);

	DrawBorder(&memDC, &clientRect, BORDER_SIZE);
	DrawHoles(&memDC, &clientRect, BORDER_SIZE);
	
	// ne valja
	// - lopta se ne treba rotira
	// - stap treba da moze da se rotira oko loptu dok lopta moze da se krece
	// - stap se treba udaljava (tj. da odrzava istu distancu) dok se lopta pomera nazad?

	Translate(&memDC, STARTING_X, STARTING_Y);
	Rotate(&memDC, m_rotAngle);
	Translate(&memDC, m_ballOffset, 0);

	DrawBall(&memDC, BALL_DIAMETER);

	Translate(&memDC, m_stickOffset, 0);
	DrawStick(&memDC, STICK_SIZE);

	memDC.SetWorldTransform(&oldXForm);
	memDC.SetGraphicsMode(prevMode);

	pDC->BitBlt(0, 0, clientRect.Width(), clientRect.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.DeleteDC();
	memBM.DeleteObject();
}


// CKol12016View printing

BOOL CKol12016View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKol12016View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKol12016View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKol12016View diagnostics

#ifdef _DEBUG
void CKol12016View::AssertValid() const
{
	CView::AssertValid();
}

void CKol12016View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKol12016Doc* CKol12016View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKol12016Doc)));
	return (CKol12016Doc*)m_pDocument;
}
#endif //_DEBUG


// CKol12016View message handlers


void CKol12016View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_LEFT:
		if (!m_collision) m_rotAngle -= 5;
		Invalidate();
		break;
	case VK_RIGHT:
		if (!m_collision) m_rotAngle += 5;
		Invalidate();
		break;
	case VK_UP:
		if (m_collision) {
			m_ballOffset -= 10;
			m_stickOffset += 10;
		}
		else {
			if (m_stickOffset - 10 <= BALL_DIAMETER / 2) {
				m_collision = true;
			}
			if (m_stickOffset != BALL_DIAMETER / 2) {
				m_stickOffset -= 10;
			}
		}

		Invalidate();
		break;
	case VK_DOWN:
		if (m_collision) {
			m_ballOffset += 10;
			m_stickOffset -= 10;
			m_collision = !(m_ballOffset == 0);
		}
		else {
			m_stickOffset += 10;
		}

		Invalidate();
		break;
	default:
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CKol12016View::OnEraseBkgnd(CDC* pDC)
{
	//return CView::OnEraseBkgnd(pDC);

	return true;
}


// Kol1_2019View.cpp : implementation of the CKol12019View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kol1_2019.h"
#endif

#include "Kol1_2019Doc.h"
#include "Kol1_2019View.h"

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKol12019View

IMPLEMENT_DYNCREATE(CKol12019View, CView)

BEGIN_MESSAGE_MAP(CKol12019View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CKol12019View construction/destruction

CKol12019View::CKol12019View() noexcept
{
	m_body1.Load(L"body1.png");
	m_arm1.Load(L"arm1.png");
	m_arm2.Load(L"arm2.png");
	m_leg1.Load(L"leg1.png");
	m_leg2.Load(L"leg2.png");
	m_background.Load(L"background.jpg");
}

CKol12019View::~CKol12019View()
{
}

BOOL CKol12019View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKol12019View drawing

void CKol12019View::DrawBackground(CDC* pDC, CRect rc)
{
	m_background.Draw(pDC,
		CRect(0, 0, rc.Width(), rc.Height()),
		rc
	);
}

void CKol12019View::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	auto bytes = pImage->GetDIBBits();
	pImage->DrawTransparent(pDC,
		CRect(0, 0, pImage->Width(), pImage->Height()),
		CRect(0, 0, pImage->Width(), pImage->Height()),
		RGB(bytes[2], bytes[1], bytes[0])
	);
}

void CKol12019View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply = false)
{
	XFORM m = {
		1, 0,
		0, 1,
		dX, dY
	};
	pDC->ModifyWorldTransform(&m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKol12019View::Rotate(CDC* pDC, float angle, bool rightMultiply = false)
{
	float radAngle = angle * M_PI / 180;
	XFORM m = {
		cos(radAngle), sin(radAngle),
		-sin(radAngle), cos(radAngle)
	};
	pDC->ModifyWorldTransform(&m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKol12019View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply = false)
{
	XFORM m = {
		sX, 0,
		0, sY
	};
	pDC->ModifyWorldTransform(&m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKol12019View::DrawArm1(CDC* pDC)
{
	Translate(pDC, 34, 31);
	Rotate(pDC, m_rotArm1QA);
	Translate(pDC, -34, -31);

	DrawImgTransparent(pDC, &m_arm1);
}

void CKol12019View::DrawArm2(CDC* pDC)
{
	XFORM old;
	pDC->GetWorldTransform(&old);

	Translate(pDC, -(210 - 23), -(102 - 61));
	Translate(pDC, 34, 31);
	Rotate(pDC, m_rotArm1QA);
	Translate(pDC, -34, -31);
	Translate(pDC, 210 - 23, 102 - 61);

	Translate(pDC, 23, 61);
	Rotate(pDC, 180 - m_rotArm2TG);
	Translate(pDC, -23, -61);

	DrawImgTransparent(pDC, &m_arm2);

	pDC->SetWorldTransform(&old);
}

void CKol12019View::DrawLeg1(CDC* pDC)
{
	Translate(pDC, 30, 125);
	Rotate(pDC, m_rotLeg1ED);
	Translate(pDC, -30, -125);

	DrawImgTransparent(pDC, &m_leg1);
}

void CKol12019View::DrawLeg2(CDC* pDC)
{
	XFORM old;
	pDC->GetWorldTransform(&old);

	Translate(pDC, 35, 60);
	Rotate(pDC, m_rotLeg2RF);
	Translate(pDC, -35, -60);

	DrawImgTransparent(pDC, &m_leg2);

	pDC->SetWorldTransform(&old);
}

void CKol12019View::DrawBody1(CDC* pDC)
{
	Translate(pDC, 26, 133);
	Rotate(pDC, m_rotBody1WS);
	Translate(pDC, -26, -133);

	DrawImgTransparent(pDC, &m_body1);
}

void CKol12019View::DrawTransformer(CDC* pDC)
{
	int prevMode = pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM oldXForm;
	pDC->GetWorldTransform(&oldXForm);

	Scale(pDC, 0.5, 0.5);
	Translate(pDC, 500, 500);

	DrawLeg2(pDC);

	Translate(pDC, 35 - 30, 60 - 125);
	DrawLeg1(pDC);
	
	Translate(pDC, 237 - 26, 125 - 133);
	DrawBody1(pDC);

	Translate(pDC, 212 - 34 + 187, 85 - 31 + 41);
	DrawArm2(pDC);

	Translate(pDC, -(210 - 23), -(102 - 61));
	DrawArm1(pDC);

	pDC->SetWorldTransform(&oldXForm);
	pDC->SetGraphicsMode(prevMode);
}

void CKol12019View::OnDraw(CDC* pDC)
{
	CKol12019Doc* pDoc = GetDocument();
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

	DrawBackground(&memDC, &client);
	DrawTransformer(&memDC);

	pDC->BitBlt(0, 0, client.Width(), client.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.DeleteDC();
	memBM.DeleteObject();
}


// CKol12019View printing

BOOL CKol12019View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKol12019View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKol12019View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKol12019View diagnostics

#ifdef _DEBUG
void CKol12019View::AssertValid() const
{
	CView::AssertValid();
}

void CKol12019View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKol12019Doc* CKol12019View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKol12019Doc)));
	return (CKol12019Doc*)m_pDocument;
}
#endif //_DEBUG


// CKol12019View message handlers


void CKol12019View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case 'Q':
		m_rotArm1QA -= 10;
		if (m_rotArm1QA < 0) m_rotArm1QA = 0;
		Invalidate();
		break;
	case 'A':
		m_rotArm1QA += 10;
		if (m_rotArm1QA > 270) m_rotArm1QA = 270;
		Invalidate();
		break;
	case 'T':
		m_rotArm2TG -= 10;
		Invalidate();
		break;	
	case 'G':
		m_rotArm2TG += 10;
		Invalidate();
		break;
	case 'E':
		m_rotLeg1ED -= 10;
		Invalidate();
		break;
	case 'D':
		m_rotLeg1ED += 10;
		Invalidate();
		break;
	case 'R':
		m_rotLeg2RF -= 10;
		Invalidate();
		break;
	case 'F':
		m_rotLeg2RF += 10;
		Invalidate();
		break;
	case 'W':
		m_rotBody1WS -= 10;
		Invalidate();
		break;
	case 'S':
		m_rotBody1WS += 10;
		Invalidate();
		break;
	case VK_RIGHT:
		m_rotArm1QA -= 10;
		if (m_rotArm1QA < -270) m_rotArm1QA = -270;

		m_rotArm2TG += 10;
		if (m_rotArm2TG > 270) m_rotArm2TG = 270;

		m_rotLeg1ED -= 10;
		if (m_rotLeg1ED < -110) m_rotLeg1ED = -110;

		m_rotLeg2RF += 10;
		if (m_rotLeg2RF > 110) m_rotLeg2RF = 110;

		m_rotBody1WS += 10;
		if (m_rotBody1WS > 20) m_rotBody1WS = 20;

		Invalidate();
		break;
	case VK_LEFT:
		m_rotArm1QA += 10;
		if (m_rotArm1QA > 0) m_rotArm1QA = 0;

		m_rotArm2TG -= 10;
		if (m_rotArm2TG < 0) m_rotArm2TG = 0;

		m_rotLeg1ED += 10;
		if (m_rotLeg1ED > 0) m_rotLeg1ED = 0;

		m_rotLeg2RF -= 10;
		if (m_rotLeg2RF < 0) m_rotLeg2RF = 0;

		m_rotBody1WS -= 10;
		if (m_rotBody1WS < 0) m_rotBody1WS = 0;

		Invalidate();
		break;
	default:
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CKol12019View::OnEraseBkgnd(CDC* pDC)
{
	//return CView::OnEraseBkgnd(pDC);
	return true;
}

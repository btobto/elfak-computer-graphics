
// Kol1_2018View.cpp : implementation of the CKol12018View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kol1_2018.h"
#endif

#include "Kol1_2018Doc.h"
#include "Kol1_2018View.h"

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKol12018View

IMPLEMENT_DYNCREATE(CKol12018View, CView)

BEGIN_MESSAGE_MAP(CKol12018View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CKol12018View construction/destruction

CKol12018View::CKol12018View() noexcept
{
	m_excavator.Load(L"./res/bager.png");
	m_arm1.Load(L"./res/arm1.png");
	m_arm2.Load(L"./res/arm2.png");
	m_background.Load(L"./res/pozadina.png");
	m_fork = GetEnhMetaFile(L"./res/viljuska.emf");
}

CKol12018View::~CKol12018View()
{
	DeleteEnhMetaFile(m_fork);
}

BOOL CKol12018View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKol12018View drawing

void CKol12018View::DrawAxes(CDC* pDC)
{
	POINT pt = pDC->GetCurrentPosition();
	
	CPen xAxisPen(PS_SOLID, 5, RGB(255, 0, 0));
	CPen* oldPen = pDC->SelectObject(&xAxisPen);
	pDC->LineTo(pt.x, pt.y + 100);
	pDC->MoveTo(pt.x, pt.y);

	CPen yAxisPen(PS_SOLID, 5, RGB(0, 0, 255));
	pDC->SelectObject(&yAxisPen);
	pDC->LineTo(pt.x + 100, pt.y);
	pDC->MoveTo(pt.x, pt.y);

	pDC->SelectObject(oldPen);
}

void CKol12018View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply = false)
{
	XFORM m = {
		1, 0,
		0, 1,
		dX, dY
	};
	
	pDC->ModifyWorldTransform(&m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKol12018View::Rotate(CDC* pDC, float angle, bool rightMultiply = false)
{
	float radAngle = angle * M_PI / 180;

	XFORM m = {
		cos(radAngle), sin(radAngle),
		-sin(radAngle), cos(radAngle),
	};

	pDC->ModifyWorldTransform(&m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKol12018View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply = false)
{
	XFORM m = {
		sX, 0,
		0, sY,
	};

	pDC->ModifyWorldTransform(&m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKol12018View::DrawBackground(CDC* pDC)
{
	CRect client;
	GetClientRect(&client);

	int h = m_background.Height() - client.Height();
	int w = m_background.Width() - client.Width();

	m_background.Draw(
		pDC,
		CRect(w / 2, h, w / 2 + client.Width(), m_background.Height()),
		CRect(0, 0, client.Width(), client.Height())
	);
}

void CKol12018View::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	int w = pImage->Width();
	int h = pImage->Height();

	CBitmap* bmpImage = pImage->GetBitmap();

	CBitmap bmpMask;
	bmpMask.CreateBitmap(w, h, 1, 1, NULL);

	CDC srcDC;
	srcDC.CreateCompatibleDC(pDC);
	CDC dstDC;
	dstDC.CreateCompatibleDC(pDC);

	srcDC.SelectObject(bmpImage);
	dstDC.SelectObject(&bmpMask);

	srcDC.SetBkColor(srcDC.GetPixel(0, 0));
	dstDC.BitBlt(0, 0, w, h, &srcDC, 0, 0, SRCCOPY);

	srcDC.SetTextColor(RGB(255, 255, 255));
	srcDC.SetBkColor(RGB(0, 0, 0));
	srcDC.BitBlt(0, 0, w, h, &dstDC, 0, 0, SRCAND);

	srcDC.DeleteDC();
	dstDC.DeleteDC();

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	memDC.SelectObject(&bmpMask);
	pDC->BitBlt(0, 0, w, h, &memDC, 0, 0, SRCAND);

	memDC.SelectObject(bmpImage);
	pDC->BitBlt(0, 0, w, h, &memDC, 0, 0, SRCPAINT);
}

void CKol12018View::DrawBody(CDC* pDC)
{
	DrawImgTransparent(pDC, &m_excavator);
	Translate(pDC, 0, 225);
}

void CKol12018View::DrawArm1(CDC* pDC)
{
	Rotate(pDC, -90);

	Translate(pDC, 58, 61);
	Rotate(pDC, m_arm1Angle);
	Translate(pDC, -58, -61);

	DrawImgTransparent(pDC, &m_arm1);

	Translate(pDC, 270, 97);
}

void CKol12018View::DrawArm2(CDC* pDC)
{
	Rotate(pDC, -90);

	Translate(pDC, 36, 40);
	Rotate(pDC, m_arm2Angle);
	Translate(pDC, -36, -40);

	DrawImgTransparent(pDC, &m_arm2);

	Translate(pDC, 230, 65);
}

void CKol12018View::DrawFork(CDC* pDC)
{
	Scale(pDC, 2.5, 2.5);
	Rotate(pDC, -90);

	Translate(pDC, 10, 16);
	Rotate(pDC, m_forkAngle);
	Translate(pDC, -10, -16);
	
	ENHMETAHEADER emfHeader;
	GetEnhMetaFileHeader(m_fork, sizeof(ENHMETAHEADER), &emfHeader);
	PlayEnhMetaFile(pDC->m_hDC, m_fork, CRect{ 0, 0, emfHeader.rclBounds.right, emfHeader.rclBounds.bottom });
}

void CKol12018View::DrawExcavator(CDC* pDC)
{
	CRect client;
	GetClientRect(&client);

	int prevMode = pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM oldXForm;
	pDC->GetWorldTransform(&oldXForm);

	Translate(pDC, client.Width() - m_excavator.Width(), client.Height() - m_excavator.Height());
	Translate(pDC, m_xOffset, 0);

	DrawBody(pDC);
	DrawArm1(pDC);
	DrawArm2(pDC);
	DrawFork(pDC);

	pDC->SetWorldTransform(&oldXForm);
	pDC->SetGraphicsMode(prevMode);
}

void CKol12018View::OnDraw(CDC* pDC)
{
	CKol12018Doc* pDoc = GetDocument();
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

	DrawBackground(&memDC);
	DrawExcavator(&memDC);

	pDC->BitBlt(0, 0, client.Width(), client.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.DeleteDC();
	memBM.DeleteObject();
}


// CKol12018View printing

BOOL CKol12018View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKol12018View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKol12018View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKol12018View diagnostics

#ifdef _DEBUG
void CKol12018View::AssertValid() const
{
	CView::AssertValid();
}

void CKol12018View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKol12018Doc* CKol12018View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKol12018Doc)));
	return (CKol12018Doc*)m_pDocument;
}
#endif //_DEBUG


// CKol12018View message handlers


void CKol12018View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case '1':
		m_arm1Angle -= 10;
		Invalidate();
		break;
	case '2':
		m_arm1Angle += 10;
		Invalidate();
		break;
	case '3':
		m_arm2Angle -= 10;
		Invalidate();
		break;
	case '4':
		m_arm2Angle += 10;
		Invalidate();
		break;
	case '5':
		m_forkAngle -= 10;
		Invalidate();
		break;
	case '6':
		m_forkAngle += 10;
		Invalidate();
		break;
	case VK_LEFT:
		m_xOffset -= 10;
		Invalidate();
		break;
	case VK_RIGHT:
		m_xOffset += 10;
		Invalidate();
		break;
	default:
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CKol12018View::OnEraseBkgnd(CDC* pDC)
{
	//return CView::OnEraseBkgnd(pDC);
	return true;
}

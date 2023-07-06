
// Kol1_2017View.cpp : implementation of the CKol12017View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kol1_2017.h"
#endif

#include "Kol1_2017Doc.h"
#include "Kol1_2017View.h"

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKol12017View

IMPLEMENT_DYNCREATE(CKol12017View, CView)

BEGIN_MESSAGE_MAP(CKol12017View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CKol12017View construction/destruction

CKol12017View::CKol12017View() noexcept
{
	body.Load(L"Body.png");
	leg1.Load(L"Leg1.png");
	leg2.Load(L"Leg2.png");
	leg3.Load(L"Leg3.png");
	back2.Load(L"Back2.jpg");
}

CKol12017View::~CKol12017View()
{
}

BOOL CKol12017View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKol12017View drawing

void CKol12017View::DrawTransparentImage(CDC* pDC, CRect rect, DImage& img)
{
	int w = img.Width();
	int h = img.Height();

	CBitmap bmpImg;
	bmpImg.CreateCompatibleBitmap(pDC, img.Width(), img.Height());
	
	CDC srcDC;
	srcDC.CreateCompatibleDC(pDC);
	srcDC.SelectObject(&bmpImg);
	img.Draw(&srcDC, CRect(0, 0, img.Width(), img.Height()), CRect(0, 0, img.Width(), img.Height()));

	CDC dstDC;
	dstDC.CreateCompatibleDC(pDC);

	CBitmap bmpMask;
	bmpMask.CreateBitmap(img.Width(), img.Height(), 1, 1, NULL);
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
	pDC->BitBlt(rect.left, rect.top, rect.left + w, rect.top + h, &memDC, 0, 0, SRCAND);

	memDC.SelectObject(&bmpImg);
	pDC->BitBlt(rect.left, rect.top, rect.left + w, rect.top + h, &memDC, 0, 0, SRCPAINT);
}

void CKol12017View::LoadIdentity(CDC* pDC)
{
	XFORM m = {
		1, 0,
		0, 1,
		0, 0
	};
	pDC->ModifyWorldTransform(&m, MWT_IDENTITY);
}

void CKol12017View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply = false)
{
	XFORM m = {
	1, 0,
	0, 1,
	dX, dY
	};

	pDC->ModifyWorldTransform(&m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKol12017View::Rotate(CDC* pDC, float angle, bool rightMultiply = false)
{
	double radAngle = angle * M_PI / 180;

	XFORM m = {
		cos(radAngle), -sin(radAngle),
		sin(radAngle), cos(radAngle)
	};

	pDC->ModifyWorldTransform(&m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKol12017View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply = false)
{
	XFORM m = {
		sX, 0,
		0, sY
	};

	pDC->ModifyWorldTransform(&m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKol12017View::DrawScene(CDC* pDC, CRect rect)
{
	back2.Draw(pDC, CRect(0, 0, back2.Width(), back2.Height()), rect);

	int prevMode = pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM xform;
	pDC->GetWorldTransform(&xform);

	Translate(pDC, 100, 100);

	DrawTransparentImage(pDC, rect, body);
	DrawLeg(pDC, 0, 52, 168);
	DrawLeg(pDC, 0, 270, 168);

	pDC->SetWorldTransform(&xform);
	pDC->SetGraphicsMode(prevMode);
}

void CKol12017View::DrawLeg(CDC* pDC, double alpha, double dx, double dy)
{
	//leg1.Draw(pDC, 
	//	CRect(0, 0, leg1.Width(), leg1.Height()),
	//	CRect(dx, dy, dx + leg1.Width(), dy + leg1.Height())
	//);
	DrawTransparentImage(pDC, CRect(POINT{ int(dx), int(dy) }, SIZE{ leg1.Width(), leg1.Height() }), leg1);
}

void CKol12017View::OnDraw(CDC* pDC)
{
	CKol12017Doc* pDoc = GetDocument();
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

	DrawScene(&memDC, client);

	pDC->BitBlt(0, 0, client.Width(), client.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.DeleteDC();
	memBM.DeleteObject();
}


// CKol12017View printing

BOOL CKol12017View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKol12017View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKol12017View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKol12017View diagnostics

#ifdef _DEBUG
void CKol12017View::AssertValid() const
{
	CView::AssertValid();
}

void CKol12017View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKol12017Doc* CKol12017View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKol12017Doc)));
	return (CKol12017Doc*)m_pDocument;
}
#endif //_DEBUG


// CKol12017View message handlers

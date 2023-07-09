
// FillingAlgorithmsView.cpp : implementation of the CFillingAlgorithmsView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "FillingAlgorithms.h"
#endif

#include "FillingAlgorithmsDoc.h"
#include "FillingAlgorithmsView.h"

#include "fill.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFillingAlgorithmsView

IMPLEMENT_DYNCREATE(CFillingAlgorithmsView, CView)

BEGIN_MESSAGE_MAP(CFillingAlgorithmsView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CFillingAlgorithmsView construction/destruction

CFillingAlgorithmsView::CFillingAlgorithmsView() noexcept
{
	// TODO: add construction code here

}

CFillingAlgorithmsView::~CFillingAlgorithmsView()
{
}

BOOL CFillingAlgorithmsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CFillingAlgorithmsView drawing

void CFillingAlgorithmsView::translate(CDC* pDC, float dx, float dy, bool isRightMultiply)
{
	XFORM m = {
		1, 0,
		0, 1,
		dx, dy
	};
	pDC->ModifyWorldTransform(&m, isRightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CFillingAlgorithmsView::OnDraw(CDC* pDC)
{
	CFillingAlgorithmsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	int prevMode = pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM old;
	pDC->GetWorldTransform(&old);

	COLORREF black = RGB(0, 0, 0);
	CPen pen(PS_SOLID, 0, black);
	CPen* oldPen = pDC->SelectObject(&pen);

	pDC->Ellipse(50, 50, 100, 120);
	fill::boundary4(pDC, POINT{ 70, 70 }, black, RGB(0, 0, 255));

	translate(pDC, 150, 50);

	POINT pts[] = {
		{ 0, 0 }, { 30, 0 }, { 0, 30 }, { 30, 30 }
	};
	pDC->Polygon(pts, 4);
	fill::boundary8(pDC, POINT{ 20, 20 }, black, RGB(255, 0, 0));

	pDC->SelectObject(oldPen);

	pDC->SetWorldTransform(&old);
	pDC->SetGraphicsMode(prevMode);
}


// CFillingAlgorithmsView printing

BOOL CFillingAlgorithmsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CFillingAlgorithmsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CFillingAlgorithmsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CFillingAlgorithmsView diagnostics

#ifdef _DEBUG
void CFillingAlgorithmsView::AssertValid() const
{
	CView::AssertValid();
}

void CFillingAlgorithmsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFillingAlgorithmsDoc* CFillingAlgorithmsView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFillingAlgorithmsDoc)));
	return (CFillingAlgorithmsDoc*)m_pDocument;
}
#endif //_DEBUG


// CFillingAlgorithmsView message handlers

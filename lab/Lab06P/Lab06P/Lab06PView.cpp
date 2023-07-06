
// Lab06PView.cpp : implementation of the CLab06PView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Lab06P.h"
#endif

#include "Lab06PDoc.h"
#include "Lab06PView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLab06PView

IMPLEMENT_DYNCREATE(CLab06PView, CView)

BEGIN_MESSAGE_MAP(CLab06PView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CLab06PView construction/destruction

CLab06PView::CLab06PView() noexcept
{
	// TODO: add construction code here

}

CLab06PView::~CLab06PView()
{
}

BOOL CLab06PView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CLab06PView drawing

void CLab06PView::OnDraw(CDC* pDC)
{
	CLab06PDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_glRenderer.DrawScene(pDC);
}


// CLab06PView printing

BOOL CLab06PView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLab06PView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLab06PView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CLab06PView diagnostics

#ifdef _DEBUG
void CLab06PView::AssertValid() const
{
	CView::AssertValid();
}

void CLab06PView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLab06PDoc* CLab06PView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLab06PDoc)));
	return (CLab06PDoc*)m_pDocument;
}
#endif //_DEBUG


// CLab06PView message handlers


void CLab06PView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}


int CLab06PView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	m_glRenderer.CreateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}


void CLab06PView::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	m_glRenderer.DestroyScene(pDC);
	ReleaseDC(pDC);
}


BOOL CLab06PView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	//return CView::OnEraseBkgnd(pDC);
}


void CLab06PView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	m_glRenderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}


void CLab06PView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CLab06PView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_rotating = true;

	CView::OnLButtonDown(nFlags, point);
}


void CLab06PView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_rotating = false;

	CView::OnLButtonUp(nFlags, point);
}


BOOL CLab06PView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_glRenderer.Zoom(zDelta > 0 ? -1 : 1);
	Invalidate();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CLab06PView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_rotating) {
		m_glRenderer.RotateView(point.x - m_prevX, point.y - m_prevY);
		Invalidate();
	}

	m_prevX = point.x;
	m_prevY = point.y;

	CView::OnMouseMove(nFlags, point);
}

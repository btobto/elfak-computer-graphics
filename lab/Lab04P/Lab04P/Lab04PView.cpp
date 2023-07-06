
// Lab04PView.cpp : implementation of the CLab04PView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Lab04P.h"
#endif

#include "Lab04PDoc.h"
#include "Lab04PView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLab04PView

IMPLEMENT_DYNCREATE(CLab04PView, CView)

BEGIN_MESSAGE_MAP(CLab04PView, CView)
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
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CLab04PView construction/destruction

CLab04PView::CLab04PView() noexcept
{
	// TODO: add construction code here

}

CLab04PView::~CLab04PView()
{
}

BOOL CLab04PView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CLab04PView drawing

void CLab04PView::OnDraw(CDC* pDC)
{
	CLab04PDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_glRenderer.DrawScene(pDC);
}


// CLab04PView printing

BOOL CLab04PView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLab04PView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLab04PView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CLab04PView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}


// CLab04PView diagnostics

#ifdef _DEBUG
void CLab04PView::AssertValid() const
{
	CView::AssertValid();
}

void CLab04PView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLab04PDoc* CLab04PView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLab04PDoc)));
	return (CLab04PDoc*)m_pDocument;
}
#endif //_DEBUG


// CLab04PView message handlers


int CLab04PView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	m_glRenderer.CreateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}


void CLab04PView::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	m_glRenderer.DestroyScene(pDC);
	ReleaseDC(pDC);
}


BOOL CLab04PView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	//return CView::OnEraseBkgnd(pDC);
}


void CLab04PView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	m_glRenderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}




void CLab04PView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	constexpr int step = 10;

	switch (nChar)
	{
	case VK_LEFT:
		m_glRenderer.RotateArm(step);
		Invalidate();
		break;
	case VK_RIGHT:
		m_glRenderer.RotateArm(-step);
		Invalidate();
		break;
	default:
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CLab04PView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_rotating = true;

	CView::OnLButtonDown(nFlags, point);
}


void CLab04PView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_rotating = false;

	CView::OnLButtonUp(nFlags, point);
}


void CLab04PView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_rotating) {
		m_glRenderer.RotateView(point.x - m_prevX, point.y - m_prevY);
		Invalidate();
	}

	m_prevX = point.x;
	m_prevY = point.y;

	CView::OnMouseMove(nFlags, point);
}


BOOL CLab04PView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_glRenderer.Zoom(zDelta > 0 ? -1 : 1);
	Invalidate();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

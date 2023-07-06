
// Lab05PView.cpp : implementation of the CLab05PView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Lab05P.h"
#endif

#include "Lab05PDoc.h"
#include "Lab05PView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLab05PView

IMPLEMENT_DYNCREATE(CLab05PView, CView)

BEGIN_MESSAGE_MAP(CLab05PView, CView)
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

// CLab05PView construction/destruction

CLab05PView::CLab05PView() noexcept
{
	// TODO: add construction code here

}

CLab05PView::~CLab05PView()
{
}

BOOL CLab05PView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CLab05PView drawing

void CLab05PView::OnDraw(CDC* pDC)
{
	CLab05PDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_glRenderer.DrawScene(pDC);
}


// CLab05PView printing

BOOL CLab05PView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLab05PView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLab05PView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CLab05PView diagnostics

#ifdef _DEBUG
void CLab05PView::AssertValid() const
{
	CView::AssertValid();
}

void CLab05PView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLab05PDoc* CLab05PView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLab05PDoc)));
	return (CLab05PDoc*)m_pDocument;
}
#endif //_DEBUG


// CLab05PView message handlers


void CLab05PView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}


int CLab05PView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	m_glRenderer.CreateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}


void CLab05PView::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	m_glRenderer.DestroyScene(pDC);
	ReleaseDC(pDC);
}


BOOL CLab05PView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	// return CView::OnEraseBkgnd(pDC);
}


void CLab05PView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	m_glRenderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}


void CLab05PView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_SPACE:
		m_glRenderer.m_showNormals = !m_glRenderer.m_showNormals;
		Invalidate();
		break;
	case '1':
		m_glRenderer.m_redLightOn = !m_glRenderer.m_redLightOn;
		Invalidate();
		break;
	case '2':
		m_glRenderer.m_purpleLightOn = !m_glRenderer.m_purpleLightOn;
		Invalidate();
		break;
	case '3':
		m_glRenderer.m_blueLightOn = !m_glRenderer.m_blueLightOn;
		Invalidate();
		break;
	default:
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CLab05PView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_rotating = true;

	CView::OnLButtonDown(nFlags, point);
}


void CLab05PView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_rotating = false;

	CView::OnLButtonUp(nFlags, point);
}


void CLab05PView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_rotating) {
		m_glRenderer.RotateView(point.x - m_prevX, point.y - m_prevY);
		Invalidate();
	}

	m_prevX = point.x;
	m_prevY = point.y;

	CView::OnMouseMove(nFlags, point);
}


BOOL CLab05PView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_glRenderer.Zoom(zDelta > 0 ? -1 : 1);
	Invalidate();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

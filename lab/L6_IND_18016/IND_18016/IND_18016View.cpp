
// IND_18016View.cpp : implementation of the CIND18016View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IND_18016.h"
#endif

#include "IND_18016Doc.h"
#include "IND_18016View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIND18016View

IMPLEMENT_DYNCREATE(CIND18016View, CView)

BEGIN_MESSAGE_MAP(CIND18016View, CView)
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

// CIND18016View construction/destruction

CIND18016View::CIND18016View() noexcept
{
	// TODO: add construction code here

}

CIND18016View::~CIND18016View()
{
}

BOOL CIND18016View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIND18016View drawing

void CIND18016View::OnDraw(CDC* pDC)
{
	CIND18016Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_glRenderer.DrawScene(pDC);
}


// CIND18016View printing

BOOL CIND18016View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIND18016View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIND18016View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CIND18016View diagnostics

#ifdef _DEBUG
void CIND18016View::AssertValid() const
{
	CView::AssertValid();
}

void CIND18016View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIND18016Doc* CIND18016View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIND18016Doc)));
	return (CIND18016Doc*)m_pDocument;
}
#endif //_DEBUG


// CIND18016View message handlers


void CIND18016View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}


int CIND18016View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	m_glRenderer.CreateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}


void CIND18016View::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	m_glRenderer.DestroyScene(pDC);
	ReleaseDC(pDC);
}


BOOL CIND18016View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	//return CView::OnEraseBkgnd(pDC);
}


void CIND18016View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	m_glRenderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}


void CIND18016View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CIND18016View::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_rotating = true;

	CView::OnLButtonDown(nFlags, point);
}


void CIND18016View::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_rotating = false;

	CView::OnLButtonUp(nFlags, point);
}


void CIND18016View::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_rotating) {
		m_glRenderer.RotateView(point.x - m_prevX, point.y - m_prevY);
		Invalidate();
	}

	m_prevX = point.x;
	m_prevY = point.y;

	CView::OnMouseMove(nFlags, point);
}


BOOL CIND18016View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_glRenderer.Zoom(zDelta > 0 ? -1 : 1);
	Invalidate();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

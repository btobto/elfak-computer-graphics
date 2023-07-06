
// Lab04PView.h : interface of the CLab04PView class
//

#pragma once

#include "CGLRenderer.h"

class CLab04PView : public CView
{
protected: // create from serialization only
	CLab04PView() noexcept;
	DECLARE_DYNCREATE(CLab04PView)

// Attributes
public:
	CLab04PDoc* GetDocument() const;
private:
	CGLRenderer m_glRenderer;
	bool m_rotating = false;
	int m_prevX;
	int m_prevY;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnInitialUpdate();

// Implementation
public:
	virtual ~CLab04PView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

#ifndef _DEBUG  // debug version in Lab04PView.cpp
inline CLab04PDoc* CLab04PView::GetDocument() const
   { return reinterpret_cast<CLab04PDoc*>(m_pDocument); }
#endif



// Kol1_2015View.h : interface of the CKol12015View class
//

#pragma once

#include "DImage.h"

class CKol12015View : public CView
{
protected: // create from serialization only
	CKol12015View() noexcept;
	DECLARE_DYNCREATE(CKol12015View)

// Attributes
public:
	CKol12015Doc* GetDocument() const;
protected:
	DImage m_background;
	int m_x = 400;
	int m_y = 200;
	bool m_open = true;
	bool m_mirrored = false;
	int m_angle = 0;

// Operations
protected:
	void Mirror(CDC* pDC, bool mX, bool mY, bool isRightMultiply);
	void Rotate(CDC* pDC, float angle, bool isRightMultiply);
	void Translate(CDC* pDC, float dX, float dY, bool isRightMultiply);
	void DrawBackground(CDC* pDC, CRect rect);
	bool Collision(CRect rect1, CRect rect2);
	void DrawTextAt(CDC* pDC, POINT pt, CString font, int size, CString text, COLORREF color);

	void DrawPacman(CDC* pDC, CRect rect, float angle);
	void DrawGhost(CDC* pDC, CRect rect);

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKol12015View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in Kol1_2015View.cpp
inline CKol12015Doc* CKol12015View::GetDocument() const
   { return reinterpret_cast<CKol12015Doc*>(m_pDocument); }
#endif



// Kol1_2018View.h : interface of the CKol12018View class
//

#pragma once

#include "DImage.h"

class CKol12018View : public CView
{
protected: // create from serialization only
	CKol12018View() noexcept;
	DECLARE_DYNCREATE(CKol12018View)

// Attributes
public:
	CKol12018Doc* GetDocument() const;
protected:
	DImage m_excavator, m_arm1, m_arm2, m_background;
	HENHMETAFILE m_fork;
	int m_arm1Angle = 0;
	int m_arm2Angle = 0;
	int m_forkAngle = 0;
	int m_xOffset = 0;

// Operations
protected:
	void DrawAxes(CDC* pDC);

	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void DrawBackground(CDC* pDC);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void DrawBody(CDC* pDC);
	void DrawArm1(CDC* pDC);
	void DrawArm2(CDC* pDC);
	void DrawFork(CDC* pDC);
	void DrawExcavator(CDC* pDC);

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
	virtual ~CKol12018View();
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

#ifndef _DEBUG  // debug version in Kol1_2018View.cpp
inline CKol12018Doc* CKol12018View::GetDocument() const
   { return reinterpret_cast<CKol12018Doc*>(m_pDocument); }
#endif


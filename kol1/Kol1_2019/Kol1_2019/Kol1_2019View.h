
// Kol1_2019View.h : interface of the CKol12019View class
//

#pragma once

#include "DImage.h"


class CKol12019View : public CView
{
protected: // create from serialization only
	CKol12019View() noexcept;
	DECLARE_DYNCREATE(CKol12019View)

// Attributes
public:
	CKol12019Doc* GetDocument() const;
protected:
	DImage m_body1, m_arm1, m_arm2, m_leg1, m_leg2, m_background;
	int m_rotArm1QA = 0;
	int m_rotArm2TG = 0;
	int m_rotLeg1ED = 0;
	int m_rotLeg2RF = 0;
	int m_rotBody1WS = 0;

// Operations
protected:
	void DrawBackground(CDC* pDC, CRect rc);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void DrawArm1(CDC* pDC);
	void DrawArm2(CDC* pDC);
	void DrawLeg1(CDC* pDC);
	void DrawLeg2(CDC* pDC);
	void DrawBody1(CDC* pDC);
	void DrawTransformer(CDC* pDC);

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
	virtual ~CKol12019View();
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

#ifndef _DEBUG  // debug version in Kol1_2019View.cpp
inline CKol12019Doc* CKol12019View::GetDocument() const
   { return reinterpret_cast<CKol12019Doc*>(m_pDocument); }
#endif


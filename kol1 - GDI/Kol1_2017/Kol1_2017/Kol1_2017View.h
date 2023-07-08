
// Kol1_2017View.h : interface of the CKol12017View class
//

#pragma once

#include "DImage.h"

class CKol12017View : public CView
{
protected: // create from serialization only
	CKol12017View() noexcept;
	DECLARE_DYNCREATE(CKol12017View)

// Attributes
public:
	CKol12017Doc* GetDocument() const;
protected:
	DImage body, leg1, leg2, leg3, back2;

// Operations
protected:
	void DrawTransparentImage(CDC* pDC, CRect rect, DImage& img);
	void LoadIdentity(CDC* pDC);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void DrawScene(CDC* pDC, CRect rect);
	void DrawLeg(CDC* pDC, double alpha, double dx, double dy);

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
	virtual ~CKol12017View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Kol1_2017View.cpp
inline CKol12017Doc* CKol12017View::GetDocument() const
   { return reinterpret_cast<CKol12017Doc*>(m_pDocument); }
#endif


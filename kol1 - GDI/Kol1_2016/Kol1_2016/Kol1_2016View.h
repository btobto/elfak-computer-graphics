
// Kol1_2016View.h : interface of the CKol12016View class
//

#pragma once

#include "DImage.h"

#define STARTING_X 600
#define STARTING_Y 400
#define BORDER_SIZE 35
#define BALL_DIAMETER 30
#define STICK_SIZE 600

class CKol12016View : public CView
{
protected: // create from serialization only
	CKol12016View() noexcept;
	DECLARE_DYNCREATE(CKol12016View)

// Attributes
public:
	CKol12016Doc* GetDocument() const;
protected:
	DImage m_felt;
	DImage m_border;
	int m_ballOffset = 0;
	int m_stickOffset = BALL_DIAMETER / 2;
	int m_rotAngle = 0;
	bool m_collision = false;
	
// Operations
public:
	void DrawAxes(CDC* pDC);
	float ToRadAngle(int angle);

	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void DrawStick(CDC* pDC, int w);
	void DrawBall(CDC* pDC, int w);
	void DrawTable(CDC* pDC, CRect rect);
	void DrawBorder(CDC* pDC, CRect rect, int w);
	void DrawHoles(CDC* pDC, CRect rect, int size);

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
	virtual ~CKol12016View();
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

#ifndef _DEBUG  // debug version in Kol1_2016View.cpp
inline CKol12016Doc* CKol12016View::GetDocument() const
   { return reinterpret_cast<CKol12016Doc*>(m_pDocument); }
#endif


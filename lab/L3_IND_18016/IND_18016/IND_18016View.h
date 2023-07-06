
// IND_18016View.h : interface of the CIND18016View class
//

#pragma once
#include "DImage.h"

#define N 3
#define M 3
#define SIZE (N * M)

struct PuzzlePiece {
	DImage piece;
	int dx;
	int dy;
	bool mx;
	bool my;
	int angle;
	bool blueFilter;
	int i;
	int j;
};


class CIND18016View : public CView
{
protected: // create from serialization only
	CIND18016View() noexcept;
	DECLARE_DYNCREATE(CIND18016View)

// Attributes
public:
	CIND18016Doc* GetDocument() const;
protected:
	PuzzlePiece* m_pieces;

// Operations
protected:
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply);
	void DrawGrid(CDC* pDC, COLORREF color);
	void ApplyBlueFilter(CBitmap* bitmap);
	void ApplyGreyFilter(CBitmap* bitmap);
	void DrawPuzzlePiece(CDC* pDC, PuzzlePiece& pc);
	void DrawPuzzle(CDC* pDC);

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
	virtual ~CIND18016View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in IND_18016View.cpp
inline CIND18016Doc* CIND18016View::GetDocument() const
   { return reinterpret_cast<CIND18016Doc*>(m_pDocument); }
#endif



// Lab01FView.h : interface of the CLab01FView class
//

#pragma once


class CLab01FView : public CView
{
protected: // create from serialization only
	CLab01FView() noexcept;
	DECLARE_DYNCREATE(CLab01FView)

// Attributes
public:
	CLab01FDoc* GetDocument() const;
protected:
	static const int m_areaDim = 500;
	static const int m_squareNum = 20;
	static const int m_squareSize = m_areaDim / m_squareNum;
	bool m_gridEnabled = false;

// Operations
protected:
	void DrawPolygon(CDC* pDC, POINT points[], int pointNum, COLORREF fill, int hatch);
	void DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle);
	void DrawGrid(CDC* pDC, COLORREF color);

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
	virtual ~CLab01FView();
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
};

#ifndef _DEBUG  // debug version in Lab01FView.cpp
inline CLab01FDoc* CLab01FView::GetDocument() const
   { return reinterpret_cast<CLab01FDoc*>(m_pDocument); }
#endif


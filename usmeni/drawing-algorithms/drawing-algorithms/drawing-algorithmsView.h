
// drawing-algorithmsView.h : interface of the CdrawingalgorithmsView class
//

#pragma once


class CdrawingalgorithmsView : public CView
{
protected: // create from serialization only
	CdrawingalgorithmsView() noexcept;
	DECLARE_DYNCREATE(CdrawingalgorithmsView)

// Attributes
public:
	CdrawingalgorithmsDoc* GetDocument() const;

// Operations
public:
	void translate(CDC* pDC, float dx, float dy, bool isRightMultiply = false);
	void scale(CDC* pDC, float sx, float sy, bool isRightMultiply = false);
	void drawGrid(CDC* pDC, int tileNum, int tileLen, COLORREF clr);
	void drawAxes(CDC* pDC, int axisLen);
	void drawLines(CDC* pDC);
	void drawCircles(CDC* pDC);
	void drawEllipses(CDC* pDC);
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
	virtual ~CdrawingalgorithmsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in drawing-algorithmsView.cpp
inline CdrawingalgorithmsDoc* CdrawingalgorithmsView::GetDocument() const
   { return reinterpret_cast<CdrawingalgorithmsDoc*>(m_pDocument); }
#endif


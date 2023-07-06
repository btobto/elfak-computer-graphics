
// Lab02PView.h : interface of the CLab02PView class
//

#pragma once
#include <string>


class CLab02PView : public CView
{
protected: // create from serialization only
	CLab02PView() noexcept;
	DECLARE_DYNCREATE(CLab02PView)

// Attributes
public:
	CLab02PDoc* GetDocument() const;
protected:
	static const int m_areaDim = 500;
	static const int m_squareNum = 20;
	static const int m_squareSize = m_areaDim / m_squareNum;
	bool m_gridEnabled = false;
	HENHMETAFILE m_cactusMF;
	HENHMETAFILE m_lightCactusMF;
	int m_smallCactusRotAngle = 0;
	int m_bigCactusRotAngle = 0;
	float m_cactusHeightFactor;
	float m_thickCactusWidthFactor;
	float m_mediumCactusWidthFactor;
	float m_thinCactusWidthFactor;

// Operations
protected:
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void RotateAroundPoint(CDC* pDC, float x, float y, float angle);
	void DrawPolygon(CDC* pDC, POINT points[], int pointNum, COLORREF fill);
	void DrawCircle(CDC* pDC, int x, int y, int diameter);
	void DrawGrid(CDC* pDC, COLORREF color);
	void DrawPot(CDC* pDC);
	void DrawCactus(CDC* pDC, float x, float y, float widthFactor, float heightFactor, int rotAngle, const HENHMETAFILE& mf);
	void DrawFigure(CDC* pDC);

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
	virtual ~CLab02PView();
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

#ifndef _DEBUG  // debug version in Lab02PView.cpp
inline CLab02PDoc* CLab02PView::GetDocument() const
   { return reinterpret_cast<CLab02PDoc*>(m_pDocument); }
#endif


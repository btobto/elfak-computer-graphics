
// IND_18016View.h : interface of the CIND18016View class
//

#pragma once


class CIND18016View : public CView
{
protected: // create from serialization only
	CIND18016View() noexcept;
	DECLARE_DYNCREATE(CIND18016View)

// Attributes
public:
	CIND18016Doc* GetDocument() const;
protected:
	bool m_gridEnabled = false;
	HENHMETAFILE m_cactusMF;
	HENHMETAFILE m_lightCactusMF;
	int m_smallCactusRotAngle = 0;
	int m_cactusArmRotAngle = 0;

// Operations
protected:
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void RotateAroundPoint(CDC* pDC, float tX, float tY, int angle, bool rightMultiply);
	void DrawCircle(CDC* pDC, POINT center, int diameter);
	void DrawGrid(CDC* pDC, COLORREF color);
	void DrawPot(CDC* pDC);
	void DrawTextWithShadow(CDC* pDC, CString text, POINT location, LOGFONT logFont, COLORREF color, COLORREF shadowColor, int offset);
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
	virtual ~CIND18016View();
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

#ifndef _DEBUG  // debug version in IND_18016View.cpp
inline CIND18016Doc* CIND18016View::GetDocument() const
   { return reinterpret_cast<CIND18016Doc*>(m_pDocument); }
#endif


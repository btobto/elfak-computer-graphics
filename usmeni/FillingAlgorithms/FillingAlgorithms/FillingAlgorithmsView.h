
// FillingAlgorithmsView.h : interface of the CFillingAlgorithmsView class
//

#pragma once


class CFillingAlgorithmsView : public CView
{
protected: // create from serialization only
	CFillingAlgorithmsView() noexcept;
	DECLARE_DYNCREATE(CFillingAlgorithmsView)

// Attributes
public:
	CFillingAlgorithmsDoc* GetDocument() const;

// Operations
public:
	void translate(CDC* pDC, float dx, float dy, bool isRightMultiply = false);
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
	virtual ~CFillingAlgorithmsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in FillingAlgorithmsView.cpp
inline CFillingAlgorithmsDoc* CFillingAlgorithmsView::GetDocument() const
   { return reinterpret_cast<CFillingAlgorithmsDoc*>(m_pDocument); }
#endif



// IND_18016View.cpp : implementation of the CIND18016View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IND_18016.h"
#endif

#include "IND_18016Doc.h"
#include "IND_18016View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#define AREA_SIZE 500
#define SQUARE_NUM 20
#define SQUARE_SIZE (AREA_SIZE / SQUARE_NUM)

// CIND18016View

IMPLEMENT_DYNCREATE(CIND18016View, CView)

BEGIN_MESSAGE_MAP(CIND18016View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CIND18016View construction/destruction

CIND18016View::CIND18016View() noexcept
{
	//{ DImage(), 0, 0, false, false, 0, false },

	m_pieces = new PuzzlePiece[SIZE]{
		{ DImage(), 53, 61, true, false, -21, false },
		{ DImage(), 61, 56, true, false, 49, true },
		{ DImage(), 46, 58, true, false, -72, false },
		{ DImage(), 45, 50, false, true, 52, false },
		{ DImage(), 60, 48, false, true, -78, false },
		{ DImage(), 59, 48, false, true, -73, false },
		{ DImage(), 48, 59, true, false, -56, false },
		{ DImage(), 61, 53, true, false, 61, false },
		{ DImage(), 60, 49, false, true, -92, false },
	};

	for (int i = 0; i < SIZE; i++) {
		CString file;
		file.Format(L"./res/%d.bmp", i + 1);
		m_pieces[i].piece.Load(file);

		m_pieces[i].i = i / M;
		m_pieces[i].j = i % M;
	}
}

CIND18016View::~CIND18016View()
{
	delete[] m_pieces;
}

BOOL CIND18016View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIND18016View drawing

void CIND18016View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM m = {
	1, 0,
	0, 1,
	dX, dY
	};

	pDC->ModifyWorldTransform(&m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CIND18016View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM m = {
	sX, 0,
	0, sY
	};

	pDC->ModifyWorldTransform(&m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CIND18016View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	double radAngle = angle * M_PI / 180;

	XFORM m = {
		cos(radAngle), -sin(radAngle),
		sin(radAngle), cos(radAngle)
	};

	pDC->ModifyWorldTransform(&m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CIND18016View::Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply)
{
	XFORM m = {
	mx ? -1 : 1, 0,
	0, my ? -1 : 1
	};

	pDC->ModifyWorldTransform(&m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CIND18016View::DrawGrid(CDC* pDC, COLORREF color)
{
	CPen newPen(PS_SOLID, 0, color);
	CPen* oldPen = pDC->SelectObject(&newPen);

	for (int i = 0; i < SQUARE_NUM + 1; i++) {
		int pos = SQUARE_SIZE * i;

		// horizontal
		pDC->MoveTo(0, pos);
		pDC->LineTo(AREA_SIZE, pos);

		// vertical
		pDC->MoveTo(pos, 0);
		pDC->LineTo(pos, AREA_SIZE);
	}

	pDC->SelectObject(oldPen);

}

void CIND18016View::ApplyBlueFilter(CBitmap* bitmap)
{
	BITMAP bm;
	bitmap->GetBitmap(&bm);

	int size = bm.bmWidth * bm.bmHeight * bm.bmBitsPixel;
	unsigned char* bits = new unsigned char[size];

	bitmap->GetBitmapBits(size, bits);

	for (int i = 0; i < size; i += 4) {
		int val = bits[i] + 64;
		bits[i] = val > 255 ? 255 : val;
		bits[i + 1] = bits[i + 2] = bits[i + 3] = 0;
	}

	bitmap->SetBitmapBits(size, bits);

	delete[] bits;

}

void CIND18016View::ApplyGreyFilter(CBitmap* bitmap)
{
	BITMAP bm;
	bitmap->GetBitmap(&bm);

	int size = bm.bmWidth * bm.bmHeight * bm.bmBitsPixel;
	unsigned char* bits = new unsigned char[size];

	bitmap->GetBitmapBits(size, bits);

	for (int i = 0; i < size; i += 4) {
		int B = bits[i];
		int G = bits[i + 1];
		int R = bits[i + 2];
		int val = (R + G + B) / 3 + 64;

		bits[i] = bits[i + 1] = bits[i + 2] = bits[i + 3] = val > 255 ? 255 : val;
	}

	bitmap->SetBitmapBits(size, bits);

	delete[] bits;

}

void CIND18016View::DrawPuzzlePiece(CDC* pDC, PuzzlePiece& pc)
{
	XFORM oldXForm;
	pDC->GetWorldTransform(&oldXForm);

	constexpr int pieceSize = (SQUARE_NUM - 2) * SQUARE_SIZE / 3;
	POINT pt{ SQUARE_SIZE + pc.j * pieceSize, SQUARE_SIZE + pc.i * pieceSize };

	CBitmap bmpImage;
	bmpImage.CreateCompatibleBitmap(pDC, pc.piece.Width(), pc.piece.Height());
	bmpImage.SetBitmapBits(pc.piece.Width() * pc.piece.Height() * pc.piece.BPP(), pc.piece.GetDIBBits());

	CBitmap bmpMask;
	bmpMask.CreateBitmap(pc.piece.Width(), pc.piece.Height(), 1, 1, NULL);

	CDC srcDC;
	srcDC.CreateCompatibleDC(pDC);
	CDC dstDC;
	dstDC.CreateCompatibleDC(pDC);

	CBitmap* pOldSrcBmp = srcDC.SelectObject(&bmpImage);
	CBitmap* pOldDstBmp = dstDC.SelectObject(&bmpMask);

	COLORREF srcBkColor = srcDC.SetBkColor(srcDC.GetPixel(0, 0));
	dstDC.BitBlt(0, 0, pc.piece.Width(), pc.piece.Height(), &srcDC, 0, 0, SRCCOPY);

	if (pc.blueFilter) {
		ApplyBlueFilter(&bmpImage);
	}
	else {
		ApplyGreyFilter(&bmpImage);
	}

	COLORREF dstTextColor = srcDC.SetTextColor(RGB(255, 255, 255));
	srcDC.SetBkColor(RGB(0, 0, 0));
	srcDC.BitBlt(0, 0, pc.piece.Width(), pc.piece.Height(), &dstDC, 0, 0, SRCAND);

	srcDC.DeleteDC();
	dstDC.DeleteDC();

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	Translate(pDC, -pc.dx + pt.x + pc.piece.Width() / 2, -pc.dy + pt.y + pc.piece.Height() / 2, false);
	Rotate(pDC, pc.angle, false);
	Mirror(pDC, pc.mx, pc.my, false);
	Translate(pDC, -pt.x - pc.piece.Width() / 2, -pt.y - pc.piece.Height() / 2, false);

	CBitmap* old = memDC.SelectObject(&bmpMask);
	pDC->BitBlt(pt.x, pt.y, pc.piece.Width(), pc.piece.Height(), &memDC, 0, 0, SRCAND);

	memDC.SelectObject(&bmpImage);
	pDC->BitBlt(pt.x, pt.y, pc.piece.Width(), pc.piece.Height(), &memDC, 0, 0, SRCPAINT);
	memDC.SelectObject(old);

	pDC->SetWorldTransform(&oldXForm);
}

void CIND18016View::DrawPuzzle(CDC* pDC)
{
	int prevMode = pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM oldXForm;
	pDC->GetWorldTransform(&oldXForm);

	Translate(pDC, AREA_SIZE / 2, AREA_SIZE / 2, false);
	Rotate(pDC, 90, false);
	Translate(pDC, -AREA_SIZE / 2, -AREA_SIZE / 2, false);

	for (int i = 0; i < SIZE; i++) {
		DrawPuzzlePiece(pDC, m_pieces[i]);
	}

	pDC->SetWorldTransform(&oldXForm);
	pDC->SetGraphicsMode(prevMode);
}

void CIND18016View::OnDraw(CDC* pDC)
{
	CIND18016Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect clientRect;
	GetClientRect(&clientRect);

	CBitmap memBM;
	memBM.CreateCompatibleBitmap(pDC, clientRect.Width(), clientRect.Height());

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(&memBM);

	memDC.Rectangle(0, 0, clientRect.Width(), clientRect.Height());

	DrawGrid(&memDC, RGB(128, 128, 128));
	DrawPuzzle(&memDC);

	pDC->BitBlt(0, 0, clientRect.Width(), clientRect.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.DeleteDC();
	memBM.DeleteObject();
}


// CIND18016View printing

BOOL CIND18016View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIND18016View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIND18016View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CIND18016View diagnostics

#ifdef _DEBUG
void CIND18016View::AssertValid() const
{
	CView::AssertValid();
}

void CIND18016View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIND18016Doc* CIND18016View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIND18016Doc)));
	return (CIND18016Doc*)m_pDocument;
}
#endif //_DEBUG


// CIND18016View message handlers


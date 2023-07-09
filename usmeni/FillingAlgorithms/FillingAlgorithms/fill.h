#pragma once

#include "pch.h"

namespace fill
{
	void flood4(CDC* pDC, POINT pt, COLORREF oldVal, COLORREF newVal);
	void flood8(CDC* pDC, POINT pt, COLORREF oldVal, COLORREF newVal);
	void boundary4(CDC* pDC, POINT pt, COLORREF boundaryVal, COLORREF newVal);
	void boundary8(CDC* pDC, POINT pt, COLORREF boundaryVal, COLORREF newVal);
}
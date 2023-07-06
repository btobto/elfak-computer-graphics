
// IND_18016.h : main header file for the IND_18016 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CIND18016App:
// See IND_18016.cpp for the implementation of this class
//

class CIND18016App : public CWinApp
{
public:
	CIND18016App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CIND18016App theApp;


// Lab06P.h : main header file for the Lab06P application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CLab06PApp:
// See Lab06P.cpp for the implementation of this class
//

class CLab06PApp : public CWinApp
{
public:
	CLab06PApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CLab06PApp theApp;

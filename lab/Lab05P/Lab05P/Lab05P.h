
// Lab05P.h : main header file for the Lab05P application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CLab05PApp:
// See Lab05P.cpp for the implementation of this class
//

class CLab05PApp : public CWinApp
{
public:
	CLab05PApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CLab05PApp theApp;


// Lab04P.h : main header file for the Lab04P application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CLab04PApp:
// See Lab04P.cpp for the implementation of this class
//

class CLab04PApp : public CWinApp
{
public:
	CLab04PApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CLab04PApp theApp;


// Lab03P.h : main header file for the Lab03P application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CLab03PApp:
// See Lab03P.cpp for the implementation of this class
//

class CLab03PApp : public CWinApp
{
public:
	CLab03PApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CLab03PApp theApp;


// Lab02P.h : main header file for the Lab02P application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CLab02PApp:
// See Lab02P.cpp for the implementation of this class
//

class CLab02PApp : public CWinApp
{
public:
	CLab02PApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CLab02PApp theApp;

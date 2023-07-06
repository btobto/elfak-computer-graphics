
// Lab01F.h : main header file for the Lab01F application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CLab01FApp:
// See Lab01F.cpp for the implementation of this class
//

class CLab01FApp : public CWinApp
{
public:
	CLab01FApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CLab01FApp theApp;

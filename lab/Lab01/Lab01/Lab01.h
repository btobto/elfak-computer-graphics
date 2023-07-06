
// Lab01.h : main header file for the Lab01 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CLab01App:
// See Lab01.cpp for the implementation of this class
//

class CLab01App : public CWinApp
{
public:
	CLab01App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CLab01App theApp;

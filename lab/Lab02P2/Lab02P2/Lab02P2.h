
// Lab02P2.h : main header file for the Lab02P2 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CLab02P2App:
// See Lab02P2.cpp for the implementation of this class
//

class CLab02P2App : public CWinApp
{
public:
	CLab02P2App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CLab02P2App theApp;

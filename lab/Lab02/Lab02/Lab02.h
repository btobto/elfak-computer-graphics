
// Lab02.h : main header file for the Lab02 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CLab02App:
// See Lab02.cpp for the implementation of this class
//

class CLab02App : public CWinApp
{
public:
	CLab02App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CLab02App theApp;

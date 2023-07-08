
// Kol1_2018.h : main header file for the Kol1_2018 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKol12018App:
// See Kol1_2018.cpp for the implementation of this class
//

class CKol12018App : public CWinApp
{
public:
	CKol12018App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKol12018App theApp;

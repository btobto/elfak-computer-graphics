
// Kol1_2016.h : main header file for the Kol1_2016 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKol12016App:
// See Kol1_2016.cpp for the implementation of this class
//

class CKol12016App : public CWinApp
{
public:
	CKol12016App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKol12016App theApp;

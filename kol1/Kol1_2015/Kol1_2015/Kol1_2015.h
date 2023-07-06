
// Kol1_2015.h : main header file for the Kol1_2015 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKol12015App:
// See Kol1_2015.cpp for the implementation of this class
//

class CKol12015App : public CWinApp
{
public:
	CKol12015App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKol12015App theApp;

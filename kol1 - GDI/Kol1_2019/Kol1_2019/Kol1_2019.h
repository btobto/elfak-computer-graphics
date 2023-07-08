
// Kol1_2019.h : main header file for the Kol1_2019 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKol12019App:
// See Kol1_2019.cpp for the implementation of this class
//

class CKol12019App : public CWinApp
{
public:
	CKol12019App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKol12019App theApp;

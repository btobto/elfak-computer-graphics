
// FillingAlgorithms.h : main header file for the FillingAlgorithms application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CFillingAlgorithmsApp:
// See FillingAlgorithms.cpp for the implementation of this class
//

class CFillingAlgorithmsApp : public CWinApp
{
public:
	CFillingAlgorithmsApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CFillingAlgorithmsApp theApp;

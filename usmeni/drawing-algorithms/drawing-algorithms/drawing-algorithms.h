
// drawing-algorithms.h : main header file for the drawing-algorithms application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CdrawingalgorithmsApp:
// See drawing-algorithms.cpp for the implementation of this class
//

class CdrawingalgorithmsApp : public CWinApp
{
public:
	CdrawingalgorithmsApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CdrawingalgorithmsApp theApp;


// FaceMorph.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CFaceMorphApp:
// See FaceMorph.cpp for the implementation of this class
//

class CFaceMorphApp : public CWinApp
{
public:
	CFaceMorphApp();
protected: 
// Overrides
public:
	virtual BOOL InitInstance();
	
// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CFaceMorphApp theApp;

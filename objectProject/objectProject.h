// objectProject.h : main header file for the objectProject DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CobjectProjectApp
// See objectProject.cpp for the implementation of this class
//

class CobjectProjectApp : public CWinApp
{
public:
	CobjectProjectApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

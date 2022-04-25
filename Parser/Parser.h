
// Parser.h : main header file for the Parser application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

#define MM_REDRAWTREE				(WM_USER + 1)
#define MM_CHARGENEWDOC				(WM_USER + 2)
#define MM_CHARGEPROPERTIES			(WM_USER + 3)
#define MM_SETNODE					(WM_USER + 4)
#define MM_NEXTUNKNOW				(WM_USER + 5)
#define MM_CHANGEEXPRESSION			(WM_USER + 6)


int get_DeltaTimeMS(const SYSTEMTIME& s0, const SYSTEMTIME& s1);
// CParserApp:
// See Parser.cpp for the implementation of this class
//

class CParserApp : public CWinAppEx
{
public:
	CParserApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;
	bool  m_bClosing;

	CString getAppDataPath(const CString& filename) const;

private:
	void openLastDocumentsOpen(CMultiDocTemplate* pDocTemplate);

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CParserApp theApp;

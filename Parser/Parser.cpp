
// Parser.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Parser.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "ParserDoc.h"
#include "ParserView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CParserApp

BEGIN_MESSAGE_MAP(CParserApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CParserApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CParserApp construction

CParserApp::CParserApp()
{
	m_bHiColorIcons = TRUE;

	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Parser.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CParserApp object

CParserApp theApp;


#include "pmb_log.h"
// CParserApp initialization

BOOL CParserApp::InitInstance()
{
	std::map<std::string, CString> mapcmd;
	{
		bool bCmd = false,
			bValue = false;
		std::string cmd;
		CString value;
		for (LPTSTR lc = m_lpCmdLine; *lc; ++lc)
		{
			const char* c = reinterpret_cast<const char*>(lc);
			if ((c[0] == ' ' || c[0] == '\t') && bCmd && !cmd.empty())
			{
				bValue = true;
			}
			else if (c[0] == '-' && (m_lpCmdLine == lc || m_lpCmdLine < lc && ((lc - 1)[0] == ' ' || (lc - 1)[0] == '\t')))
			{
				if (bCmd && !cmd.empty() && bValue)
				{
					bValue = false;
					mapcmd[cmd] = value.Trim();
					cmd.clear();
					value.Empty();
				}
				bCmd = true;
			}
			else if (bCmd)
			{
				if (!bValue)
					cmd += c[0];
				else if (!cmd.empty())
					value += *lc;
			}
		}
		if (bCmd && !cmd.empty())
			mapcmd[cmd] = value;
	}

	pmb::log* log;
	{
		bool bColored = false;
		pmb::log_type loglevel;
		std::map<std::string, CString>::const_iterator ill = mapcmd.find("logLevel");
		if (ill == mapcmd.end())
			loglevel = pmb::logWarning;
		else
		{
			CString sloglevel = ill->second;
			if (4 < sloglevel.GetLength())
			{
				if (bColored = sloglevel.Right(2) == L"-c")
					sloglevel = sloglevel.Left(sloglevel.GetLength() - 2);
			}
			if (sloglevel == L"log-Error")
				loglevel = pmb::logError;
			else if (sloglevel == L"log-Warning")
				loglevel = pmb::logWarning;
			else if (sloglevel == L"log-Inf")
				loglevel = pmb::logInf;
			else if (sloglevel == L"log-Debug")
				loglevel = pmb::logDebug;
			else if (sloglevel == L"log-All")
				loglevel = pmb::logNone;
			else if (sloglevel == L"log-WTrace")
				loglevel = pmb::logWTrace;
			else
				loglevel = pmb::logWarning;
		}
		ill = mapcmd.find("logLevelFunction");
		bool bLevelFunction = ill != mapcmd.end() && ill->second == L"on";
		CStringA logfile;
		if (mapcmd.find("logFile") == mapcmd.end())
			logfile = "Parser-%YY%%MM%%DD%.log";
		else
			logfile = mapcmd["logFile"];
		if (mapcmd.find("logPath") == mapcmd.end())
		{
			WCHAR buffer[512];
			if (GetEnvironmentVariable(L"APPDATA", buffer, sizeof(buffer)))
				logfile = CStringA(buffer) + CStringA("\\") + logfile;
		}
		else
			logfile = mapcmd["logPath"] + "\\" + logfile;

		SYSTEMTIME st;
		GetLocalTime(&st);
		CStringA replace;
		replace.Format("%04d", st.wYear);
		logfile.Replace("%YYYY%", replace);
		replace.Format("%02d", st.wYear % 100);
		logfile.Replace("%YY%", replace);
		replace.Format("%02d", st.wMonth % 100);
		logfile.Replace("%MM%", replace);
		replace.Format("%02d", st.wDay % 100);
		logfile.Replace("%DD%", replace);
		log = pmb::log::instance(loglevel, logfile, bColored, bLevelFunction);
	}

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Calculator"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_ParserTYPE,
		RUNTIME_CLASS(CParserDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CParserView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd

	// Parse command line for standard shell commands, DDE, file open
//	CCommandLineInfo cmdInfo;
//	ParseCommandLine(cmdInfo);



	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
//	if (!ProcessShellCommand(cmdInfo))
//		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CParserApp::ExitInstance()
{
	delete pmb::log::instance();

	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CParserApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CParserApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CParserApp customization load/save methods

void CParserApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CParserApp::LoadCustomState()
{
}

void CParserApp::SaveCustomState()
{
}

// CParserApp message handlers




int get_DeltaTimeMS(const SYSTEMTIME& s0, const SYSTEMTIME& s1)
{
	return ((s1.wHour - s0.wHour) * 3600 + (s1.wMinute - s0.wMinute) * 60 + (s1.wSecond - s0.wSecond)) * 1000 + s1.wMilliseconds - s0.wMilliseconds;
}

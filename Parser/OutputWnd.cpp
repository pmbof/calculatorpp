
#include "stdafx.h"

#include "OutputWnd.h"
#include "Resource.h"
#include "MainFrm.h"
//#include "ParserDoc.h"
#include "Parser.h"

#include "pmb_parser_node.cpp"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputWnd::COutputWnd()
	: m_wndOutputDebug(true), m_wndOutputResult(false)
{
}

COutputWnd::~COutputWnd()
{
}

BEGIN_MESSAGE_MAP(COutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(MM_CHARGEPROPERTIES, &COutputWnd::OnChargeNewdoc)
END_MESSAGE_MAP()

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create tabs window:
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("Failed to create output tab window\n");
		return -1;      // fail to create
	}

	// Create output panes:
	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if (!m_wndOutputStadistics.Create(dwStyle, rectDummy, &m_wndTabs, 2) ||
		!m_wndOutputDebug.Create(NULL, NULL, dwStyle, rectDummy, &m_wndTabs, 3) ||
		!m_wndOutputFind.Create(dwStyle, rectDummy, &m_wndTabs, 4) ||
		!m_wndOutputResult.Create(NULL, NULL, dwStyle, rectDummy, &m_wndTabs, 5) )
	{
		TRACE0("Failed to create output windows\n");
		return -1;      // fail to create
	}

	UpdateFonts();

	CString strTabName;
	BOOL bNameValid;

	// Attach list windows to tab:
	m_wndTabs.AddTab(&m_wndOutputResult, L"Result", (UINT)0);
	bNameValid = strTabName.LoadString(IDS_DEBUG_TAB);

	bNameValid = strTabName.LoadString(IDS_BUILD_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputStadistics, _T("Stadistics"), (UINT)1);
	bNameValid = strTabName.LoadString(IDS_DEBUG_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputDebug, strTabName, (UINT)2);
	bNameValid = strTabName.LoadString(IDS_FIND_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputFind, strTabName, (UINT)3);

	// Fill output tabs with some dummy text (nothing magic here)
	FillStadisticsWindow();
//	FillDebugWindow();
	FillFindWindow();

	return 0;
}

void COutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// Tab control should cover the whole client area:
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void COutputWnd::AdjustHorzScroll(CListBox& wndListBox)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);

	int cxExtentMax = 0;

	for (int i = 0; i < wndListBox.GetCount(); i ++)
	{
		CString strItem;
		wndListBox.GetText(i, strItem);

		cxExtentMax = max(cxExtentMax, dc.GetTextExtent(strItem).cx);
	}

	wndListBox.SetHorizontalExtent(cxExtentMax);
	dc.SelectObject(pOldFont);
}



#define MY_MACRO_SIZECOUNT(TYPE) \
	int count ## TYPE, size ## TYPE;\
	if(count##TYPE = pNd->getMetricsNodes(pmb::parser::nd ## TYPE, size ## TYPE))\
	{\
		CString s("-" #TYPE " node count: %d");\
		str.Format(s, count ## TYPE);\
		m_wndOutputStadistics.AddString(str);\
		CString s1("   Size of " #TYPE  " node: %dB (%d x %dB = %dB) ");\
		str.Format(s1, size ## TYPE, count ## TYPE, size ## TYPE, size ## TYPE * count ## TYPE);\
		m_wndOutputStadistics.AddString(str);\
		str.Format(_T("      size of node*: %dB (%d x %dB= %dB)"), sizeof(pmb::parser::node<transporter>*), 3, sizeof(pmb::parser::node<transporter>*), 3 * sizeof(pmb::parser::node<transporter>*));\
		m_wndOutputStadistics.AddString(str);\
		str.Format(_T("      size of item: %dB"), sizeof(pmb::parser::item));\
		m_wndOutputStadistics.AddString(str);\
		str.Format(_T("      size of ndType: %dB"), sizeof(pmb::parser::ndtype));\
		m_wndOutputStadistics.AddString(str);\
	}


void COutputWnd::FillStadisticsWindow()
{
	CParserDoc* pDoc = CParserDoc::getDocument(this);
	if(!pDoc)
		return;
	const pmb::parser::node<transporter>* pNd = pDoc->getTree2()->getRootNode();
	int size = 0, 
		count = pNd->getMetricsNodes(size);
	CString str;
	str.Format(_T("Node count: %d"), count);
	m_wndOutputStadistics.AddString(str);
	str.Format(_T("Size of Tree: %dB = %.2fKB"), size, size / 1024.0);
	m_wndOutputStadistics.AddString(str);
	int dmsec = get_DeltaTimeMS(pDoc->m_time_ini, pDoc->m_time_end);
	str.Format(_T("Time elapsed: %02d:%02d:%02d.%03d"), dmsec / 1000 / 3600, dmsec / 1000 / 60 % 60, dmsec / 1000 % 60, dmsec % 1000);
	m_wndOutputStadistics.AddString(str);

	MY_MACRO_SIZECOUNT(Space);
	MY_MACRO_SIZECOUNT(Alpha);
	MY_MACRO_SIZECOUNT(Number);
	MY_MACRO_SIZECOUNT(String);
	MY_MACRO_SIZECOUNT(Parentheses);
	str.Format(_T("      size of _prtype: %dB"), sizeof(char));
	m_wndOutputStadistics.AddString(str);
	str.Format(_T("      size of _opened: %dB"), sizeof(int));
	m_wndOutputStadistics.AddString(str);
	str.Format(_T("      size of _bCalculated: %dB"), sizeof(bool));
	m_wndOutputStadistics.AddString(str);
	str.Format(_T("      size of _isVariableDependent: %dB"), sizeof(bool));
	m_wndOutputStadistics.AddString(str);
	MY_MACRO_SIZECOUNT(List);
	str.Format(_T("      size of _bCalculated: %dB"), sizeof(bool));
	m_wndOutputStadistics.AddString(str);
	str.Format(_T("      size of _isVariableDependent: %dB"), sizeof(bool));
	m_wndOutputStadistics.AddString(str);
	MY_MACRO_SIZECOUNT(Unknow);
	str.Format(_T("      size of _bCalculated: %dB"), sizeof(bool));
	m_wndOutputStadistics.AddString(str);
	str.Format(_T("      size of _isVariableDependent: %dB"), sizeof(bool));
	m_wndOutputStadistics.AddString(str);
	str.Format(_T("      size of operation<_TVALUE>*: %dB"), sizeof(void*));
	m_wndOutputStadistics.AddString(str);
	int ccount = countSpace + countAlpha + countNumber + countParentheses + countList + countUnknow;
	int csize = countSpace * sizeSpace + countAlpha * sizeAlpha + countNumber * sizeNumber + countParentheses * sizeParentheses + countList * sizeList + countUnknow * sizeUnknow;
	str.Format(_T("Total of nodes: %d, size %dB = %.2fkB"), ccount, csize, csize / 1024.0);
	m_wndOutputStadistics.AddString(str);

}

void COutputWnd::FillDebugWindow()
{
//	m_wndOutputDebug.AddString(_T("Debug output is being displayed here."));
//	m_wndOutputDebug.AddString(_T("The output is being displayed in rows of a list view"));
//	m_wndOutputDebug.AddString(_T("but you can change the way it is displayed as you wish..."));
}

void COutputWnd::FillFindWindow()
{
	m_wndOutputFind.AddString(_T("Find output is being displayed here."));
	m_wndOutputFind.AddString(_T("The output is being displayed in rows of a list view"));
	m_wndOutputFind.AddString(_T("but you can change the way it is displayed as you wish..."));
}

void COutputWnd::UpdateFonts()
{
	m_wndOutputStadistics.SetFont(&afxGlobalData.fontRegular);
	m_wndOutputDebug.SetFont(&afxGlobalData.fontRegular);
	m_wndOutputFind.SetFont(&afxGlobalData.fontRegular);
}

/////////////////////////////////////////////////////////////////////////////
// COutputList1

COutputList::COutputList()
{
}

COutputList::~COutputList()
{
}

BEGIN_MESSAGE_MAP(COutputList, CListBox)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputList message handlers

void COutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}

void COutputList::OnEditCopy()
{
	MessageBox(_T("Copy output"));
}

void COutputList::OnEditClear()
{
	MessageBox(_T("Clear output"));
}

void COutputList::OnViewOutput()
{
	CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

	if (pMainFrame != NULL && pParentBar != NULL)
	{
		pMainFrame->SetFocus();
		pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
		pMainFrame->RecalcLayout();

	}
}




afx_msg LRESULT COutputWnd::OnChargeNewdoc(WPARAM wParam, LPARAM lParam)
{
	FillStadisticsWindow();
	m_wndOutputResult.RedrawWindow();
	return 0;
}

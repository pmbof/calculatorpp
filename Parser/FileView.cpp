
#include "stdafx.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"
#include "Parser.h"
#include "ParserDoc.h"


#include "pmb_parser_operation.cpp"
#include "pmb_parser_function.cpp"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView

CFileView::CFileView()
{
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_MESSAGE(MM_CHARGEPROPERTIES, &CFileView::OnChargeNewDoc)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create view:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}

	// Load view images:
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// Fill in some static tree view data (dummy code, nothing magic here)
	AdjustLayout();

	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFileView::FillFileView(WPARAM wParam)
{
	const pmb::parser::symbol<transporter>::_tpMMap* map = reinterpret_cast<const pmb::parser::symbol<transporter>::_tpMMap*>(wParam);

	HTREEITEM hRoot = m_wndFileView.InsertItem(_T("Symbols"), 0, 0);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	for(pmb::parser::symbol<transporter>::_tpMMap::const_iterator i = map->begin(); i != map->end(); ++i)
	{
		HTREEITEM hSrc = m_wndFileView.InsertItem(CString(i->first.c_str()), 0, 0, hRoot);
		for(pmb::parser::symbol<transporter>::_tpMap::const_iterator j = i->second->begin(); j != i->second->end(); ++j)
		{
			CString str;
			str.Format(_T(" <0x%08X>"), *j->second);
			str = CString(j->first.c_str()) + str;
			HTREEITEM hVar = m_wndFileView.InsertItem(str, 1, 1, hSrc);
			if (!j->second.isNull())
			{
				std::stringstream ss;
				j->second->stringstream(ss);
				str = ss.str().c_str();
				m_wndFileView.InsertItem(str, 2, 2, hVar);
			}
			str = "j->second._getState()";
			m_wndFileView.InsertItem(str, 2, 2, hVar);
		}
		m_wndFileView.Expand(hSrc, TVE_EXPAND);
	}
	m_wndFileView.Expand(hRoot, TVE_EXPAND);

	hRoot = m_wndFileView.InsertItem(_T("Functions"), 0, 0);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
/*	pmb::calculator<block>::_tdFncTable fncTable;
	for(int i = 0; i < fncTable.size(); ++i)
	{
		const pmb::parser::function<transporter>* fnc = fncTable.get(i);
		CString str(fnc->getName());
		str += CString(L" \'") + CString(fnc->getDescription()) + L"\'";
		HTREEITEM hSrc = m_wndFileView.InsertItem(CString(str), 0, 0, hRoot);
		str = L"Name: " + CString(fnc->getName());
		m_wndFileView.InsertItem(str, 1, 1, hSrc);
		str = L"Description: " + CString(fnc->getDescription());
		m_wndFileView.InsertItem(str, 1, 1, hSrc);
		str.Format(L"Arguments: %d", fnc->getNArgs());
		m_wndFileView.InsertItem(str, 1, 1, hSrc);
	}//*/

	hRoot = m_wndFileView.InsertItem(_T("Operators"), 0, 0);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
/*	pmb::calculator<block, operation_table>::_tdOprTable* oprTable = ;
	for(int i = 0; i < oprTable.size(); ++i)
	{
		const pmb::parser::operation<transporter>* opr = oprTable.get(i);
		CString str(opr->getSymbol());
		str += CString(L" \'") + CString(opr->getName()) + L"\'";
		HTREEITEM hSrc = m_wndFileView.InsertItem(CString(str), 0, 0, hRoot);
		if(opr->canCallFunction())
		{
			str = L"Can Call function!";
			m_wndFileView.InsertItem(str, 1, 1, hSrc);
		}
		str = L"Symbol: " + CString(opr->getSymbol());
		m_wndFileView.InsertItem(str, 1, 1, hSrc);
		str = L"Name: " + CString(opr->getName());
		m_wndFileView.InsertItem(str, 1, 1, hSrc);
		str = L"Description: " + CString(opr->getDescription());
		m_wndFileView.InsertItem(str, 1, 1, hSrc);
		str.Format(L"Precedence: %d", opr->getPrecedence());
		m_wndFileView.InsertItem(str, 1, 1, hSrc);
		str = L"Association: " + CString(opr->isLeftToRight() ? L"Left to right": L"Right to Left");
		m_wndFileView.InsertItem(str, 1, 1, hSrc);
		str = L"Operator: " + CString(opr->isBinary() ? L"Binary": L"Mono");
		m_wndFileView.InsertItem(str, 1, 1, hSrc);
	}//*/
}


void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Select clicked item:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFileView::OnProperties()
{
	AfxMessageBox(_T("Properties...."));

}

void CFileView::OnFileOpen()
{
	// TODO: Add your command handler code here
}

void CFileView::OnFileOpenWith()
{
	// TODO: Add your command handler code here
}

void CFileView::OnDummyCompile()
{
	// TODO: Add your command handler code here
}

void CFileView::OnEditCut()
{
	// TODO: Add your command handler code here
}

void CFileView::OnEditCopy()
{
	// TODO: Add your command handler code here
}

void CFileView::OnEditClear()
{
	// TODO: Add your command handler code here
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void CFileView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* Locked */);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}




afx_msg LRESULT CFileView::OnChargeNewDoc(WPARAM wParam, LPARAM lParam)
{
	FillFileView(wParam);
	return 0;
}

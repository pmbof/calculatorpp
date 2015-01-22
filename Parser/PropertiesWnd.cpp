
#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "Parser.h"
#include "ParserDoc.h"
#include "pmb_parser_transporter.cpp"
#include "pmb_parser_operation.cpp"
#include "pmb_parser_nodes_parentheses.h"

#include "pmb_parser_nodes_unknow.h"

#include "pmb_parser_nodes_unknow.cpp"
#include "pmb_parser_node.cpp"
#include "pmb_parser_nodes_calc.cpp"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
	: m_pPropNode(NULL), m_pPropUnknowNode(NULL)
{
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_MESSAGE(MM_CHARGENEWDOC, &CPropertiesWnd::OnChargeNewDoc)
	ON_MESSAGE(MM_CHARGEPROPERTIES, &CPropertiesWnd::OnChargeProperties)
	ON_MESSAGE(MM_SETNODE, &CPropertiesWnd::OnSetnode)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);

	m_wndObjectCombo.GetWindowRect(&rectCombo);

	int cyCmb = rectCombo.Size().cy;
	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), 200, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create combo:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Failed to create Properties Combo \n");
		return -1;      // fail to create
	}

	m_wndObjectCombo.AddString(_T("Application"));
	m_wndObjectCombo.AddString(_T("Properties Window"));
	m_wndObjectCombo.SetCurSel(0);

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

	InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* Is locked */);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* Locked */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
	// TODO: Add your command handler code here
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: Add your command handler code here
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void CPropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
	m_wndObjectCombo.SetFont(&m_fntPropList);
}


afx_msg LRESULT CPropertiesWnd::OnChargeProperties(WPARAM wParam, LPARAM lParam)
{

	CMFCPropertyGridProperty* pExprGrp = new CMFCPropertyGridProperty(_T("Expression"));
	CParserDoc* pDoc = static_cast<CParserDoc*>(GetParentFrame()->GetActiveFrame()->GetActiveDocument());
	if(pDoc && pDoc->IsKindOf(RUNTIME_CLASS(CParserDoc)))
	{
		CString str(pDoc->m_expr);
		CString name;
		name.Format(_T("Expression[%d]: \"%s\""), str.GetLength(), (LPCTSTR)str);
		pExprGrp->SetName(name);
		for(int i = 0; pDoc->m_expr[i]; ++i)
		{
			CString pos;
			pos.Format(L"%d", i);
			CString val;
			val.Format(L"%c[%#02x]", pDoc->m_expr[i], (int)pDoc->m_expr[i]);
			pExprGrp->AddSubItem(new CMFCPropertyGridProperty(pos, val, _T("Specifies the window's font will be non-bold and controls will have a 3D border")));
		}
		pExprGrp->Expand(false);
		m_wndPropList.AddProperty(pExprGrp);
		
		int size = 0, count = pDoc->getTree2()->getRootNode()->getMetricsNodes(size);
		pExprGrp = new CMFCPropertyGridProperty(_T("Nodes:"), 0, true);
		str.Format(L"%d", count);
		pExprGrp->AddSubItem(new CMFCPropertyGridProperty(_T("count"), str, _T("Nodes amount")));
		str.Format(L"%dB", size);
		pExprGrp->AddSubItem(new CMFCPropertyGridProperty(_T("size"), str, _T("Size of tree")));
//		str.Format(L"%02d:%02d:%02d.%03d", pDoc->m_time_ini.wHour, pDoc->m_time_ini.wMinute, pDoc->m_time_ini.wSecond, pDoc->m_time_ini.wMilliseconds);
//		pExprGrp->AddSubItem(new CMFCPropertyGridProperty(_T("To"), str, _T("time take to parser")));
//		str.Format(L"%02d:%02d:%02d.%03d", pDoc->m_time_end.wHour, pDoc->m_time_end.wMinute, pDoc->m_time_end.wSecond, pDoc->m_time_end.wMilliseconds);
//		pExprGrp->AddSubItem(new CMFCPropertyGridProperty(_T("T"), str, _T("time take to parser")));
		str.Format(L"%02d:%02d:%02d.%03d", pDoc->m_time_end.wHour - pDoc->m_time_ini.wHour, pDoc->m_time_end.wMinute - pDoc->m_time_ini.wMinute, pDoc->m_time_end.wSecond - pDoc->m_time_ini.wSecond, pDoc->m_time_end.wMilliseconds - pDoc->m_time_ini.wMilliseconds);
		pExprGrp->AddSubItem(new CMFCPropertyGridProperty(_T("Elapsed"), str, _T("time elapsed")));
		m_wndPropList.AddProperty(pExprGrp);
	}

/*	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("Expression"), _T("Dialog Frame"), _T("One of: None, Thin, Resizable, or Dialog Frame"));
	pProp->AddOption(_T("None"));
	pProp->AddOption(_T("Thin"));
	pProp->AddOption(_T("Resizable"));
	pProp->AddOption(_T("Dialog Frame"));
	pProp->AllowEdit(FALSE);

	pGroup1->AddSubItem(pProp);
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Caption"), (_variant_t) _T("About"), _T("Specifies the text that will be displayed in the window's title bar")));
*/

/*	CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("Window Size"), 0, TRUE);

	pProp = new CMFCPropertyGridProperty(_T("Height"), (_variant_t) 250l, _T("Specifies the window's height"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	pSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty( _T("Width"), (_variant_t) 150l, _T("Specifies the window's width"));
	pProp->EnableSpinControl(TRUE, 50, 200);
	pSize->AddSubItem(pProp);

	m_wndPropList.AddProperty(pSize);

	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("Font"));

	LOGFONT lf;
	CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	font->GetLogFont(&lf);

	lstrcpy(lf.lfFaceName, _T("Arial"));

	pGroup2->AddSubItem(new CMFCPropertyGridFontProperty(_T("Font"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("Specifies the default font for the window")));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("Use System Font"), (_variant_t) true, _T("Specifies that the window uses MS Shell Dlg font")));

	m_wndPropList.AddProperty(pGroup2);

	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("Misc"));
	pProp = new CMFCPropertyGridProperty(_T("(Name)"), _T("Application"));
	pProp->Enable(FALSE);
	pGroup3->AddSubItem(pProp);

	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("Window Color"), RGB(210, 192, 254), NULL, _T("Specifies the default window color"));
	pColorProp->EnableOtherButton(_T("Other..."));
	pColorProp->EnableAutomaticButton(_T("Default"), ::GetSysColor(COLOR_3DFACE));
	pGroup3->AddSubItem(pColorProp);

	static const TCHAR szFilter[] = _T("Icon Files(*.ico)|*.ico|All Files(*.*)|*.*||");
	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Icon"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("Specifies the window icon")));

	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Folder"), _T("c:\\")));

	m_wndPropList.AddProperty(pGroup3);

	CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("Hierarchy"));

	CMFCPropertyGridProperty* pGroup41 = new CMFCPropertyGridProperty(_T("First sub-level"));
	pGroup4->AddSubItem(pGroup41);

	CMFCPropertyGridProperty* pGroup411 = new CMFCPropertyGridProperty(_T("Second sub-level"));
	pGroup41->AddSubItem(pGroup411);

	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Item 1"), (_variant_t) _T("Value 1"), _T("This is a description")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Item 2"), (_variant_t) _T("Value 2"), _T("This is a description")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Item 3"), (_variant_t) _T("Value 3"), _T("This is a description")));

	pGroup4->Expand(FALSE);
	m_wndPropList.AddProperty(pGroup4);*/
	return 0;
}


afx_msg LRESULT CPropertiesWnd::OnChargeNewDoc(WPARAM wParam, LPARAM lParam)
{
	return PostMessage(MM_CHARGEPROPERTIES);
}






afx_msg LRESULT CPropertiesWnd::OnSetnode(WPARAM wParam, LPARAM lParam)
{
	CMFCPropertyGridProperty* prp = lParam ? m_pPropUnknowNode: m_pPropNode;
	if(prp)
	{
		m_wndPropList.SetCurSel(NULL);
		m_wndPropList.DeleteProperty(prp);
	}
	(lParam ? m_pPropUnknowNode: m_pPropNode) = prp = getPropertyNode(lParam ? lParam: wParam, NULL, NULL);
	if(prp)
		m_wndPropList.AddProperty(prp);
	return 0;
}

CMFCPropertyGridProperty* CPropertiesWnd::getPropertyNode(WPARAM wParam, LPARAM lParam, CMFCPropertyGridProperty* pNode) const
{
	const pmb::parser::node<transporter>* pNd = reinterpret_cast<pmb::parser::node<transporter>*>(wParam);
	const pmb::parser::node<transporter>* pNdBase = reinterpret_cast<pmb::parser::node<transporter>*>(lParam);
	if(!pNd)
		return NULL;

	static const char* types[] = {"AAlpha", "NNumber", "SSpace", "PParenthesis", "UUnknow", "sString", "LList"};
	CString str("Node");
	if(pNode)
		str = pNode->GetName();
	CString expr;
	CParserDoc* pDoc = static_cast<CParserDoc*>(GetParentFrame()->GetActiveFrame()->GetActiveDocument());
	if(pDoc && pDoc->IsKindOf(RUNTIME_CLASS(CParserDoc)))
	{
		str += L"[";
		pmb::parser::item::string se = pNd->getString(pDoc->m_expr);
		for(unsigned int i = 0; se[i]; ++i)
			expr += se[i];
		str += expr;
		str += L"]";
	}
	int t;
	for(t = 0; t < sizeof(types) / sizeof(*types) && *types[t] != pNd->getType(); ++t)
		;
	str += L"<";
	str += t < sizeof(types) / sizeof(*types) ? CString(types[t] + 1): CString(pNd->getType());
	str += L">";

	if(!pNode)
		pNode = new CMFCPropertyGridProperty(str, 0);
	else
		pNode->SetName(str);
	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("ini"), (_variant_t)pNd->getIni(), _T("First position"));
	pNode->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty( _T("end"), (_variant_t)pNd->getEnd(), _T("Last position + 1"));
	pNode->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty( _T("lenght"), (_variant_t)(pNd->len()), _T("Lenght of item"));
	pNode->AddSubItem(pProp);
	CString type;
	type.Format(_T("%c <%s>"), pNd->getType(), t < sizeof(types) / sizeof(*types) ? (LPCTSTR)CString(types[t] + 1): NULL);
	pProp = new CMFCPropertyGridProperty( _T("type"), (_variant_t)(type), _T("Type of item"));
	pNode->AddSubItem(pProp);
	if(pNd->getType() == pmb::parser::ndParentheses)
	{
		const pmb::parser::nodes::parentheses<transporter>* pParen = static_cast<const pmb::parser::nodes::parentheses<transporter>*>(pNd);

		pProp = new CMFCPropertyGridProperty( _T("Opened"), (_variant_t)(pParen->getOpened()), _T("Type of item"));
		pNode->AddSubItem(pProp);

		type.Format(_T("%c"), pParen->getClass());
		pProp = new CMFCPropertyGridProperty( _T("class"), (_variant_t)(type), _T("Type of item"));
		pNode->AddSubItem(pProp);
	}
	else if(pNd->getType() == pmb::parser::ndUnknow)
	{
		const pmb::parser::nodes::unknow<transporter>* pUn = static_cast<const pmb::parser::nodes::unknow<transporter>*>(pNd);

		if(pUn->getOperation())
		{
			CString descr(pUn->getOperation()->getDescription());
			pProp = new CMFCPropertyGridProperty( _T("Operation"), (_variant_t)(pUn->getOperation()->getName()), descr);
			pProp->Enable(false);
			pNode->AddSubItem(pProp);
			pProp = new CMFCPropertyGridProperty( _T("Binary"), (_variant_t)(pUn->getOperation()->isBinary()), _T("Operation binary"));
			pProp->Enable(false);
			pNode->AddSubItem(pProp);
			pProp = new CMFCPropertyGridProperty( _T("Left to right"), (_variant_t)(pUn->getOperation()->isLeftToRight()), _T("Association order"));
			pProp->Enable(false);
			pNode->AddSubItem(pProp);
			pProp = new CMFCPropertyGridProperty( _T("Priority"), (_variant_t)(pUn->getOperation()->getPrecedence()), _T("Precendence"));
			pProp->Enable(false);
			pNode->AddSubItem(pProp);
			if(pUn->getOperation()->isBinary())
				pProp = new CMFCPropertyGridHexProperty( _T("ptr function2"), (_variant_t)(pUn->getOperation()->getFunctor2()), _T("pointer to binary function"));
			else
				pProp = new CMFCPropertyGridHexProperty( _T("ptr function1"), (_variant_t)(pUn->getOperation()->getFunctor1()), _T("pointer to unitary function"));
			pProp->Enable(false);
			pNode->AddSubItem(pProp);
		}
	}
	if(pNd->isCalcType())
	{
		const transporter& val =	pNd->getType() == pmb::parser::ndUnknow ? static_cast<const pmb::parser::nodes::unknow<transporter>*>(pNd)->getValue():
							pNd->getType() == pmb::parser::ndParentheses ? static_cast<const pmb::parser::nodes::parentheses<transporter>*>(pNd)->getValue():
							static_cast<const pmb::parser::nodes::list<transporter>*>(pNd)->getRValue();
		pProp = new CMFCPropertyGridProperty(pNd->getType() == pmb::parser::ndList ? _T("rValue"): _T("Value"), 0, true);
		pNode->AddSubItem(pProp);
		pProp->AddSubItem(new CMFCPropertyGridHexProperty(_T("Value Ptr"), (_variant_t)((UINT)*val), _T("Pointer to value")));
		if(*val)
		{
			transporter val1;
			val1->_number;
			pProp->AddSubItem(new CMFCPropertyGridProperty(_T("Value"), (_variant_t)(val->_number), _T("value")));
			pProp->AddSubItem(new CMFCPropertyGridProperty(_T("Type"), (_variant_t)(val._getState()), _T("Can delete this value")));
		}
		pProp->Expand();
		if(pNd->getType() == pmb::parser::ndList)
		{
			const transporter& lVal = static_cast<const pmb::parser::nodes::list<transporter>*>(pNd)->getLValue();
			pProp = new CMFCPropertyGridProperty(_T("lValue"), 0, true);
			pNode->AddSubItem(pProp);
			pProp->AddSubItem(new CMFCPropertyGridProperty(_T("Value Ptr"), (_variant_t)((UINT)*lVal), _T("Pointer to left value")));
			if(*lVal)
			{
				pProp->AddSubItem(new CMFCPropertyGridProperty(_T("Value"), (_variant_t)(lVal->_number), _T("value")));
				pProp->AddSubItem(new CMFCPropertyGridProperty(_T("Type"), (_variant_t)(lVal._getState()), _T("Can delete this value")));
			}
			pProp->Expand();
		}
	}
	if(pNd->isCalcType())
	{
		const pmb::parser::nodes::calc<transporter>* pCalc = static_cast<const pmb::parser::nodes::calc<transporter>*>(pNd);
		pProp = new CMFCPropertyGridProperty( _T("is Calculated"), (_variant_t)(pCalc->isCalculated()), _T("Node calculated"));
		pNode->AddSubItem(pProp);
		pProp = new CMFCPropertyGridProperty( _T("is Variable dependent"), (_variant_t)(pCalc->isVariableDependent()), _T("Node variable dependent"));
		pNode->AddSubItem(pProp);
	}

	expr = _T("'") + expr + _T("'");
	CMFCPropertyGridProperty* pPropExpr = new CMFCPropertyGridProperty(expr, 0);
	if(pDoc && pDoc->IsKindOf(RUNTIME_CLASS(CParserDoc)))
	{
		pmb::parser::item::string se = pNd->getString(pDoc->m_expr);
		for(unsigned int i = 0; se[i]; ++i)
		{
			CString pos;
			pos.Format(L"%d", i + pNd->getIni());
			CString val;
			val.Format(L"%c[%#02x]", se[i], (int)se[i]);
			pPropExpr->AddSubItem(new CMFCPropertyGridProperty(pos, val, _T("Specifies the window's font will be non-bold and controls will have a 3D border")));
		}
	}
	pPropExpr->Expand(false);
	pNode->AddSubItem(pPropExpr);

	if(pNd->getParent())
	{
		CMFCPropertyGridProperty* pPropParent = new CMFCPropertyGridProperty(_T("Parent"), 0);
		if(pNd->getParent()->getLeft() == pNd)
		{
			pPropParent->SetName(_T("Parent <left>"));
			pPropParent->SetDescription(_T("This node is the LEFT children"));
		}
		else if(pNd->getParent()->getRight() == pNd)
		{
			pPropParent->SetName(_T("Parent <right>"));
			pPropParent->SetDescription(_T("This node is the RIGHT children"));
		}
		else
		{
			pPropParent->SetName(_T("Parent <ERROR>"));
			pPropParent->SetDescription(_T("Error:\nThis node has a parent that not has this node to the left or right!"));
		}
		if(pNd->getParent() != pNdBase)
			getPropertyNode(reinterpret_cast<WPARAM>(pNd->getParent()), reinterpret_cast<LPARAM>(pNd), pPropParent);
		pPropParent->Expand(false);
		pNode->AddSubItem(pPropParent);
	}
	if(pNd->getLeft())
	{
		CMFCPropertyGridProperty* pPropLeft = new CMFCPropertyGridProperty(_T("Left"), 0);
		if(pNd->getLeft()->getParent() != pNd)
		{
			pPropLeft->SetName(_T("Left <ERROR>"));
			pPropLeft->SetDescription(_T("Error:\nThe parent of left node must be this node!"));
		}
		if(pNd->getLeft() != pNdBase)
			getPropertyNode(reinterpret_cast<WPARAM>(pNd->getLeft()), reinterpret_cast<LPARAM>(pNd), pPropLeft);
		pPropLeft->Expand(false);
		pNode->AddSubItem(pPropLeft);
	}
	if(pNd->getRight())
	{
		CMFCPropertyGridProperty* pPropRight = new CMFCPropertyGridProperty(_T("Right"), 0);
		if(pNd->getRight()->getParent() != pNd)
		{
			pPropRight->SetName(_T("Right <ERROR>"));
			pPropRight->SetDescription(_T("Error:\nThe parent of right node must be this node!"));
		}
		if(pNd->getRight() != pNdBase)
			getPropertyNode(reinterpret_cast<WPARAM>(pNd->getRight()), reinterpret_cast<LPARAM>(pNd), pPropRight);
		pPropRight->Expand(false);
		pNode->AddSubItem(pPropRight);
	}

	return pNode;
}

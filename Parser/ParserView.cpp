
// ParserView.cpp : implementation of the CParserView class
//

#pragma region includes
#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Parser.h"
#endif

#include "ParserDoc.h"
#include "ParserView.h"
#include "pmb_parser_node.cpp"
#pragma endregion includes

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CParserView

IMPLEMENT_DYNCREATE(CParserView, CView)

BEGIN_MESSAGE_MAP(CParserView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CParserView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_ITERATION_MOVE_NEXT, &CParserView::OnIterationMoveNext)
	ON_MESSAGE(MM_SETNODE, &CParserView::OnSetnode)
	ON_COMMAND(ID_NEXTUNKNOW, &CParserView::OnNextunknow)
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_CHAR()
END_MESSAGE_MAP()

// CParserView construction/destruction

CParserView::CParserView()
	: m_pNd(NULL), m_pNdUnk(NULL), m_resource(this)
{
}


CParserView::~CParserView()
{
}


BOOL CParserView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}



int CParserView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CFont* pFont = m_resource.init(true, 120, L"Times New Roman", RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00));
	//CFont* pFont = m_resource.init(true, 120, L"Mathcad UnMath Prime", RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00));

	_scaleNum = CSize(150, 150);
	_scaleDen = CSize(100, 100);

	LOGFONT lf;
	pFont->GetLogFont(&lf);
	//lf.lfItalic = TRUE;
	m_resource.add_style("@numbers", &lf, RGB(0x40, 0x40, 0x40));
	lf.lfItalic = FALSE;
	lf.lfWeight = FW_BOLD;
	m_resource.add_style("@operators", &lf, RGB(0x60, 0x00, 0x00));
	m_resource.add_style("@variables", 120, L"Times New Roman Italic", RGB(0x00, 0x00, 0x80));
	CFont* pFonti = m_resource.font("@variables", 0);
	LOGFONT lfi;
	pFonti->GetLogFont(&lfi);
	m_resource.add_style("@Build-in functions", &lfi, RGB(0x00, 0x00, 0x00));
	m_resource.add_style("@user-def functions", &lfi, RGB(0x00, 0x60, 0x60));
	m_resource.add_style("@parentheses", &lf, RGB(0x00, 0x00, 0x00));
	m_resource.add_style("@list", &lf, RGB(0x00, 0x00, 0x00));
	m_resource.add_style("@strings", &lf, RGB(0x00, 0x80, 0x00));

	m_p0 = CPoint(10, 10);

	CMFCToolTipInfo params;
	params.m_bBalloonTooltip = TRUE;
	params.m_bBoldLabel = TRUE;
	params.m_bDrawDescription = TRUE;
	params.m_bDrawIcon = TRUE;
	params.m_bRoundedCorners = TRUE;
	params.m_bDrawSeparator = TRUE;
	params.m_clrFill = RGB(0xFF, 0x00, 0x00);
	params.m_clrFillGradient = RGB(228, 228, 240);
	params.m_clrText = RGB(0xFF, 0xFF, 0xFF);
	params.m_clrBorder = RGB(144, 149, 168);

	m_tooltip.SetParams(&params);
	m_tooltip.Create(this, TTS_ALWAYSTIP);
	m_tooltip.SetParams(&params);
	m_tooltip.SetFixedWidth(400, 550);
	m_tooltip.AddTool(this, L"");
	m_tooltipId = -1;
	EnableToolTips(TRUE);

	m_bShowResult = true;
	return 0;
}




void CParserView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_fstyle = PMB_STYLE_CPPEDIT_MULTILINE;
	m_bEditing = true;

	CParserDoc* pDoc = GetDocument();
	pDoc->print_operation_table();

	m_caret.pos[1] = m_p0;
	--m_caret.pos[1].x;
	m_caret.pos[0] = m_caret.pos[1];
	m_caret.spos[0] = m_caret.spos[1] = pDoc->m_expr.size();

	if (m_resource.pretty())
	{
		if (m_line.empty())
			m_line.push_back(this);

		CDC* pDC = GetDC();
		m_line[0].set(pDC, m_p0.x, m_p0.y);
		ReleaseDC(pDC);
		m_line[0](m_caret);
	}
}


// CParserView drawing

void CParserView::OnDraw(CDC* pDC)
{
	pDC->SetMapMode(MM_ISOTROPIC);
	pDC->SetViewportExt(_scaleNum);
	pDC->SetWindowExt(_scaleDen);
	if (m_resource.pretty())
		m_line[0].draw(pDC);
	else
		draw_line(pDC, false);
}





void CParserView::draw_line(CDC* pDC, bool bCalc, int* x_pos)
{
	CParserDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	const tnode* nd = pDoc->getNodeRoot();
	if (!nd)
		return;


	if (bCalc && (x_pos && *x_pos < m_p0.x || !x_pos && !m_caret.spos[1]))
	{
		if (x_pos)
			m_caret.spos[1] = 0;
		m_caret.pos[1].x = m_p0.x;
		return;
	}

	if (!bCalc)
		HideCaret();

	CString expr(pDoc->m_expr.c_str());
	CRect wr;
	GetClientRect(&wr);

	CPen pen;
	if (!bCalc)
	{
		pen.CreatePen(PS_SOLID, 1, m_resource.color());
		pDC->SetBkMode(OPAQUE);
	}
	bool bResult = false;

	COLORREF oldColor;
	CFont* oldFont = nullptr;
	CPen* oldPen = bCalc ? nullptr : pDC->SelectObject(&pen);
	CRect tr(m_p0.x, m_p0.y, m_p0.x, m_p0.y + m_resource.max_height());
	int max_x = m_p0.x;
	int end = 0;
	for (nd = nd->getFirstNode(); nd; nd = nd->getNextNode())
	{
		CFont* of;
		if (end < nd->getIni())
		{
			of = pDC->SelectObject(m_resource.font(0));
			int width;
			pDC->GetOutputCharWidth(' ', ' ', &width);
			tr.right += width * (nd->getIni() - end);

			if (!bCalc)
				pDC->FillSolidRect(tr, m_resource.back_color());
			else
			{
				if (x_pos && *x_pos < tr.right)
				{
					m_caret.spos[1] = nd->getIni() + (*x_pos - tr.left) / width;
					m_caret.pos[1].x = tr.left + width * (m_caret.spos[1] - nd->getIni());
					break;
				}
				else if (!x_pos && m_caret.spos[1] <= nd->getIni())
				{
					m_caret.pos[1].x = tr.left + width * (m_caret.spos[1] - nd->getIni() + 1);
					break;
				}
			}
			tr.left = tr.right;
			if (!oldFont)
				pDC->SelectObject(of);
		}
		COLORREF oc;
		std::string style;
		switch (nd->getType())
		{
		case pmb::parser::ndSpace:
			style = "@operators";
			break;
		case pmb::parser::ndAlpha:
			style = "@variables";
			if (nd->getParent() && nd->getParent()->getType() == pmb::parser::ndUnknown)
			{
				const pmb::parser::nodes::unknown<item, ndtype>* uk = static_cast<const pmb::parser::nodes::unknown<item, ndtype>*>(nd->getParent());
				if (uk->isBinary() && uk->isCallFunction() && (uk->isFirstLeft() && uk->getLeft() == nd || uk->isFirstRight() && uk->getRight() == nd))
					style = uk->isCallBuildInFunction() ? "@Build-in functions" : "@user-def functions";
			}
			break;
		case pmb::parser::ndNumber:
			style = "@numbers";
			break;
		case pmb::parser::ndString:
			style = "@strings";
			break;
		case pmb::parser::ndList:
			style = "@list";
			break;
		case pmb::parser::ndParentheses:
			style = "@parentheses";
			break;
		case pmb::parser::ndUnknown:
			style = "@operators";
			break;
		}
		CFont* pFont = m_resource.font(style, 0);
		of = pDC->SelectObject(pFont);
		if (!bCalc)
			oc = pDC->SetTextColor(m_resource.color(style));
		if (!oldFont)
		{
			oldFont = of;
			if (!bCalc)
				oldColor = oc;
		}
		CString sn(pDoc->m_expr.c_str() + nd->getIni(), nd->len());
		CRect ctr = tr;
		ctr.right += 100;
		pDC->DrawText(sn, ctr, DT_CALCRECT | DT_LEFT | DT_TOP | DT_SINGLELINE);
		tr.right = tr.left + ctr.Width();

		LOGFONT lf;
		pFont->GetLogFont(&lf);
		if (lf.lfItalic)
			tr.right += 2;

		if (false && bCalc)
		{
			int resultWidth = 0;
			if (nd->getType() == pmb::parser::ndUnknown)
			{
				const pmb::parser::nodes::unknown<item, ndtype>* uk = static_cast<const pmb::parser::nodes::unknown<item, ndtype>*>(nd);
				if (uk->isValid() && !uk->isCallFunction())
				{
					const operation* opr = reinterpret_cast<const operation*>(uk->pointer());
					if (opr->getSymbol() && !strcmp(opr->getSymbol(), "=.") && (!pDoc->m_result.empty() && pDoc->m_error.empty()))
					{
						of = pDC->SelectObject(m_resource.font("@operators", 0));
						sn = pDoc->m_result.c_str();
						pDC->DrawText(sn, ctr, DT_CALCRECT | DT_LEFT | DT_TOP | DT_SINGLELINE);
						of = pDC->SelectObject(pFont);
						resultWidth = ctr.Width();
					}
				}
			}
			if (x_pos && *x_pos < tr.right || !x_pos && m_caret.spos[1] <= nd->getEnd())
			{
				int x = tr.left;
				for (int i = nd->getIni(); i < nd->getEnd(); ++i)
				{
					ABC width;
					pDC->GetCharABCWidths(pDoc->m_expr[i], pDoc->m_expr[i], &width);
					//pDC->GetOutputCharWidth(m_expr[i], m_expr[i], &width);
					x += width.abcA + width.abcB + width.abcC;
					if (i + 1 == nd->getEnd())
						x += resultWidth;
					if (x_pos && *x_pos < x)
					{
						m_caret.spos[1] = i;
						m_caret.pos[1].x = x;
						break;
					}
					else if (!x_pos && m_caret.spos[1] == i + 1)
					{
						m_caret.pos[1].x = x;
						break;
					}
				}
				break;
			}
			else if (resultWidth)
			{
				tr.left = tr.right;
				tr.right += resultWidth;
			}
		}
		else
		{
			CRect backTr;
			bool bDrawText = true;
			bool bBackColor = false;
			if (nd->getType() == pmb::parser::ndUnknown)
			{
				const pmb::parser::nodes::unknown<item, ndtype>* uk = static_cast<const pmb::parser::nodes::unknown<item, ndtype>*>(nd);
				if (uk->isValid() && !uk->isCallFunction())
				{
					pDC->SelectObject(&pen);
					const ::operation* opr = reinterpret_cast<const ::operation*>(uk->pointer());
					if (!strcmp(opr->getSymbol(), "=") || !strcmp(opr->getSymbol(), "=."))
					{
						bDrawText = false;
						pDC->MoveTo(tr.left, tr.top + tr.Height() / 2 - 2);
						pDC->LineTo(tr.right, tr.top + tr.Height() / 2 - 2);
						pDC->MoveTo(tr.left, tr.top + tr.Height() / 2 + 2);
						pDC->LineTo(tr.right, tr.top + tr.Height() / 2 + 2);
						bResult = !opr->isBinary() && opr->isLeftToRight() || !strcmp(opr->getSymbol(), "=.");
						if (bResult && (!pDoc->m_result.empty() && pDoc->m_error.empty()))
						{
							of = pDC->SelectObject(m_resource.font("@operators", 0));
							pDC->SetTextColor(m_resource.color("@operators"));
							bBackColor = bDrawText = true;
							sn = pDoc->m_result.c_str();
							backTr = tr;
							tr.left = tr.right;
							pDC->DrawText(sn, ctr, DT_CALCRECT | DT_LEFT | DT_TOP | DT_SINGLELINE);
							tr.right += ctr.Width();
						}
					}
					else if (!strcmp(opr->getSymbol(), "*"))
					{
						bDrawText = false;
						pDC->MoveTo(tr.left, tr.top + tr.Height() / 2 - tr.Width() / 2);
						pDC->LineTo(tr.right, tr.top + tr.Height() / 2 + tr.Width() / 2);
						pDC->MoveTo(tr.left, tr.top + tr.Height() / 2 + tr.Width() / 2);
						pDC->LineTo(tr.right, tr.top + tr.Height() / 2 - tr.Width() / 2);
					}
					else if (!strcmp(opr->getSymbol(), "/"))
					{
						bDrawText = false;
						pDC->MoveTo(tr.right, tr.top);
						pDC->LineTo(tr.left, tr.bottom);
					}
					else if (!strcmp(opr->getSymbol(), "+"))
					{
						bDrawText = false;
						pDC->MoveTo(tr.left + tr.Width() / 2, tr.top + tr.Height() / 2 - tr.Width() / 2);
						pDC->LineTo(tr.left + tr.Width() / 2, tr.top + tr.Height() / 2 + tr.Width() / 2);
						pDC->MoveTo(tr.left, tr.top + tr.Height() / 2);
						pDC->LineTo(tr.right, tr.top + tr.Height() / 2);
					}
					else if (!strcmp(opr->getSymbol(), "-"))
					{
						bDrawText = false;
						pDC->MoveTo(tr.left, tr.top + tr.Height() / 2);
						pDC->LineTo(tr.right, tr.top + tr.Height() / 2);
					}
				}
			}
			if (bDrawText)
			{
				COLORREF bckColor;
				if (bResult && bBackColor)
				{
					bckColor = pDC->GetBkColor();
					pDC->FillSolidRect(tr, RGB(0xD0, 0xD0, 0xD0));
				}
				pDC->DrawText(sn, tr, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				if (bResult && bBackColor)
				{
					pDC->SetBkColor(bckColor);
					CRect aux(tr);
					tr = backTr;
					backTr = aux;
				}
			}

			if (nd == pDoc->m_error.item())
			{
				CPen pen(PS_SOLID, 1, RGB(0xC0, 0x00, 0x00));
				CPen* oldPen = pDC->SelectObject(&pen);
				int x0 = tr.left;
				int x1 = tr.right;
				if (x1 - x0 < 6)
				{
					x0 -= 3;
					x1 += 3;
				}
				m_error = CRect(x0, tr.top, x1, tr.bottom);
				pDC->MoveTo(x0, tr.bottom);
				for (int x = x0 + 1; x < x1; ++x)
					pDC->LineTo(x, tr.bottom - (x - x0) % 2);
				pDC->SelectObject(oldPen);
			}
			if (nd == m_pNd || nd == m_pNdUnk)
			{
				CRect r(tr);
				if (nd == m_pNd)
					r.InflateRect(1, 2);
				else
					r.InflateRect(2, 3);
				CPen pen(PS_SOLID, 1, nd == m_pNd ? RGB(0x80, 0x80, 0x80) : RGB(0x80, 0x00, 0x00));
				CPen* oldPen = pDC->SelectObject(&pen);
				pDC->SelectStockObject(NULL_BRUSH);
				pDC->Rectangle(&r);
				pDC->SelectObject(oldPen);
			}
			if (bResult && bBackColor)
				tr.right = backTr.right;
		}
		tr.left = tr.right;
		if (max_x < tr.right)
			max_x = tr.right;
		end = nd->getEnd();
	}

	if (!bCalc && (m_bShowResult && !bResult) && (!pDoc->m_result.empty() || !pDoc->m_error.empty()))
	{
		tr.left = max_x + 10;
		tr.right = wr.Width() - 1;
		tr.InflateRect(1, 1);
		pDC->FillSolidRect(tr, pDoc->m_error.empty() ? RGB(0xD0, 0xD0, 0xD0) : RGB(0xD0, 0x00, 0x00));
		tr.DeflateRect(1, 1);
		CString sr;
		if (pDoc->m_error.empty())
		{
			if (!bResult)
				sr = " = ";
			sr += pDoc->m_result.c_str();
		}
		else
			sr = pDoc->m_error.message(pDoc->m_expr.c_str()).c_str();
		pDC->DrawText(sr, tr, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
	}

	if (oldFont)
	{
		pDC->SelectObject(oldFont);
		if (!bCalc)
			pDC->SetTextColor(oldColor);
	}
	if (oldPen)
		pDC->SelectObject(oldPen);

	if (!bCalc)
		ShowCaret();
}


// CParserView printing


void CParserView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CParserView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CParserView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CParserView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CParserView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CParserView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CParserView diagnostics

#ifdef _DEBUG
void CParserView::AssertValid() const
{
	CView::AssertValid();
}

void CParserView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CParserDoc* CParserView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CParserDoc)));
	return (CParserDoc*)m_pDocument;
}
#endif //_DEBUG


// CParserView message handlers


void CParserView::OnIterationMoveNext()
{
	GetDocument()->nextStep();
	RedrawWindow();
	AfxGetMainWnd()->SendMessageToDescendants(MM_REDRAWTREE);
}


afx_msg LRESULT CParserView::OnSetnode(WPARAM wParam, LPARAM lParam)
{
	if(lParam)
		m_pNdUnk = reinterpret_cast<tnode*>(lParam);
	else
		m_pNd = reinterpret_cast<tnode*>(wParam);
	RedrawWindow();
	return 0;
}


void CParserView::OnNextunknow()
{
	AfxGetMainWnd()->SendMessageToDescendants(MM_NEXTUNKNOW);
}


void CParserView::OnMouseMove(UINT nFlags, CPoint point)
{
	point.x = point.x * _scaleDen.cx / _scaleNum.cx;
	point.y = point.y * _scaleDen.cy / _scaleNum.cy;
	if (GetDocument()->m_error.item() && m_error.PtInRect(point))
	{
		if (m_tooltipId != 1)
		{
			CString text(GetDocument()->m_error.message(GetDocument()->m_expr.c_str()).c_str());
			//m_tooltip.SetTitle(0, text);
			m_tooltip.UpdateTipText(text, this);
			m_tooltip.Popup();
			m_tooltipId = 1;
		}
	}
	else
	{
		m_tooltip.SetTitle(0, nullptr);
		m_tooltip.UpdateTipText(L"", this);
		m_tooltipId = -1;
	}
	CView::OnMouseMove(nFlags, point);
}




BOOL CParserView::PreTranslateMessage(MSG* pMsg)
{
	if (m_tooltip.m_hWnd)
		m_tooltip.RelayEvent(pMsg);

	return CView::PreTranslateMessage(pMsg);
}




inline void CParserView::set_caret()
{
	CreateSolidCaret(_scaleNum.cx < _scaleDen.cx  ? 1 : _scaleNum.cx / _scaleDen.cx, m_caret.height * _scaleNum.cy / _scaleDen.cy);
	CPoint pc(m_caret.pos[1].x * _scaleNum.cx / _scaleDen.cx, m_caret.pos[1].y * _scaleNum.cy / _scaleDen.cx);

	pmb::log* lg = pmb::log::beginFunction(pmb::logDebug, __FUNCTION__);
	lg->trace(pmb::logDebug, "caret pos x = %d; caret pos y = %d; caret height = %d\n", m_caret.pos[1].x, m_caret.pos[1].y, m_caret.height);

	SetCaretPos(pc);
	ShowCaret();

	lg->endFunction();
}



const std::string& CParserView::getExpression() const
{
	return GetDocument()->m_expr;
}



void CParserView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	if (m_resource.pretty() && !m_line.empty())
		m_line[0](m_caret);
	set_caret();
}


void CParserView::OnKillFocus(CWnd* pNewWnd)
{
	CView::OnKillFocus(pNewWnd);

	HideCaret();
	DestroyCaret();
}




void CParserView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	HideCaret();

	CParserDoc* pDoc = GetDocument();
	bool bModified = false;

	item::SIZETP old0 = m_caret.spos[0];
	item::SIZETP old1 = m_caret.spos[1];
	switch (nChar)
	{
	case VK_RIGHT:
		++m_caret.spos[1];
		break;
	case VK_LEFT:
		--m_caret.spos[1];
		break;
	case VK_HOME:
		m_caret.spos[1] = 0;
		break;
	case VK_END:
		m_caret.spos[1] = pDoc->m_expr.size();
		break;
	case VK_DELETE:
		if (m_caret.spos[1] < pDoc->m_expr.size() - 1)
		{
			bModified = true;
			std::string nexpr = pDoc->m_expr.substr(0, m_caret.spos[1]);
			if (m_caret.spos[1] + 1 < pDoc->m_expr.size())
				nexpr += pDoc->m_expr.substr(m_caret.spos[1] + 1);
			pDoc->m_expr = nexpr;
		}
		break;
	default:
		break;
	}
	if (m_caret.spos[1] < 0)
		m_caret.spos[1] = 0;
	else if (pDoc->m_expr.size() < m_caret.spos[1])
		m_caret.spos[1] = pDoc->m_expr.size();

	if (m_resource.pretty())
	{
		if (bModified)
		{
			GetDocument()->update();
			CDC* pDC = GetDC();
			m_line[0].set(pDC, m_p0.x, m_p0.y);
			ReleaseDC(pDC);
			InvalidateRect(nullptr, 1);
		}
		if (m_caret.spos[1] != old1)
		{
			if (m_line[0](m_caret))
				set_caret();
		}
	}
	else if (m_caret.spos[1] != old1)
	{
		CDC* pDC = GetDC();
		draw_line(pDC, true);
		ReleaseDC(pDC);
		set_caret();

		if (!(GetKeyState(VK_LSHIFT) & 0x8000))
		{
			m_caret.spos[0] = m_caret.spos[1];
			m_caret.pos[0] = m_caret.pos[1];
		}
	}

	ShowCaret();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}




void CParserView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	HideCaret();

	CParserDoc* pDoc = GetDocument();

	bool bModified = false;

	item::SIZETP old0 = m_caret.spos[0];
	item::SIZETP old1 = m_caret.spos[1];
	switch (nChar)
	{
	case VK_BACK:
		if (0 < m_caret.spos[1])
		{
			bModified = true;
			std::string nexpr = pDoc->m_expr.substr(0, m_caret.spos[1] - 1);
			if (m_caret.spos[1] < pDoc->m_expr.size())
				nexpr += pDoc->m_expr.substr(m_caret.spos[1]);
			pDoc->m_expr = nexpr;
			--m_caret.spos[1];
		}
		break;
	case VK_RETURN:
		break;
	default:
		if ('0' <= nChar && nChar <= '9' || nChar == '.' || 'A' <= nChar && nChar <= 'Z' || 'a' <= nChar && nChar <= 'z'
			|| nChar == '!' || nChar == '%' || nChar == '^' || nChar == '*' || nChar == '(' || nChar == ')' || nChar == '-' || nChar == VK_ADD || nChar == '='
			|| nChar == '+' || nChar == '[' || nChar == '{' || nChar == ']' || nChar == '}' || nChar == '|'
			|| nChar == '/' || nChar == '\\' || nChar == '<' || nChar == '>' || nChar == VK_SPACE
			|| nChar == ',' || nChar == ':'  || nChar == ';')
		{
			bModified = true;
			std::string nexpr;
			if (0 < m_caret.spos[1])
				nexpr = pDoc->m_expr.substr(0, m_caret.spos[1]);
			nexpr += (char)nChar;
			if (m_caret.spos[1] < pDoc->m_expr.size())
				nexpr += pDoc->m_expr.substr(m_caret.spos[1]);
			pDoc->m_expr = nexpr;
			++m_caret.spos[1];
		}
		break;
	}
	if (m_caret.spos[1] < 0)
		m_caret.spos[1] = 0;
	else if (pDoc->m_expr.size() < m_caret.spos[1])
		m_caret.spos[1] = pDoc->m_expr.size();

	if (m_resource.pretty())
	{
		if (bModified)
		{
			CDC* pDC = GetDC();
			GetDocument()->update();
			m_line[0].set(pDC, m_p0.x, m_p0.y);
			InvalidateRect(nullptr, 1);
			ReleaseDC(pDC);
		}
		if (m_caret.spos[1] != old1)
		{
			if (m_line[0](m_caret))
				set_caret();
		}
	}
	else if (m_caret.spos[1] != old1)
	{
		CDC* pDC = GetDC();
		draw_line(pDC, true);
		ReleaseDC(pDC);
		set_caret();

		if (!(GetKeyState(VK_LSHIFT) & 0x8000))
		{
			m_caret.spos[0] = m_caret.spos[1];
			m_caret.pos[0] = m_caret.pos[1];
		}
	}

	ShowCaret();

	CView::OnChar(nChar, nRepCnt, nFlags);
}



void CParserView::OnLButtonDown(UINT nFlags, CPoint point)
{
	HideCaret();

	if (m_p0.y <= point.y && point.y <= m_p0.y + m_resource.max_height())
	{
		CDC* pDC = GetDC();
		int x = point.x;
		draw_line(pDC, true, &x);
		ReleaseDC(pDC);
		set_caret();

		if (!(GetKeyState(VK_LSHIFT) & 0x8000))
		{
			m_caret.spos[0] = m_caret.spos[1];
			m_caret.pos[0] = m_caret.pos[1];
		}
	}

	ShowCaret();
	CView::OnLButtonDown(nFlags, point);
}


BOOL CParserView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_IBEAM));
	return TRUE;
}








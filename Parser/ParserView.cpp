
// ParserView.cpp : implementation of the CParserView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Parser.h"
#endif

#include "ParserDoc.h"
#include "ParserView.h"
#include "pmb_parser_node.cpp"


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
END_MESSAGE_MAP()

// CParserView construction/destruction

CParserView::CParserView()
	: m_pNd(NULL), m_pNdUnk(NULL)
{
	// TODO: add construction code here

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

	m_style.font[0].CreatePointFont(120, L"Bookman Old Style");
	LOGFONT lf;
	m_style.font[0].GetLogFont(&lf);
	lf.lfItalic = TRUE;
	m_style.font[1].CreateFontIndirect(&lf);
	lf.lfItalic = FALSE;
	lf.lfWeight = FW_BOLD;
	m_style.font[2].CreateFontIndirect(&lf);

	CDC* pDC = GetDC();
	lf.lfHeight = -MulDiv(10, pDC->GetDeviceCaps(LOGPIXELSY), 72);
	lf.lfWeight = FW_DONTCARE;
	m_style.font[3].CreateFontIndirect(&lf);
	lf.lfItalic = TRUE;
	m_style.font[4].CreatePointFontIndirect(&lf);
	lf.lfItalic = FALSE;
	lf.lfWeight = FW_BOLD;
	m_style.font[5].CreatePointFontIndirect(&lf);

	lf.lfHeight = -MulDiv(8, pDC->GetDeviceCaps(LOGPIXELSY), 72);
	lf.lfWeight = FW_DONTCARE;
	m_style.font[6].CreateFontIndirect(&lf);
	lf.lfItalic = TRUE;
	m_style.font[7].CreatePointFontIndirect(&lf);
	lf.lfItalic = FALSE;
	lf.lfWeight = FW_BOLD;
	m_style.font[8].CreatePointFontIndirect(&lf);
	ReleaseDC(pDC);

	m_style.color[0] = RGB(0xFF, 0xFF, 0xFF); // back color
	m_style.color[1] = RGB(0x60, 0x60, 0x60); // numbers
	m_style.color[2] = RGB(0x60, 0x00, 0x00); // operators
	m_style.color[3] = RGB(0x00, 0x00, 0x00); // variables
	m_style.color[4] = RGB(0x00, 0x00, 0x60); // build in functions
	m_style.color[5] = RGB(0x00, 0x60, 0x60); // user define functions
	m_style.color[6] = RGB(0x00, 0x00, 0x00); // parentheses
	m_style.color[7] = RGB(0x00, 0x00, 0x00); // list
	m_style.color[8] = RGB(0x00, 0x80, 0x00); // strings

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
	return 0;
}

void CParserView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	CFont* oldFont;
	for (int i = 0; i < 9; ++i)
	{
		if (!i)
			oldFont = pDC->SelectObject(m_style.font + i);
		else
			pDC->SelectObject(m_style.font + i);
		TEXTMETRIC tm;
		pDC->GetTextMetrics(&tm);
		m_style.height[i] = tm.tmHeight;
	}
	for (int i = 0; i < 9; ++i)
	{
		if (!i || m_style.maxHeight < m_style.height[i])
			m_style.maxHeight = m_style.lineHeight = m_style.height[i];
	}
	pDC->SelectObject(oldFont);
	ReleaseDC(pDC);

	m_expr = GetDocument()->m_expr;
	m_style.caretPos = m_p0;
	--m_style.caretPos.x;
	m_style.caretPos0 = m_style.caretPos;
	m_style.caret[0] = m_style.caret[1] = m_expr.length();
}


// CParserView drawing

void CParserView::OnDraw(CDC* pDC)
{
	draw(pDC, false);
}


void CParserView::draw(CDC* pDC, bool bCalc, int* x_pos)
{
	CParserDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	const pmb::parser::node<item, ndtype>* nd = pDoc->getNodeRoot();
	if (!nd)
		return;

	if (bCalc && (x_pos && *x_pos < m_p0.x || !x_pos && !m_style.caret[1]))
	{
		if (x_pos)
			m_style.caret[1] = 0;
		m_style.caretPos.x = m_p0.x;
		return;
	}

	if (!bCalc)
		HideCaret();

	CString expr(m_expr.c_str());
	CRect wr;
	GetClientRect(&wr);

	CPen pen;
	if (!bCalc)
		pen.CreatePen(PS_SOLID, 2, m_style.color[2]);

	COLORREF oldColor;
	CFont* oldFont = nullptr;
	CPen* oldPen = bCalc ? nullptr : pDC->SelectObject(&pen);
	CRect tr(m_p0.x, m_p0.y, m_p0.x, m_p0.y + m_style.maxHeight);
	int end = 0;
	for (nd = nd->getFirstNode(); nd; nd = nd->getNextNode())
	{
		CFont* of;
		if (end < nd->getIni())
		{
			of = pDC->SelectObject(m_style.font);
			int width;
			pDC->GetOutputCharWidth(' ', ' ', &width);
			tr.right += width * (nd->getIni() - end);

			if (!bCalc)
				pDC->FillSolidRect(tr, m_style.color[0]);
			else
			{
				if (x_pos && *x_pos < tr.right)
				{
					m_style.caret[1] = nd->getIni() + (*x_pos - tr.left) / width;
					m_style.caretPos.x = tr.left + width * (m_style.caret[1] - nd->getIni());
					break;
				}
				else if (!x_pos && m_style.caret[1] <= nd->getIni())
				{
					m_style.caretPos.x = tr.left + width * (m_style.caret[1] - nd->getIni() + 1);
					break;
				}
			}
			tr.left = tr.right;
			if (!oldFont)
				pDC->SelectObject(of);
		}
		COLORREF oc;
		short font;
		short color;
		switch (nd->getType())
		{
		case pmb::parser::ndSpace:
			font = 0;
			color = 2;
			break;
		case pmb::parser::ndAlpha:
			font = 1;
			if (nd->getParent() && nd->getParent()->getType() == pmb::parser::ndUnknown)
			{
				const pmb::parser::nodes::unknown<item, ndtype>* uk = static_cast<const pmb::parser::nodes::unknown<item, ndtype>*>(nd->getParent());
				if (uk->isBinary() && uk->isCallFunction() && (uk->isFirstLeft() && uk->getLeft() == nd || uk->isFirstRight() && uk->getRight() == nd))
					color = uk->isCallBuildInFunction() ? 4 : 5;
				else
					color = 3;
			}
			else
				color = 3;
			break;
		case pmb::parser::ndNumber:
			font = 0;
			color = 1;
			break;
		case pmb::parser::ndString:
			font = 0;
			color = 8;
			break;
		case pmb::parser::ndList:
			font = 2;
			color = 7;
			break;
		case pmb::parser::ndParentheses:
			font = 0;
			color = 6;
			break;
		case pmb::parser::ndUnknown:
			font = 2;
			color = 2;
			break;
		}
		of = pDC->SelectObject(m_style.font + font);
		if (!bCalc)
			oc = pDC->SetTextColor(m_style.color[color]);
		if (!oldFont)
		{
			oldFont = of;
			if (!bCalc)
				oldColor = oc;
		}
		CString sn(m_expr.c_str() + nd->getIni(), nd->len());
		CRect ctr = tr;
		ctr.right += 100;
		pDC->DrawText(sn, ctr, DT_CALCRECT | DT_LEFT | DT_TOP | DT_SINGLELINE);
		tr.right = tr.left + ctr.Width();

		LOGFONT lf;
		m_style.font[font].GetLogFont(&lf);
		if (lf.lfItalic)
			tr.right += 2;

		if (bCalc)
		{
			if (x_pos && *x_pos < tr.right || !x_pos && m_style.caret[1] <= nd->getEnd())
			{
				int x = tr.left;
				for (int i = nd->getIni(); i < nd->getEnd(); ++i)
				{
					ABC width;
					pDC->GetCharABCWidths(m_expr[i], m_expr[i], &width);
					//pDC->GetOutputCharWidth(m_expr[i], m_expr[i], &width);
					x += width.abcA + width.abcB + width.abcC;
					if (x_pos && *x_pos < x)
					{
						m_style.caret[1] = i;
						m_style.caretPos.x = x;
						break;
					}
					else if (!x_pos && m_style.caret[1] == i + 1)
					{
						m_style.caretPos.x = x;
						break;
					}
				}
				break;
			}
		}
		else
		{
			bool bDrawText = true;
			if (nd->getType() == pmb::parser::ndUnknown)
			{
				const pmb::parser::nodes::unknown<item, ndtype>* uk = static_cast<const pmb::parser::nodes::unknown<item, ndtype>*>(nd);
				if (uk->isValid() && !uk->isCallFunction())
				{
					const operation* opr = reinterpret_cast<const operation*>(uk->pointer());
					if (!strcmp(opr->getSymbol(), "="))
					{
						bDrawText = false;
						pDC->MoveTo(tr.left, tr.top + tr.Height() / 2 - 2);
						pDC->LineTo(tr.right, tr.top + tr.Height() / 2 - 2);
						pDC->MoveTo(tr.left, tr.top + tr.Height() / 2 + 2);
						pDC->LineTo(tr.right, tr.top + tr.Height() / 2 + 2);
					}
					else if (!strcmp(opr->getSymbol(), "*"))
					{
						bDrawText = false;
						pDC->MoveTo(tr.left, tr.top + tr.Height() / 2 - tr.Width() / 2);
						pDC->LineTo(tr.right, tr.top + tr.Height() / 2 + tr.Width() / 2);
						pDC->MoveTo(tr.left, tr.top + tr.Height() / 2 + tr.Width() / 2);
						pDC->LineTo(tr.right, tr.top + tr.Height() / 2 - tr.Width() / 2);
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
				pDC->DrawText(sn, tr, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

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
		}
		tr.left = tr.right;
		end = nd->getEnd();
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
	if (GetDocument()->m_error.item() && m_error.PtInRect(point))
	{
		if (m_tooltipId != 1)
		{
			CString text(GetDocument()->m_error.message(GetDocument()->m_expr).c_str());
			//m_tooltip.SetTitle(0, text);
			m_tooltip.UpdateTipText(text, this);
			m_tooltip.Popup();
			m_tooltipId = 1;
		}
	}
	else
	{
		m_tooltip.SetTitle(0, NULL);
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


void CParserView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	CreateSolidCaret(1, m_style.maxHeight);
	SetCaretPos(m_style.caretPos);
	ShowCaret();
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

	int old0 = m_style.caret[0];
	int old1 = m_style.caret[1];
	switch (nChar)
	{
	case VK_RIGHT:
		++m_style.caret[1];
		break;
	case VK_LEFT:
		--m_style.caret[1];
		break;
	case VK_HOME:
		m_style.caret[1] = 0;
		break;
	case VK_END:
		m_style.caret[1] = m_expr.size();
		break;
	}
	if (m_style.caret[1] < 0)
		m_style.caret[1] = 0;
	else if (m_expr.size() < m_style.caret[1])
		m_style.caret[1] = m_expr.size();

	if (m_style.caret[1] != old1)
	{
		CDC* pDC = GetDC();
		draw(pDC, true);
		ReleaseDC(pDC);
		SetCaretPos(m_style.caretPos);

		if (!(GetKeyState(VK_LSHIFT) & 0x8000))
		{
			m_style.caret[0] = m_style.caret[1];
			m_style.caretPos0 = m_style.caretPos;
		}
	}

	ShowCaret();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}




void CParserView::OnLButtonDown(UINT nFlags, CPoint point)
{
	HideCaret();

	if (m_p0.y <= point.y && point.y <= m_p0.y + m_style.maxHeight)
	{
		CDC* pDC = GetDC();
		int x = point.x;
		draw(pDC, true, &x);
		ReleaseDC(pDC);
		SetCaretPos(m_style.caretPos);

		if (!(GetKeyState(VK_LSHIFT) & 0x8000))
		{
			m_style.caret[0] = m_style.caret[1];
			m_style.caretPos0 = m_style.caretPos;
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

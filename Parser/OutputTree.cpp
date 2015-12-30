// OutputTree.cpp : implementation file
//

#include "stdafx.h"
#include "Parser.h"
#include "OutputTree.h"

#include "pmb_parser_transporter.cpp"
#include "pmb_parser_nodes_calc.h"
#include "pmb_parser_nodes_calc.cpp"
#include "pmb_parser_nodes_unknow.h"
#include "pmb_parser_nodes_unknow.cpp"
#include "pmb_parser_nodes_list.h"
#include "pmb_parser_nodes_list.cpp"
#include "pmb_parser_nodes_parentheses.h"
#include "pmb_parser_nodes_parentheses.cpp"

// COutputTree

IMPLEMENT_DYNAMIC(COutputTree, CWnd)

COutputTree::COutputTree(bool bDebug)
: m_pNd(NULL), m_pNdUnknow(NULL), m_bDebug(bDebug)
{

}

COutputTree::~COutputTree()
{
}


BEGIN_MESSAGE_MAP(COutputTree, CWnd)
	ON_WM_DRAWITEM()
	ON_WM_PAINT()
	ON_MESSAGE(MM_REDRAWTREE, &COutputTree::OnRedrawtree)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_VSCROLL()
	ON_MESSAGE(MM_NEXTUNKNOW, &COutputTree::OnNextunknow)
END_MESSAGE_MAP()



// COutputTree message handlers




//void COutputTree::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
//{
//	CDocument* pDoc = GetParentFrame()->GetActiveDocument();
//
//	CWnd::OnDrawItem(nIDCtl, lpDrawItemStruct);
//}


int COutputTree::drawIterators(CDC* pDC, CParserDoc* pDoc, int x0, int y0, int height, const CString& expr)
{
	int y = y0;
	CRect wr;
	GetWindowRect(&wr);
	if(pDoc->m_iterator)
	{
		CRect r(x0, y0, wr.Width() / 3, y0 + 1 * height);
		CString pos;
		pos.Format(L"i = %d[%d]<%c>", pDoc->m_iterator->_i, expr.GetLength(), expr[pDoc->m_iterator->_i]);
		pDC->DrawText(pos, &r, DT_SINGLELINE | DT_LEFT);

		r.top += height + 5;
		r.bottom = r.top + height;
		for(int i = 0; i < pDoc->m_countIterators; ++i)
		{
//			if(pDoc->m_iterator == pDoc->m_iterators[i])
//				pDC->Rectangle(&r);
//			else
				pDC->DrawFocusRect(&r);
			CRect rt = r;
			rt.DeflateRect(1, 1);
			CString clName;
			clName.Format(L"%d.%d- %s", pDoc->m_iterator->_step0, pDoc->m_iterator->_subStep0, (LPCTSTR)CString(pDoc->m_iterators->getClassName()));
			pDC->DrawText(clName, &rt, DT_SINGLELINE | DT_LEFT);
			r.top += height;
			r.bottom += height;
		}
		y = r.bottom + height;
	}

	const pmb::parser::node<transporter>* nd = pDoc->getNewNode();
	if(nd)
	{
		CRect r(wr.Width() / 2, y0, wr.Width() / 2 + 100, y0 + 1 * height);
		CString pos;
		pos.Format(L"New Node");
		pDC->DrawText(pos, &r, DT_SINGLELINE | DT_LEFT);

		graph_node root = {this, pDC, expr, nd, 10, wr.Width() / 2, y0 + 2 * height, height};
		root.draw();
	}
	const pmb::parser::node<transporter>* ndU = pDoc->getNewNodeUnknow();
	if(ndU)
	{
		CRect r(3 * wr.Width() / 4, y0, 3 * wr.Width() / 4 + 100, y0 + 1 * height);
		CString pos;
		pos.Format(L"New Node Unknow");
		pDC->DrawText(pos, &r, DT_SINGLELINE | DT_LEFT);

		int x = x0;
		graph_node root = {this, pDC, expr, ndU, 10, 3 * wr.Width() / 4, y0 + 2 * height, height};
		root.draw();
	}

	return y;
}


void COutputTree::OnPaint()
{
	CRect wr;
	GetWindowRect(&wr);

	CPaintDC dc(this);

	dc.Rectangle(0, 0, wr.Width(), wr.Height());

	CParserDoc* pDoc = CParserDoc::getDocument(this);
	if(!pDoc)
		return;

	CString expr(pDoc->m_expr);

	CFont f;
	f.CreatePointFont(100, L"Bookman Old Style", &dc);
	dc.SelectObject(&f);
	int height;
	{
		CRect r(0, 0, wr.right, 40);
		dc.DrawText(L"Hg", &r, DT_CALCRECT | DT_SINGLELINE | DT_LEFT);
		height = r.Height();
	}

	AfxTrace(_T("scroll pos = %d\n"), GetScrollPos(SB_VERT));
	const int x0 = 10 - GetScrollPos(SB_HORZ),
				y0 = 20 - GetScrollPos(SB_VERT);
	if(pDoc->getTree() && m_bDebug)
	{
		m_cnd.clear();
		int y = drawIterators(&dc, pDoc, x0, 10, height, expr);

		const pmb::parser::node<transporter>* rt = pDoc->getTree()->getRootNode();
		graph_node root = {this, &dc, expr, rt, 0, x0, y, height};
		m_cy = root.draw();
		m_cx = root.x;
	}


	if(pDoc->getTree2() && !m_bDebug)
	{
		m_cnd.clear();

		const pmb::parser::node<transporter>* rt = pDoc->getTree2()->getRootNode();
		graph_node root = {this, &dc, expr, rt, 0, x0, y0, height};
		m_cy = root.draw();
		m_cx = root.x;
	}
	setScrolls(wr.Width(), wr.Height());
}



void COutputTree::setScrolls(int cx, int cy)
{
	SCROLLINFO si = {sizeof(SCROLLINFO), SIF_PAGE | SIF_POS | SIF_RANGE | SIF_TRACKPOS, 0, m_cx, cx, 0, 10};
	ShowScrollBar(SB_HORZ, cx < m_cx);
	if(cx < m_cx)
		SetScrollInfo(SB_HORZ, &si);
	si.nMax = m_cy;
	si.nPage = cy;
	ShowScrollBar(SB_VERT, cy < m_cy);
	if(cy < m_cy)
		SetScrollInfo(SB_VERT, &si);
}


void COutputTree::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);

	int delta = 0;
	switch (nSBCode)
	{
	case SB_LINEDOWN:
		delta = 1;
		break;
	case SB_LINEUP:
		delta = -1;
		break;
	case SB_PAGEDOWN:
		delta = 10;
		break;
	case SB_THUMBPOSITION:
		delta = (int)nPos - GetScrollPos(SB_VERT);
		break;
	case SB_PAGEUP:
		delta = -10;
		break;
	default:
		return;
	}
	int pos = GetScrollPos(SB_VERT) + delta;
	SetScrollPos(SB_VERT, pos);
	ScrollWindow(0, -delta);
	RedrawWindow();
}




const COutputTree::graph_node::scolor COutputTree::graph_node::color[] = 
	{{'A', RGB(0x00, 0x00, 0x00)},
	 {'N', RGB(0x80, 0x80, 0x80)},
	 {'S', RGB(0x00, 0x80, 0x00)},
	 {'U', RGB(0x80, 0x00, 0x00)},
	 {'P', RGB(0x00, 0x00, 0x80)},
	 {'s', RGB(0x00, 0x80, 0x80)},
	 {'L', RGB(0x80, 0x80, 0x80)} };


COLORREF COutputTree::graph_node::getColor() const
{
	int i;
	for(i = 0; i < sizeof(color) / sizeof(*color) && color[i].type != nd->getType(); ++i)
		;
	return i < sizeof(color) / sizeof(*color) ? color[i].color: RGB(0xFF, 0x00, 0x00);
}


int COutputTree::graph_node::draw()
{
	if(!nd)
		return y;
	graph_node left = {parent, pDC, expr, nd->getLeft(), 0, x, y + 3 * h / 2, h};
	int yleft = left.draw();
	if(nd->getLeft())
		x = left.x + h / 2;

	x0 = x;
	CString str = expr.Mid(nd->getIni(), nd->len());
	
	CRect r(x, y - h / 2, x + 10, y + h / 2);
	pDC->DrawText(str, &r, DT_CALCRECT | DT_SINGLELINE | DT_LEFT);
	int xend = r.Width();
	r.left = x - xend / 2;
	r.right = x + xend / 2;
	CRect re(r);
	int inflate = nd->getType() != 'A' && nd->getType() != 'N' ? 4: 2;
	re.InflateRect(inflate, inflate);

	CPen pen(PS_SOLID, 1, getColor());
	CPen* oldPen = pDC->SelectObject(&pen);
	CBrush br;
	br.CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
	CBrush brUnknow;
	brUnknow.CreateSolidBrush(RGB(0x80, 0x00, 0x00));
	CBrush* oldBrush = NULL;

	if(parent->m_pNd == nd)
		oldBrush = pDC->SelectObject(&br);
	else if(parent->m_pNdUnknow == nd || nd->isCalcType() && static_cast<const pmb::parser::nodes::calc<transporter>*>(nd)->isCalculated())
		oldBrush = pDC->SelectObject(&brUnknow);
	pDC->Ellipse(&re);
	parent->m_cnd.push_back(node(re, nd));
	if(nd->getType() != pmb::parser::ndAlpha && nd->getType() != pmb::parser::ndNumber && nd->getType() != pmb::parser::ndString)
	{
		if(oldBrush && parent->m_pNdUnknow != nd && parent->m_pNd != nd)
			pDC->SelectObject(oldBrush);
		re.DeflateRect(2, 2);
		pDC->Ellipse(&re);
	}
	if(nd->isCalcType())
	{
		const transporter& val =	nd->getType() == pmb::parser::ndUnknow ? static_cast<const pmb::parser::nodes::unknow<transporter>*>(nd)->getValue():
							nd->getType() == pmb::parser::ndParentheses ? static_cast<const pmb::parser::nodes::parentheses<transporter>*>(nd)->getValue():
							*static_cast<const pmb::parser::nodes::list<transporter>*>(nd)->getRValue();
		CBrush brVal;
		brVal.CreateSolidBrush(val._getStateColor());
		CBrush* oBr = pDC->SelectObject(&brVal);
		pDC->Ellipse(re.right + 2, re.top + re.Height() / 2, re.right + 7, re.top + re.Height() / 2 + 5);
		if(nd->getType() == pmb::parser::ndList)
		{
			const transporter& lVal =	static_cast<const pmb::parser::nodes::list<transporter>*>(nd)->getLValue();
			CBrush brValL;
			brValL.CreateSolidBrush(lVal._getStateColor());
			pDC->SelectObject(&brValL);
			pDC->Ellipse(re.left - 2, re.top + re.Height() / 2, re.left - 7, re.top + re.Height() / 2 + 5);
		}
		pDC->SelectObject(oBr);
	}

	if(parent->m_pNd == nd || parent->m_pNdUnknow == nd)
		pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
	else
		pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(str, &r, DT_SINGLELINE | DT_LEFT);

	pDC->SelectObject(oldPen);
	if(parent->m_pNd == nd || parent->m_pNdUnknow == nd)
		pDC->SelectObject(oldBrush);
	if(nd->getLeft())
	{
		pDC->MoveTo(x0 - 2, y + h / 2);
		pDC->LineTo(left.x0, left.y - h / 2);
	}

	x += xend / 2;

	graph_node right = {parent, pDC, expr, nd->getRight(), 0, x + h / 2, y + 3 * h / 2, h};
	int yright = right.draw();

	if(nd->getRight())
	{
		pDC->MoveTo(x0 + 2, y + h / 2);
		pDC->LineTo(right.x0, right.y - h / 2);
		if(x < right.x)
			x = right.x;
	}
	return y < yleft ? yleft < yright ? yright: yleft: y < yright ? yright < yleft ? yleft: yright: y;
}


afx_msg LRESULT COutputTree::OnRedrawtree(WPARAM wParam, LPARAM lParam)
{
	RedrawWindow();
	return 0;
}


void COutputTree::OnMouseMove(UINT nFlags, CPoint point)
{
	lrect::const_iterator i;
	for(i = m_cnd.begin(); i != m_cnd.end() && !i->PtInRect(point); ++i)
		;
	SetCursor(i != m_cnd.end() ? AfxGetApp()->LoadStandardCursor(IDC_HAND): AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	CWnd::OnMouseMove(nFlags, point);
}


void COutputTree::OnLButtonDown(UINT nFlags, CPoint point)
{
	lrect::const_iterator i;
	for(i = m_cnd.begin(); i != m_cnd.end() && !i->PtInRect(point); ++i)
		;
	m_pNd = i != m_cnd.end() ? i->pNd: NULL;
	GetParentFrame()->SendMessageToDescendants(MM_SETNODE, reinterpret_cast<WPARAM>(m_pNd));
	RedrawWindow();

	CWnd::OnLButtonDown(nFlags, point);
}




BOOL COutputTree::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}





afx_msg LRESULT COutputTree::OnNextunknow(WPARAM wParam, LPARAM lParam)
{
	CParserDoc* pDoc = CParserDoc::getDocument(this);
	if(pDoc && !m_bDebug)
	{
		const pmb::parser::node<transporter>* old = m_pNdUnknow;
		m_pNdUnknow = pDoc->getNextUnknowNode(old);
		if(old != m_pNdUnknow)
		{
			GetParentFrame()->SendMessageToDescendants(MM_SETNODE, 0, reinterpret_cast<LPARAM>(m_pNdUnknow));
			RedrawWindow();
		}
	}
	return 0;
}


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
	ON_WM_CHAR()
END_MESSAGE_MAP()

// CParserView construction/destruction

CParserView::CParserView()
	: m_pNd(NULL), m_pNdUnk(NULL), m_line(this)
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
	m_style.color[1] = RGB(0x40, 0x40, 0x40); // numbers
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

	m_bShowResult = true;
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

	m_fstyle = PMB_STYLE_CPPEDIT_MULTILINE;
	m_bEditing = false;

	m_expr = GetDocument()->m_expr;
	m_style.caretPos = m_p0;
	--m_style.caretPos.x;
	m_style.caretPos0 = m_style.caretPos;
	m_style.caret[0] = m_style.caret[1] = m_expr.length();
	m_line.set(pDC, 0, 40);
	ReleaseDC(pDC);
}


// CParserView drawing

void CParserView::OnDraw(CDC* pDC)
{
	m_line.draw(pDC);
	draw_line(pDC, false);
}







void CParserView::line::sdraw::begin_expr(const bnode* pnd)
{
	pline->_swith_expr_begin(pnd, this);
}

void CParserView::line::sdraw::end_expr(const bnode* pnd)
{
	pline->_swith_expr_end(pnd, this);
}






CParserView::line::bnode::bnode()
	: _left(nullptr), _right(nullptr)
{
}


CParserView::line::bnode::~bnode()
{
	if (_left)
		delete _left;
	if (_right)
		delete _right;
}


void CParserView::line::bnode::clear()
{
	if (_left)
	{
		delete _left;
		_left = nullptr;
	}
	if (_right)
	{
		delete _right;
		_right = nullptr;
	}
}


CParserView::line::bnode& CParserView::line::bnode::operator=(const CRect& right)
{
	*static_cast<CRect*>(this) = right;
	return *this;
}


CParserView::line::bnode* CParserView::line::bnode::node_mright()
{
	bnode* rnd;
	for (rnd = this; rnd && rnd->_right; rnd = rnd->_right)
		;
	return rnd;
}


void CParserView::line::bnode::rec_move(int dx, int dy)
{
	_middle += dy;
	top += dy;
	left += dx;
	bottom += dy;
	right += dx;
	if (_left)
		_left->rec_move(dx, dy);
	if (_right)
		_right->rec_move(dx, dy);
}


CRect CParserView::line::bnode::rec_rect() const
{
	if (!_left && !_right)
		return *this;
	CRect cr;
	if (_left)
	{
		cr = _left->rec_rect();
		max_rect(cr, *this);
	}
	else
		cr = *this;
	if (_right)
	{
		CRect rr = _right->rec_rect();
		max_rect(cr, rr);
	}
	return cr;
}


void CParserView::line::bnode::max_rect(CRect& r, const CRect& r2) const
{
	if (r2.top < r.top)
		r.top = r2.top;
	if (r.bottom < r2.bottom)
		r.bottom = r2.bottom;
	if (r2.left < r.left)
		r.left = r2.left;
	if (r.right < r2.right)
		r.right = r2.right;
}


void CParserView::line::bnode::check_error(sset* ss)
{
	ss->pline->check_error(ss->nd, this);
}


void CParserView::line::bnode::end(sset* ss)
{
	CParserDoc* pDoc = ss->pline->_parent->GetDocument();
	if (!ss->pline->_result && pDoc && ss->bEditing && !pDoc->m_result.empty())
	{
		ss->nd = nullptr;
		bnode* nd;
		for (nd = this; nd->_right; nd = nd->_right)
			;
		ss->pnd = nd;

		nd->_right = new node_result(nullptr);
		nd->_right->set(ss);
		nd->_right->rec_move(0, ss->pline->_root->_middle - nd->_right->_middle);
	}
}












CParserView::line::node::node()
	: bnode()
{
}


CParserView::line::node::node(const tnode* nd)
	: bnode(), _ini(nd->getIni()), _end(nd->getEnd())
{
}


CParserView::line::node::~node()
{
}


CParserView::line::node* CParserView::line::node::new_instance(const tnode* nd)
{
	node* nnd = nullptr;
	switch (nd->getType())
	{
	case pmb::parser::ndSpace:
		nnd = new node_space(nd);
		break;
	case pmb::parser::ndAlpha:
		if (nd->getParent() && nd->getParent()->getType() == pmb::parser::ndUnknown)
		{
			const pmb::parser::nodes::unknown<item, ndtype>* uk = static_cast<const pmb::parser::nodes::unknown<item, ndtype>*>(nd->getParent());
			if (uk->isBinary() && uk->isCallFunction() && (uk->isFirstLeft() && uk->getLeft() == nd || uk->isFirstRight() && uk->getRight() == nd))
				nnd = uk->isCallBuildInFunction() ? static_cast<node*>(new node_buildin_function(nd)) : new node_function(nd);
			else
				nnd = new node_alpha(nd);
		}
		else
			nnd = new node_alpha(nd);
		break;
	case pmb::parser::ndNumber:
		nnd = new node_number(nd);
		break;
	case pmb::parser::ndString:
		nnd = new node_string(nd);
		break;
	case pmb::parser::ndList:
		nnd = new node_list(nd);
		break;
	case pmb::parser::ndParentheses:
		nnd = new node_parenthesis(nd);
		break;
	case pmb::parser::ndUnknown:
	{
		const pmb::parser::nodes::unknown<item, ndtype>* uk = static_cast<const pmb::parser::nodes::unknown<item, ndtype>*>(nd);
		if (uk->isValid() && !uk->isCallFunction())
		{
			const operation* opr = reinterpret_cast<const operation*>(uk->pointer());
			if (!strcmp(opr->getSymbol(), "="))
				nnd = new node_operator_equal(nd);
			else if (!strcmp(opr->getSymbol(), "=:"))
				nnd = new node_operator_result(nd);
			else if (!strcmp(opr->getSymbol(), "*"))
				nnd = new node_operator_product(nd);
			else if (!strcmp(opr->getSymbol(), "+"))
				nnd = new node_operator_plus(nd);
			else if (!strcmp(opr->getSymbol(), "-"))
				nnd = new node_operator_minus(nd);
			else if (!strcmp(opr->getSymbol(), "/."))
				nnd = new node_operator_division_inline(nd);
			else if (!strcmp(opr->getSymbol(), "/"))
				nnd = new node_operator_division(nd);
			else if (!strcmp(opr->getSymbol(), "^"))
				nnd = new node_operator_power(nd);
			else if (!strcmp(opr->getSymbol(), "\\"))
				nnd = new node_operator_root(nd);
			else
				nnd = new node_unknown(nd);
		}
		else
			nnd = new node_unknown(nd);
	}
		break;
	default:
		break;
	}
	return nnd;
}




void CParserView::line::node::set(sset* ss)
{
	const tnode* lnd = ss->nd->getLeft();
	const tnode* rnd = ss->nd->getRight();

	ss->parents.push_back(this);
	if (ss->pnd)
	{
		top = ss->pnd->top;
		bottom = ss->pnd->bottom;
		left = right = ss->pnd->right;
		_middle = ss->pnd->_middle;
	}
	else
	{
		top = ss->pline->top;
		bottom = ss->pline->bottom;

		left = right = ss->pline->left;
		_middle = top + Height() / 2;
	}

	if (lnd)
	{
		const tnode* nd = ss->nd;
		ss->nd = lnd;
		ss->pnd = this;
		_left = new_instance(lnd);
		_left->set(ss);
		ss->nd = nd;
	}

	CRect rl;
	if (lnd)
	{
		rl = _left->rec_rect();
		left = rl.right;
		right = left;
		top = rl.top;
		bottom = rl.bottom;
		_middle = _left->_middle;
	}
	if (_ini < _end)
	{
		CFont* oldFont = ss->pDC->SelectObject(ss->pline->_parent->m_style.font + font());
		CString sn(ss->pstr + _ini, ss->nd->len());

		CRect cr(this);
		ss->pDC->DrawText(sn, cr, DT_CALCRECT | DT_LEFT | DT_TOP | DT_SINGLELINE);
		right = left + cr.Width();
		if (lnd && rl.Height() != cr.Height())
		{
			top = _middle - cr.Height() / 2 - cr.Height() % 2;
			bottom = top + cr.Height();
		}
		else if (!lnd)
		{
			top = _middle - cr.Height() / 2;
			bottom = top + cr.Height();
		}
		LOGFONT lf;
		ss->pline->_parent->m_style.font[font()].GetLogFont(&lf);
		if (lf.lfItalic)
			right += 2;
		ss->pDC->SelectObject(oldFont);
	}
	if (rnd)
	{
		const tnode* nd = ss->nd;
		ss->nd = rnd;
		ss->pnd = this;
		_right = new_instance(rnd);
		_right->set(ss);
		ss->nd = nd;
	}
	check_error(ss);
	ss->parents.pop_back();
}



void CParserView::line::node::draw(sdraw* sd) const
{
	sd->begin_expr(this);
	if (_left)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_left->draw(sd);
		sd->pnd = pnd;
	}
	if (_ini < _end)
	{
		COLORREF oldColor = sd->pDC->SetTextColor(sd->pline->_parent->m_style.color[color()]);
		CFont* oldFont = sd->pDC->SelectObject(sd->pline->_parent->m_style.font + font());
		CString sn(sd->pstr + _ini, _end - _ini);

		sd->pDC->DrawText(sn, const_cast<CRect*>(static_cast<const CRect*>(this)), DT_LEFT | DT_TOP | DT_SINGLELINE);
		sd->pDC->SetTextColor(oldColor);
		sd->pDC->SelectObject(oldFont);
	}
	if (_right)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_right->draw(sd);
		sd->pnd = pnd;
	}
	sd->end_expr(this);
}




bool CParserView::line::node::empty() const
{
	return _ini == _end && !_ini && !_left && !_right;
}


bool CParserView::line::node::parenthesis() const
{
	return false;
}


short CParserView::line::node::nparenthesis() const
{
	return 0;
}











CParserView::line::node_space::node_space(const tnode* nd)
	: node(nd)
{
}

short CParserView::line::node_space::font() const
{
	return 0;
}

short CParserView::line::node_space::color() const
{
	return 2;
}







CParserView::line::node_alpha::node_alpha(const tnode* nd)
	: node(nd)
{
}


void CParserView::line::node_alpha::set(sset* ss)
{
	const tnode* lnd = ss->nd->getLeft();
	const tnode* rnd = ss->nd->getRight();

	ss->parents.push_back(this);
	if (ss->pnd)
	{
		top = ss->pnd->top;
		bottom = ss->pnd->bottom;
		left = right = ss->pnd->right;
		_middle = ss->pnd->_middle;
	}
	else
	{
		top = ss->pline->top;
		bottom = ss->pline->bottom;

		left = right = ss->pline->left;
		_middle = top + Height() / 2;
	}

	if (lnd)
	{
		ASSERT(false); // because doble left in  a.0
		const tnode* nd = ss->nd;
		ss->nd = lnd;
		ss->pnd = this;
		_left = new_instance(lnd);
		_left->set(ss);
		ss->nd = nd;
	}

	CRect rl;
	if (lnd)
	{
		ASSERT(false); // because doble left in  a.0
		rl = _left->rec_rect();
		left = rl.right;
		right = left;
		top = rl.top;
		bottom = rl.bottom;
		_middle = _left->_middle;
	}
	if (_ini < _end)
	{
		CFont* oldFont = ss->pDC->SelectObject(ss->pline->_parent->m_style.font + font());
		CString sn(ss->pstr + _ini, _end - _ini);

		int pt = sn.Find(L".");
		if (0 < pt && pt + 1 < sn.GetLength())
			sn = sn.Mid(0, pt);
		else 
			pt = -1;

		CRect cr(this);
		ss->pDC->DrawText(sn, cr, DT_CALCRECT | DT_LEFT | DT_TOP | DT_SINGLELINE);
		right = left + cr.Width();
		if (lnd && rl.Height() != cr.Height())
		{
			top = _middle - cr.Height() / 2 - cr.Height() % 2;
			bottom = top + cr.Height();
		}
		else if (!lnd)
		{
			top = _middle - cr.Height() / 2;
			bottom = top + cr.Height();
		}
		LOGFONT lf;
		ss->pline->_parent->m_style.font[font()].GetLogFont(&lf);
		if (lf.lfItalic)
			right += 2;
		ss->pDC->SelectObject(oldFont);
		if (0 < pt)
		{
			_right = new node_alpha(ss->nd);;
			static_cast<node_alpha*>(_right)->_ini += pt + (ss->bEditing ? 0 : 1);

			ss->pnd = this;
			_right->set(ss);
			_right->rec_move(lf.lfItalic ? -2 : 0, Height() / 2);
			rnd = nullptr;
		}
	}
	if (rnd)
	{
		const tnode* nd = ss->nd;
		ss->nd = rnd;
		ss->pnd = this;
		bnode* lright;
		for (lright = this; static_cast<node_alpha*>(lright)->bnode::_right; lright = static_cast<node_alpha*>(lright)->bnode::_right)
			;
		static_cast<node_alpha*>(lright)->bnode::_right = new_instance(rnd);
		static_cast<node_alpha*>(lright)->bnode::_right->set(ss);
		ss->nd = nd;
	}
	check_error(ss);
	ss->parents.pop_back();
}


short CParserView::line::node_alpha::font() const
{
	return 1;
}


short CParserView::line::node_alpha::color() const
{
	return 3;
}





CParserView::line::node_function::node_function(const tnode* nd)
	: node(nd)
{
}

short CParserView::line::node_function::font() const
{
	return 1;
}

short CParserView::line::node_function::color() const
{
	return 4;
}





CParserView::line::node_buildin_function::node_buildin_function(const tnode* nd)
	: node(nd)
{
}

short CParserView::line::node_buildin_function::font() const
{
	return 1;
}

short CParserView::line::node_buildin_function::color() const
{
	return 5;
}





CParserView::line::node_number::node_number(const tnode* nd)
	: node(nd)
{
}

short CParserView::line::node_number::font() const
{
	return 0;
}

short CParserView::line::node_number::color() const
{
	return 1;
}





CParserView::line::node_string::node_string(const tnode* nd)
	: node(nd)
{
}

short CParserView::line::node_string::font() const
{
	return 0;
}

short CParserView::line::node_string::color() const
{
	return 8;
}






CParserView::line::node_list::node_list(const tnode* nd)
	: node(nd)
{
}

short CParserView::line::node_list::font() const
{
	return 2;
}

short CParserView::line::node_list::color() const
{
	return 7;
}





CParserView::line::node_parenthesis::node_parenthesis(const tnode* nd)
	: node(nd), _nparenthesis(static_cast<const pmb::parser::nodes::parentheses<item, ndtype>*>(nd)->getOpened())
{
}


void CParserView::line::node_parenthesis::set(sset* ss)
{
	const tnode* lnd = ss->nd->getLeft();
	const tnode* rnd = ss->nd->getRight();

	ss->parents.push_back(this);
	if (ss->pnd)
	{
		top = ss->pnd->top;
		bottom = ss->pnd->bottom;
		left = right = ss->pnd->right;
		_middle = ss->pnd->_middle;
	}
	else
	{
		top = ss->pline->top;
		bottom = ss->pline->bottom;

		left = right = ss->pline->left;
		_middle = top + Height() / 2;
	}

	if (lnd)
	{
		const tnode* nd = ss->nd;
		bnode* pnd = ss->pnd;
		ss->nd = lnd;
		ss->pnd = this;
		_left = new_instance(lnd);
		_left->set(ss);
		ss->nd = nd;
		ss->pnd = pnd;
	}

	if (_nparenthesis < 0)
	{
		short nprt = _nparenthesis;
		for (lbnode::const_reverse_iterator p = ss->parents.rbegin(); p != ss->parents.rend(); ++p)
		{
			if ((*p)->parenthesis())
			{
				nprt += (*p)->nparenthesis();
				if (0 <= nprt)
				{

				}
			}
		}
	}
	CFont* oldFont = ss->pDC->SelectObject(ss->pline->_parent->m_style.font + font());
	CString sn(ss->pstr + _ini, ss->nd->len());

	CRect cr(this);
	ss->pDC->DrawText(sn, cr, DT_CALCRECT | DT_LEFT | DT_TOP | DT_SINGLELINE);
	right = left + cr.Width();
	LOGFONT lf;
	ss->pline->_parent->m_style.font[font()].GetLogFont(&lf);
	if (lf.lfItalic)
		right += 2;

	if (rnd)
	{
		const tnode* nd = ss->nd;
		bnode* pnd = ss->pnd;
		ss->nd = rnd;
		ss->pnd = this;
		_right = new_instance(rnd);
		_right->set(ss);
		ss->nd = nd;
		ss->pnd = pnd;
		CRect rr = _right->rec_rect();
		top = rr.top - 2;
		bottom = rr.bottom + 2;
		_middle = _right->_middle;
	}
	check_error(ss);
	ss->parents.pop_back();
}



void CParserView::line::node_parenthesis::draw(sdraw* sd) const
{
	sd->begin_expr(this);
	if (_left)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_left->draw(sd);
		sd->pnd = pnd;
	}
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, sd->pline->_parent->m_style.color[color()]);
	CPen* oldPen = sd->pDC->SelectObject(&pen);
	CRect r(this);
	r.top -= Height();
	r.bottom += Height();
	if (_right)
	{
		r.right = ++r.left + 3 * Height();
		sd->pDC->Arc(r, CPoint(right, top + 1), CPoint(right, bottom - 1));
	}
	else
	{
		r.left = --r.right - 3 * Height();
		sd->pDC->Arc(r, CPoint(left, bottom - 1), CPoint(left, top + 1));
	}
	sd->pDC->SelectObject(oldPen);
	if (_right)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_right->draw(sd);
		sd->pnd = pnd;
	}
	sd->end_expr(this);
}


bool CParserView::line::node_parenthesis::parenthesis() const
{
	return true;
}


short CParserView::line::node_parenthesis::nparenthesis() const
{
	return _nparenthesis;
}


short CParserView::line::node_parenthesis::font() const
{
	return 0;
}

short CParserView::line::node_parenthesis::color() const
{
	return 6;
}





CParserView::line::node_unknown::node_unknown(const tnode* nd)
	: node(nd)
{
}

short CParserView::line::node_unknown::font() const
{
	return 2;
}

short CParserView::line::node_unknown::color() const
{
	return 2;
}





CParserView::line::node_operator_equal::node_operator_equal(const tnode* nd)
	: node(nd)
{
}


void CParserView::line::node_operator_equal::set(sset* ss)
{
	const tnode* lnd = ss->nd->getLeft();
	const tnode* rnd = ss->nd->getRight();

	ss->parents.push_back(this);
	if (ss->pnd)
	{
		top = ss->pnd->top;
		bottom = ss->pnd->bottom;
		left = right = ss->pnd->right;
		_middle = ss->pnd->_middle;
	}
	else
	{
		top = ss->pline->top;
		bottom = ss->pline->bottom;

		left = right = ss->pline->left;
		_middle = top + Height() / 2;
	}

	if (lnd)
	{
		const tnode* nd = ss->nd;
		ss->nd = lnd;
		ss->pnd = this;
		_left = new_instance(lnd);
		_left->set(ss);
		ss->nd = nd;
	}

	CRect rl;
	if (lnd)
	{
		rl = _left->rec_rect();
		left = rl.right;
		right = left + 10;
		top = rl.top;
		bottom = rl.bottom;
		_middle = _left->_middle;
	}
	CFont* oldFont = ss->pDC->SelectObject(ss->pline->_parent->m_style.font + font());
	CString sn(ss->pstr + _ini, ss->nd->len());

	CRect cr(this);
	ss->pDC->DrawText(sn, cr, DT_CALCRECT | DT_LEFT | DT_TOP | DT_SINGLELINE);
	right = left + cr.Width();
	if (lnd && rl.Height() != cr.Height())
	{
		top = _middle - cr.Height() / 2 - cr.Height() % 2;
		bottom = top + cr.Height();
	}
	else if (!lnd)
	{
		bottom = top + cr.Height();
		_middle = top + cr.Height() / 2;
	}
	LOGFONT lf;
	ss->pline->_parent->m_style.font[font()].GetLogFont(&lf);
	if (lf.lfItalic)
		right += 2;

	if (rnd)
	{
		const tnode* nd = ss->nd;
		ss->nd = rnd;
		ss->pnd = this;
		_right = new_instance(rnd);
		_right->set(ss);
		ss->nd = nd;
	}
	else
	{
		CParserDoc* pDoc = ss->pline->_parent->GetDocument();
		if (pDoc && !pDoc->m_result.empty())
		{
			const tnode* nd = ss->nd;
			ss->nd = rnd;
			ss->pnd = this;

			_right = ss->pline->_result = new node_result(nullptr);
			_right->set(ss);
			ss->nd = nd;
		}
	}
	check_error(ss);
	ss->parents.pop_back();
}


void CParserView::line::node_operator_equal::draw(sdraw* sd) const
{
	sd->begin_expr(this);
	if (_left)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_left->draw(sd);
		sd->pnd = pnd;
	}
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, sd->pline->_parent->m_style.color[color()]);
	CPen* oldPen = sd->pDC->SelectObject(&pen);
	sd->pDC->MoveTo(left + 1, _middle - 2);
	sd->pDC->LineTo(right - 1, _middle - 2);
	sd->pDC->MoveTo(left + 1, _middle + 2);
	sd->pDC->LineTo(right - 1, _middle + 2);
	sd->pDC->SelectObject(oldPen);
	if (_right)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_right->draw(sd);
		sd->pnd = pnd;
	}
	sd->end_expr(this);
}


short CParserView::line::node_operator_equal::font() const
{
	return 2;
}

short CParserView::line::node_operator_equal::color() const
{
	return 2;
}









CParserView::line::node_operator_result::node_operator_result(const tnode* nd)
	: node(nd)
{
}


void CParserView::line::node_operator_result::set(sset* ss)
{
	const tnode* lnd = ss->nd->getLeft();
	const tnode* rnd = ss->nd->getRight();

	ss->parents.push_back(this);
	if (ss->pnd)
	{
		top = ss->pnd->top;
		bottom = ss->pnd->bottom;
		left = right = ss->pnd->right;
		_middle = ss->pnd->_middle;
	}
	else
	{
		top = ss->pline->top;
		bottom = ss->pline->bottom;

		left = right = ss->pline->left;
		_middle = top + Height() / 2;
	}

	if (lnd)
	{
		const tnode* nd = ss->nd;
		ss->nd = lnd;
		ss->pnd = this;
		_left = new_instance(lnd);
		_left->set(ss);
		ss->nd = nd;
	}

	CRect rl;
	if (lnd)
	{
		rl = _left->rec_rect();
		left = rl.right;
		right = left + 10;
		top = rl.top;
		bottom = rl.bottom;
		_middle = _left->_middle;
	}
	CFont* oldFont = ss->pDC->SelectObject(ss->pline->_parent->m_style.font + font());
	CString sn(ss->pstr + _ini, ss->nd->len());

	CRect cr(this);
	ss->pDC->DrawText(sn, cr, DT_CALCRECT | DT_LEFT | DT_TOP | DT_SINGLELINE);
	right = left + cr.Width();
	if (lnd && rl.Height() != cr.Height())
	{
		top = _middle - cr.Height() / 2 - cr.Height() % 2;
		bottom = top + cr.Height();
	}
	else if (!lnd)
	{
		bottom = top + cr.Height();
		_middle = top + cr.Height() / 2;
	}
	LOGFONT lf;
	ss->pline->_parent->m_style.font[font()].GetLogFont(&lf);
	if (lf.lfItalic)
		right += 2;

	CParserDoc* pDoc = ss->pline->_parent->GetDocument();
	if (pDoc && !pDoc->m_result.empty())
	{
		const tnode* nd = ss->nd;
		ss->nd = rnd;
		ss->pnd = this;

		_right = ss->pline->_result = new node_result(nullptr);
		_right->set(ss);
		ss->nd = nd;
	}
	if (rnd)
	{
		const tnode* nd = ss->nd;
		ss->nd = rnd;
		ss->pnd = this;
		if (_right)
		{ 
			bnode* bmr = _right->node_mright();
			ss->pline->_ndres = static_cast<node_operator_result*>(bmr)->bnode::_right = new_instance(rnd);
			ss->pline->_ndres->set(ss);
			CRect rmr = ss->pline->_ndres->rec_rect();
			if (rmr.left != _right->right)
				ss->pline->_ndres->rec_move(_right->right - rmr.left, 0);
		}
		else
		{
			_right = new_instance(rnd);
			_right->set(ss);
		}
		ss->nd = nd;
	}
	check_error(ss);
	ss->parents.pop_back();
}


void CParserView::line::node_operator_result::draw(sdraw* sd) const
{
	sd->begin_expr(this);
	if (_left)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_left->draw(sd);
		sd->pnd = pnd;
	}
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, sd->pline->_parent->m_style.color[color()]);
	CPen* oldPen = sd->pDC->SelectObject(&pen);
	sd->pDC->MoveTo(left + 2, _middle - 2);
	sd->pDC->LineTo(right - 2, _middle - 2);
	sd->pDC->MoveTo(left + 2, _middle + 2);
	sd->pDC->LineTo(right - 2, _middle + 2);
	sd->pDC->SelectObject(oldPen);
	if (_right)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_right->draw(sd);
		sd->pnd = pnd;
	}
	sd->end_expr(this);
}


short CParserView::line::node_operator_result::font() const
{
	return 2;
}

short CParserView::line::node_operator_result::color() const
{
	return 2;
}







CParserView::line::node_result::node_result(const tnode* nd)
	: node()
{
	if (nd)
	{
		_ini = nd->getIni();
		_end = nd->getEnd();
	}
	else
		_end = _ini = 0;
}


void CParserView::line::node_result::set(sset* ss)
{
	if (ss->pnd)
	{
		top = ss->pnd->top;
		bottom = ss->pnd->bottom;
		left = right = ss->pnd->right;
		_middle = ss->pnd->_middle;
	}
	else
	{
		top = ss->pline->top;
		bottom = ss->pline->bottom;

		left = right = ss->pline->left;
		_middle = top + Height() / 2;
	}
	CParserDoc* pDoc = ss->pline->_parent->GetDocument();
	if (!pDoc)
		return;

	const tnode* resnd = pDoc->getNodeResult();
	sset nss = { ss->pline, nullptr, this, ss->pDC, pDoc->m_result.c_str(), false };

	_bNodes = ss->pline->_result == this;
	ss->pline->_result = this;

	if (!_bNodes)
	{
		right = left += 10;
		CFont* oldFont = ss->pDC->SelectObject(ss->pline->_parent->m_style.font + font());
		CRect cr(this);
		ss->pDC->DrawText(L" = ", cr, DT_CALCRECT | DT_LEFT | DT_TOP | DT_SINGLELINE);
		right += cr.Width();
		if (Height() != cr.Height())
		{
			top = _middle - cr.Height() / 2 - cr.Height() % 2;
			bottom = top + cr.Height();
		}
		ss->pDC->SelectObject(oldFont);
	}

	if (resnd)
	{
		_ini = _end = 0;

		nss.nd = resnd;
		_right = node::new_instance(nss.nd);
		_right->set(&nss);
		CRect rr = _right->rec_rect();
		max_rect(*this, rr);
	}
}



void CParserView::line::node_result::draw(sdraw* sd) const
{
	sd->begin_expr(this);
	COLORREF bkColor = sd->pDC->GetBkColor();
	if (sd->pline->_parent->m_bEditing)
		sd->pDC->FillSolidRect(this, RGB(0xD0, 0xD0, 0xD0));
	else
		sd->pDC->FillSolidRect(this, RGB(0xF8, 0xF8, 0xF8));

	int oMode = sd->pDC->SetBkMode(TRANSPARENT);

	CParserDoc* pDoc = sd->pline->_parent->GetDocument();
	if (pDoc)
	{
		sdraw nsd = { sd->pline, this, sd->pDC, pDoc->m_result.c_str(), false };

		if (_left)
			_left->draw(&nsd);

		CString sn;
		if (!_bNodes)
			sn = L" = ";
		if (_ini < _end)
			sn += CString(nsd.pstr + _ini, _end - _ini);
		if (!sn.IsEmpty())
		{
			COLORREF oldColor = sd->pDC->SetTextColor(sd->pline->_parent->m_style.color[color()]);
			CFont* oldFont = sd->pDC->SelectObject(sd->pline->_parent->m_style.font + font());
			CRect r(this);
			if (_left)
			{
				r.left = _left->right;
				r.top = _left->_middle - Height() / 2;
				r.bottom = r.top + Height();
			}
			if (_right)
			{
				r.right = _right->left;
				if (!_left)
				{
					r.top = _right->_middle - Height() / 2;
					r.bottom = r.top + Height();
				}
			}
			sd->pDC->DrawText(sn, r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			sd->pDC->SelectObject(oldFont);
			sd->pDC->SetTextColor(oldColor);
			sd->pDC->SetBkColor(bkColor);
		}
		if (_right)
			_right->draw(&nsd);
	}
	else if (_right)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_right->draw(sd);
		sd->pnd = pnd;
	}
	sd->pDC->SetBkMode(oMode);
	sd->end_expr(this);
}


bool CParserView::line::node_result::empty() const
{
	return false;
}

short CParserView::line::node_result::font() const
{
	return 0;
}

short CParserView::line::node_result::color() const
{
	return 1;
}





















CParserView::line::node_operator_root::node_operator_root(const tnode* nd)
	: node(nd)
{
}


void CParserView::line::node_operator_root::set(sset* ss)
{
	const tnode* lnd = ss->nd->getLeft();
	const tnode* rnd = ss->nd->getRight();

	if (rnd)
	{
		ss->parents.push_back(this);
		if (ss->pnd)
		{
			top = ss->pnd->top;
			bottom = ss->pnd->bottom;
			left = right = ss->pnd->right;
			_middle = ss->pnd->_middle;
		}
		else
		{
			top = ss->pline->top;
			bottom = ss->pline->bottom;

			left = right = ss->pline->left;
			_middle = top + Height() / 2;
		}

		const tnode* nd = ss->nd;
		bnode* pnd = ss->pnd;
		CRect rl;
		if (lnd)
		{
			ss->nd = lnd;
			ss->pnd = this;
			_left = new_instance(lnd);
			_left->set(ss);
			rl = _left->rec_rect();
			left = rl.right;
			right = left + 2;
			top = rl.top - 1;
			bottom = rl.bottom;
		}
		{
			CFont* oldFont = ss->pDC->SelectObject(ss->pline->_parent->m_style.font + font());
			CString sn(ss->pstr + _ini, nd->len());

			CRect cr(this);
			ss->pDC->DrawText(sn, cr, DT_CALCRECT | DT_LEFT | DT_TOP | DT_SINGLELINE);
			right = left + cr.Width();
			bottom = top + cr.Height();
			ss->pDC->SelectObject(oldFont);
		}
		ss->nd = rnd;
		ss->pnd = this;
		_right = new_instance(rnd);
		_right->set(ss);
		CRect rr = _right->rec_rect();
		top = rr.top - 2;
		bottom = rr.bottom;
		rright = rr.right;
		if (_left)
		{
			if (2 * rl.Height() < 3 * (rr.Height() - rl.Height()) * (rr.Height() < rl.Height() ? -1 : 1))
				_left->rec_move(0, rr.top - rl.top);
			else
				_left->rec_move(0, rr.top - rl.top - rl.Height() / 2);
		}
		check_error(ss);
		ss->parents.pop_back();
	}
	else
		node::set(ss);
}


void CParserView::line::node_operator_root::draw(sdraw* sd) const
{
	sd->begin_expr(this);
	if (_left)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_left->draw(sd);
		sd->pnd = pnd;
	}
	if (!_right)
	{
		COLORREF oldColor = sd->pDC->SetTextColor(sd->pline->_parent->m_style.color[color()]);
		CFont* oldFont = sd->pDC->SelectObject(sd->pline->_parent->m_style.font + font());
		CString sn(sd->pstr + _ini, _end - _ini);

		sd->pDC->DrawText(sn, const_cast<CRect*>(static_cast<const CRect*>(this)), DT_LEFT | DT_TOP | DT_SINGLELINE);
		sd->pDC->SelectObject(oldFont);
		sd->pDC->SetTextColor(oldColor);
	}
	if (_right)
	{
		CPen pen;
		pen.CreatePen(PS_SOLID, 1, sd->pline->_parent->m_style.color[color()]);
		CPen* oldPen = sd->pDC->SelectObject(&pen);

		int t = _left ? _left->bottom : top + Height() / 3;
		sd->pDC->MoveTo(left + 1, t);
		sd->pDC->LineTo(left + 1 + Width() / 2, bottom);
		sd->pDC->LineTo(right - 1, top + 1);

		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_right->draw(sd);
		sd->pnd = pnd;
		sd->pDC->MoveTo(right - 1, top + 1);
		sd->pDC->LineTo(rright - 4, top + 1);
		sd->pDC->LineTo(rright - 1, top + 6);
		sd->pDC->SelectObject(oldPen);
	}
	sd->end_expr(this);
}


short CParserView::line::node_operator_root::font() const
{
	return 2;
}

short CParserView::line::node_operator_root::color() const
{
	return 2;
}
















CParserView::line::node_operator_power::node_operator_power(const tnode* nd)
	: node(nd)
{
}


void CParserView::line::node_operator_power::set(sset* ss)
{
	const tnode* lnd = ss->nd->getLeft();
	const tnode* rnd = ss->nd->getRight();

	if (lnd && rnd)
	{
		ss->parents.push_back(this);
		if (ss->pnd)
		{
			top = ss->pnd->top;
			bottom = ss->pnd->bottom;
			left = right = ss->pnd->right;
			_middle = ss->pnd->_middle;
		}
		else
		{
			top = ss->pline->top;
			bottom = ss->pline->bottom;

			left = right = ss->pline->left;
			_middle = top + Height() / 2;
		}

		const tnode* nd = ss->nd;
		bnode* pnd = ss->pnd;
		ss->nd = lnd;
		ss->pnd = this;
		_left = new_instance(lnd);
		_left->set(ss);
		CRect rl = _left->rec_rect();
		left = rl.right;
		right = left + 2;
		top = rl.top;
		bottom = rl.bottom;
		if (ss->bEditing)
		{
			CFont* oldFont = ss->pDC->SelectObject(ss->pline->_parent->m_style.font + font());
			CString sn(ss->pstr + _ini, nd->len());

			CRect cr(this);
			ss->pDC->DrawText(sn, cr, DT_CALCRECT | DT_LEFT | DT_TOP | DT_SINGLELINE);
			right = left + cr.Width();
			bottom = top + cr.Height();
			ss->pDC->SelectObject(oldFont);
		}
		_middle = _left->_middle;
		ss->nd = rnd;
		ss->pnd = this;
		_right = new_instance(rnd);
		_right->set(ss);
		CRect rr = _right->rec_rect();
		_right->rec_move(0, rl.top - rr.top + 3 * rl.Height() / 4 - rr.Height());
		check_error(ss);
		ss->parents.pop_back();
	}
	else
		node::set(ss);
}


void CParserView::line::node_operator_power::draw(sdraw* sd) const
{
	sd->begin_expr(this);
	if (_left)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_left->draw(sd);
		sd->pnd = pnd;
	}
	if (sd->bEditing || !_right)
	{
		COLORREF oldColor = sd->pDC->SetTextColor(sd->pline->_parent->m_style.color[color()]);
		CFont* oldFont = sd->pDC->SelectObject(sd->pline->_parent->m_style.font + font());
		CString sn(sd->pstr + _ini, _end - _ini);

		sd->pDC->DrawText(sn, const_cast<CRect*>(static_cast<const CRect*>(this)), DT_LEFT | DT_TOP | DT_SINGLELINE);
		sd->pDC->SelectObject(oldFont);
		sd->pDC->SetTextColor(oldColor);
	}
	if (_right)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_right->draw(sd);
		sd->pnd = pnd;
	}
	sd->end_expr(this);
}


short CParserView::line::node_operator_power::font() const
{
	return 2;
}

short CParserView::line::node_operator_power::color() const
{
	return 2;
}








CParserView::line::node_operator_product::node_operator_product(const tnode* nd)
	: node(nd)
{
}


void CParserView::line::node_operator_product::draw(sdraw* sd) const
{
	sd->begin_expr(this);
	if (_left)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_left->draw(sd);
		sd->pnd = pnd;
	}
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, sd->pline->_parent->m_style.color[color()]);
	CPen* oldPen = sd->pDC->SelectObject(&pen);
	short hm = Height() % 2;
	short wm = Width() % 2;
	sd->pDC->MoveTo(left + 1 - wm, top + Height() / 2 - Width() / 2 + 1);
	sd->pDC->LineTo(right - 1, top + Height() / 2 + Width() / 2 - 1 + hm);
	sd->pDC->MoveTo(left + 1 - wm, top + Height() / 2 + Width() / 2 - 1);
	sd->pDC->LineTo(right - 1, top + Height() / 2 - Width() / 2 + 1 - hm);
	sd->pDC->SelectObject(oldPen);
	if (_right)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_right->draw(sd);
		sd->pnd = pnd;
	}
	sd->end_expr(this);
}


short CParserView::line::node_operator_product::font() const
{
	return 2;
}

short CParserView::line::node_operator_product::color() const
{
	return 2;
}






CParserView::line::node_operator_division_inline::node_operator_division_inline(const tnode* nd)
	: node(nd)
{
}


void CParserView::line::node_operator_division_inline::set(sset* ss)
{
	node::set(ss);
	CRect rl;
	if (_left)
		rl = _left->rec_rect();
	if (_right)
	{
		CRect rr = _right->rec_rect();
		if (_left)
			max_rect(rr, rl);
		if (Height() < rr.Height())
		{
			top = _middle - rr.Height() / 2;
			bottom = top + rr.Height();
		}
	}
	else if (_left)
	{
		if (Height() < rl.Height())
		{
			top = _middle - rl.Height() / 2;
			bottom = top + rl.Height();
		}
	}
}



void CParserView::line::node_operator_division_inline::draw(sdraw* sd) const
{
	sd->begin_expr(this);
	if (_left)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_left->draw(sd);
		sd->pnd = pnd;
	}
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, sd->pline->_parent->m_style.color[color()]);
	CPen* oldPen = sd->pDC->SelectObject(&pen);
	if (sd->bEditing)
	{
		sd->pDC->MoveTo(left + 1, bottom - 2);
		sd->pDC->LineTo(right - 3, top + 2);
		sd->pDC->SetPixel(right - 2, bottom - 2, color());
	}
	else
	{
		sd->pDC->MoveTo(left + 1, bottom - 2);
		sd->pDC->LineTo(right - 2, top + 2);
	}
	sd->pDC->SelectObject(oldPen);
	if (_right)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_right->draw(sd);
		sd->pnd = pnd;
	}
	sd->end_expr(this);
}


short CParserView::line::node_operator_division_inline::font() const
{
	return 2;
}

short CParserView::line::node_operator_division_inline::color() const
{
	return 2;
}








CParserView::line::node_operator_division::node_operator_division(const tnode* nd)
	: node(nd)
{
}


void CParserView::line::node_operator_division::set(sset* ss)
{
	const tnode* lnd = ss->nd->getLeft();
	const tnode* rnd = ss->nd->getRight();

	if (lnd && rnd && ss->pline->_parent->m_fstyle & PMB_STYLE_CPPEDIT_MULTILINE)
	{
		ss->parents.push_back(this);
		if (ss->pnd)
		{
			top = ss->pnd->top;
			bottom = ss->pnd->bottom;
			left = right = ss->pnd->right;
			_middle = ss->pnd->_middle;
		}
		else
		{
			top = ss->pline->top;
			bottom = ss->pline->bottom;

			left = right = ss->pline->left;
			_middle = top + Height() / 2;
		}

		const tnode* nd = ss->nd;
		bnode* pnd = ss->pnd;
		if (lnd)
		{
			ss->nd = lnd;
			ss->pnd = this;
			_left = new_instance(lnd);
			_left->set(ss);
		}

		CRect rl = _left->rec_rect();

		ss->nd = rnd;
		ss->pnd = this;
		_right = new_instance(rnd);
		_right->set(ss);
		ss->nd = nd;
		ss->pnd = pnd;

		CRect rr = _right->rec_rect();
		int width = rr.Width() < rl.Width() ? rl.Width() : rr.Width();
		right = left + width + 2;
		top += _middle - 2;
		bottom = _middle + 4;
		_left->rec_move(width / 2 - rl.Width() / 2, _middle - rl.bottom - 2);
		_right->rec_move(width / 2 - rr.Width() / 2, _middle - rr.top + 2);
		check_error(ss);
		ss->parents.pop_back();
	}
	else
		node::set(ss);
}


void CParserView::line::node_operator_division::draw(sdraw* sd) const
{
	sd->begin_expr(this);
	if (_left)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_left->draw(sd);
		sd->pnd = pnd;
	}
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, sd->pline->_parent->m_style.color[color()]);
	if (_left && _right && sd->pline->_parent->m_fstyle & PMB_STYLE_CPPEDIT_MULTILINE)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_right->draw(sd);
		sd->pnd = pnd;
		CPen* oldPen = sd->pDC->SelectObject(&pen);
		sd->pDC->MoveTo(left + 1, _middle);
		sd->pDC->LineTo(right - 1, _middle);
		sd->pDC->SelectObject(oldPen);
	}
	else
	{
		CPen* oldPen = sd->pDC->SelectObject(&pen);
		sd->pDC->MoveTo(right - 1, top - 1);
		sd->pDC->LineTo(left + 1, bottom + 1);
		sd->pDC->SelectObject(oldPen);
		if (_right)
		{
			const bnode* pnd = sd->pnd;
			sd->pnd = this;
			_right->draw(sd);
			sd->pnd = pnd;
		}
	}
	sd->end_expr(this);
}


short CParserView::line::node_operator_division::font() const
{
	return 2;
}

short CParserView::line::node_operator_division::color() const
{
	return 2;
}









CParserView::line::node_operator_plus::node_operator_plus(const tnode* nd)
	: node(nd)
{
}


void CParserView::line::node_operator_plus::draw(sdraw* sd) const
{
	sd->begin_expr(this);
	if (_left)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_left->draw(sd);
		sd->pnd = pnd;
	}
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, sd->pline->_parent->m_style.color[color()]);
	CPen* oldPen = sd->pDC->SelectObject(&pen);
	short hm = Height() % 2;
	short wm = Width() % 2;
	sd->pDC->MoveTo(left + Width() / 2 + wm, top + Height() / 2 - Width() / 2 + 1);
	sd->pDC->LineTo(left + Width() / 2 + wm, top + Height() / 2 + Width() / 2 - 1 + hm);
	sd->pDC->MoveTo(left + 1, _middle);
	sd->pDC->LineTo(right - 1, _middle);
	sd->pDC->SelectObject(oldPen);
	if (_right)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_right->draw(sd);
		sd->pnd = pnd;
	}
	sd->end_expr(this);
}


short CParserView::line::node_operator_plus::font() const
{
	return 2;
}

short CParserView::line::node_operator_plus::color() const
{
	return 2;
}






CParserView::line::node_operator_minus::node_operator_minus(const tnode* nd)
	: node(nd)
{
}


void CParserView::line::node_operator_minus::draw(sdraw* sd) const
{
	sd->begin_expr(this);
	if (_left)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_left->draw(sd);
		sd->pnd = pnd;
	}
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, sd->pline->_parent->m_style.color[color()]);
	CPen* oldPen = sd->pDC->SelectObject(&pen);
	sd->pDC->MoveTo(left + 1, _middle);
	sd->pDC->LineTo(right - 1, _middle);
	sd->pDC->SelectObject(oldPen);
	if (_right)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_right->draw(sd);
		sd->pnd = pnd;
	}
	sd->end_expr(this);
}


short CParserView::line::node_operator_minus::font() const
{
	return 2;
}

short CParserView::line::node_operator_minus::color() const
{
	return 2;
}












CParserView::line::line(CParserView* parent)
	: _parent(parent), _root(nullptr), _result(nullptr), _ndres(nullptr)
{
}

CParserView::line::~line()
{
	if (_root)
		delete _root;
}


void CParserView::line::set(CDC* pDC, int xo, int yo)
{
	if (_root)
	{
		delete _root;
		_root = nullptr;
		_result = nullptr;
		_ndres = nullptr;
	}
	_nerror = nullptr;

	left = right = xo;
	top = bottom = yo;

	CParserDoc* pDoc = _parent->GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	const tnode* nd = pDoc->getNodeRoot();
	if (!nd)
		return;

	_root = node::new_instance(nd);
	if (_root)
	{
		sset ss = { this, nd, nullptr, pDC, _parent->m_expr.c_str(), _parent->m_bEditing };
		_root->set(&ss);
		_root->end(&ss);
		normalize(xo, yo);
	}
}



void CParserView::line::normalize(int xo, int yo)
{
	if (!_root->empty())
	{
		CRect cr = _root->rec_rect();
		if (xo != cr.left || yo != cr.top)
		{
			int dx = xo - cr.left;
			int dy = yo - cr.top;
			_root->rec_move(dx, dy);
		}
		left = xo;
		top = yo;
		right = xo + cr.Width();
		bottom = yo + cr.Height();
	}
	else
	{
		left = right = xo;
		top = bottom = yo;
	}
}


void CParserView::line::check_error(const tnode* nd, const bnode* nnd)
{
	CParserDoc* pDoc = _parent->GetDocument();
	if (pDoc && nd == pDoc->m_error.item())
		_nerror = nnd;
}


void CParserView::line::draw_error(CDC* pDC)
{
	if (_nerror)
	{
		CPen pen(PS_SOLID, 1, RGB(0xC0, 0x00, 0x00));
		CPen* oldPen = pDC->SelectObject(&pen);
		int x0 = _nerror->left;
		int x1 = _nerror->right;
		if (x1 - x0 < 6)
		{
			x0 -= 3;
			x1 += 3;
		}
		_parent->m_error = CRect(x0, _nerror->top, x1, _nerror->bottom);
		pDC->MoveTo(x0, _nerror->bottom);
		for (int x = x0 + 1; x < x1; ++x)
			pDC->LineTo(x, _nerror->bottom - (x - x0) % 2);
		pDC->SelectObject(oldPen);
	}
}



void CParserView::line::draw(CDC* pDC)
{
	if (_root && !_root->empty())
	{
		sdraw sd = { this, nullptr, pDC, _parent->m_expr.c_str(), _parent->m_bEditing };
		_root->draw(&sd);
		draw_error(pDC);
	}
}



void CParserView::line::_swith_expr_begin(const bnode* pnd, sdraw* sd) const
{
	if (_ndres == pnd)
	{
		sd->postr = sd->pstr;
		sd->pstr = _parent->m_expr.c_str();
		sd->pDC->SetBkMode(OPAQUE);
		sd->pDC->SetBkColor(RGB(0xFF, 0xFF, 0xFF));
		sd->bEditing = true;
	}
}



void CParserView::line::_swith_expr_end(const bnode* pnd, sdraw* sd) const
{
	if (_ndres == pnd)
	{
		sd->pstr = sd->postr;
		sd->pDC->SetBkMode(TRANSPARENT);
		sd->bEditing = false;
	}
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
	{
		pen.CreatePen(PS_SOLID, 1, m_style.color[2]);
		pDC->SetBkMode(OPAQUE);
	}
	bool bResult = false;

	COLORREF oldColor;
	CFont* oldFont = nullptr;
	CPen* oldPen = bCalc ? nullptr : pDC->SelectObject(&pen);
	CRect tr(m_p0.x, m_p0.y, m_p0.x, m_p0.y + m_style.maxHeight);
	int max_x = m_p0.x;
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
			int resultWidth = 0;
			if (nd->getType() == pmb::parser::ndUnknown)
			{
				const pmb::parser::nodes::unknown<item, ndtype>* uk = static_cast<const pmb::parser::nodes::unknown<item, ndtype>*>(nd);
				if (uk->isValid() && !uk->isCallFunction())
				{
					const operation* opr = reinterpret_cast<const operation*>(uk->pointer());
					if (!strcmp(opr->getSymbol(), "=:") && (!pDoc->m_result.empty() && pDoc->m_error.empty()))
					{
						of = pDC->SelectObject(m_style.font);
						sn = pDoc->m_result.c_str();
						pDC->DrawText(sn, ctr, DT_CALCRECT | DT_LEFT | DT_TOP | DT_SINGLELINE);
						of = pDC->SelectObject(m_style.font + font);
						resultWidth = ctr.Width();
					}
				}
			}
			if (x_pos && *x_pos < tr.right || !x_pos && m_style.caret[1] <= nd->getEnd())
			{
				int x = tr.left;
				for (int i = nd->getIni(); i < nd->getEnd(); ++i)
				{
					ABC width;
					pDC->GetCharABCWidths(m_expr[i], m_expr[i], &width);
					//pDC->GetOutputCharWidth(m_expr[i], m_expr[i], &width);
					x += width.abcA + width.abcB + width.abcC;
					if (i + 1 == nd->getEnd())
						x += resultWidth;
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
					const operation* opr = reinterpret_cast<const operation*>(uk->pointer());
					if (!strcmp(opr->getSymbol(), "=") || !strcmp(opr->getSymbol(), "=:"))
					{
						bDrawText = false;
						pDC->MoveTo(tr.left, tr.top + tr.Height() / 2 - 2);
						pDC->LineTo(tr.right, tr.top + tr.Height() / 2 - 2);
						pDC->MoveTo(tr.left, tr.top + tr.Height() / 2 + 2);
						pDC->LineTo(tr.right, tr.top + tr.Height() / 2 + 2);
						bResult = !opr->isBinary() && opr->isLeftToRight() || !strcmp(opr->getSymbol(), "=:");
						if (bResult && (!pDoc->m_result.empty() && pDoc->m_error.empty()))
						{
							of = pDC->SelectObject(m_style.font);
							pDC->SetTextColor(m_style.color[1]);
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
			sr = pDoc->m_error.message(pDoc->m_expr).c_str();
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

	bool bModified = false;

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
	case VK_DELETE:
		if (m_style.caret[1] < m_expr.size() - 1)
		{
			bModified = true;
			std::string nexpr = m_expr.substr(0, m_style.caret[1]);
			if (m_style.caret[1] + 1 < m_expr.size())
				nexpr += m_expr.substr(m_style.caret[1] + 1);
			m_expr = nexpr;
		}
		break;
	default:
		break;
	}
	if (m_style.caret[1] < 0)
		m_style.caret[1] = 0;
	else if (m_expr.size() < m_style.caret[1])
		m_style.caret[1] = m_expr.size();

	if (bModified)
	{
		CDC* pDC = GetDC();
		GetDocument()->update(m_expr.c_str());
		m_line.set(pDC, 0, 40);
		InvalidateRect(nullptr, 1);
		ReleaseDC(pDC);
	}
	if (m_style.caret[1] != old1)
	{
		CDC* pDC = GetDC();
		draw_line(pDC, true);
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




void CParserView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	HideCaret();

	bool bModified = false;

	int old0 = m_style.caret[0];
	int old1 = m_style.caret[1];
	switch (nChar)
	{
	case VK_BACK:
		if (0 < m_style.caret[1])
		{
			bModified = true;
			std::string nexpr = m_expr.substr(0, m_style.caret[1] - 1);
			if (m_style.caret[1] < m_expr.size())
				nexpr += m_expr.substr(m_style.caret[1]);
			m_expr = nexpr;
			--m_style.caret[1];
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
			if (0 < m_style.caret[1])
				nexpr = m_expr.substr(0, m_style.caret[1]);
			nexpr += (char)nChar;
			if (m_style.caret[1] < m_expr.size())
				nexpr += m_expr.substr(m_style.caret[1]);
			m_expr = nexpr;
			++m_style.caret[1];
		}
		break;
	}
	if (m_style.caret[1] < 0)
		m_style.caret[1] = 0;
	else if (m_expr.size() < m_style.caret[1])
		m_style.caret[1] = m_expr.size();

	if (bModified)
	{
		GetDocument()->update(m_expr.c_str());
		CDC* pDC = GetDC();
		m_line.set(pDC, 0, 40);
		ReleaseDC(pDC);
		InvalidateRect(nullptr, 1);
	}
	if (m_style.caret[1] != old1)
	{
		CDC* pDC = GetDC();
		draw_line(pDC, true);
		ReleaseDC(pDC);
		SetCaretPos(m_style.caretPos);

		if (!(GetKeyState(VK_LSHIFT) & 0x8000))
		{
			m_style.caret[0] = m_style.caret[1];
			m_style.caretPos0 = m_style.caretPos;
		}
	}

	ShowCaret();

	CView::OnChar(nChar, nRepCnt, nFlags);
}



void CParserView::OnLButtonDown(UINT nFlags, CPoint point)
{
	HideCaret();

	if (m_p0.y <= point.y && point.y <= m_p0.y + m_style.maxHeight)
	{
		CDC* pDC = GetDC();
		int x = point.x;
		draw_line(pDC, true, &x);
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


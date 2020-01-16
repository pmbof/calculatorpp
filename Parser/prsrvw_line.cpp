
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::line(CParserView* parent)
	: _parent(parent), _root(nullptr), _result(nullptr), _ndres(nullptr)
{
}




CParserView::line::~line()
{
	if (_root)
	{
		pmb::log* lg = pmb::log::beginFunction(pmb::logDebug, __FUNCTION__);
		lg->trace(pmb::logDebug, "Root node: 0x%08X\n", _root);
		for (bnode* nd = _root->get_first(); nd; )
		{
			bnode* next = nd->get_next();
			lg->trace(pmb::logDebug, "Deleting node 0x%08X\n", nd);
			delete nd;
			nd = next;
		}
		lg->endFunction(pmb::logDebug);
	}
}


void CParserView::line::clear()
{
	if (_root)
	{
		pmb::log* lg = pmb::log::beginFunction(pmb::logDebug, __FUNCTION__);
		lg->trace(pmb::logDebug, "Root node: 0x%08X\n", _root);
		for (bnode* nd = _root->get_first(); nd; )
		{
			bnode* next = nd->get_next();
			lg->trace(pmb::logDebug, "Deleting node 0x%08X\n", nd);
			delete nd;
			nd = next;
		}
		_root = nullptr;
		_result = nullptr;
		_ndres = nullptr;
		lg->endFunction(pmb::logDebug);
	}
	_nerror = nullptr;
}



void CParserView::line::set(CDC* pDC, int xo, int yo)
{
	clear();

	_style.clear();
	_style[bndOther] = "@operators";
	_style[bndSpace] = "@operators";
	_style[bndAlpha] = "@variables";
	_style[bndFunction] = "@user-def functions";
	_style[bndBIFunction] = "@Build-in functions";
	_style[bndNumber] = "@numbers";
	_style[bndString] = "@strings";
	_style[bndList] = "@list";
	_style[bndParentheses] = "@parentheses";
	_style[bndUnknow] = "@operators";
	_style[bndOprEqual] = "@operators";
	_style[bndOprResult] = "@operators";
	_style[bndResult] = "@operators";
	_style[bndOprRoot] = "@operators";
	_style[bndOprPower] = "@operators";
	_style[bndOprProduct] = "@operators";
	_style[bndOprDivisionIl] = "@operators";
	_style[bndOprDivision] = "@operators";
	_style[bndOprPlus] = "@operators";
	_style[bndOprMinus] = "@operators";

	left = right = xo;
	top = bottom = yo;

	CParserDoc* pDoc = _parent->GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	const tnode* nd = pDoc->getNodeRoot();
	if (!nd)
		return;

	pDoc->check_operation_table();
	if (node::new_instance(&_root, nullptr, nd))
	{
		sset ss = { this, nd, nullptr, pDC, _parent->m_expr.c_str(), _parent->m_bEditing };
		_root->set(&ss);
		_root->end(&ss);
		normalize(xo, yo);
		_root->debug_check_all();
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



CFont* CParserView::line::font(bnodetypes type, short index)
{
	mstyletp::const_iterator s = _style.find(type);
	if (s != _style.end())
		return _parent->m_resource.font(s->second, index);
	
	return _parent->m_resource.font(index);
}



COLORREF CParserView::line::color(bnodetypes type) const
{
	mstyletp::const_iterator s = _style.find(type);
	if (s != _style.end())
		return _parent->m_resource.color(s->second);

	return _parent->m_resource.color();
}



COLORREF CParserView::line::back_color() const
{
	return _parent->m_resource.back_color();
}





bool CParserView::line::operator()(scaret& caret) const
{
	for (short c = 0; c < 2; ++c)
	{
		if (caret.spos[c] < 0)
			caret.spos[c] = 0;
		else if (_parent->m_expr.size() < caret.spos[c])
			caret.spos[c] = _parent->m_expr.size();
	}

	const bnode* nend = nullptr;
	for (const bnode* nd = _root->get_first(); nd; nd = nd->get_next())
	{
		if (nd->get_ini() <= caret.spos[1] && caret.spos[1] < nd->get_end())
		{
			caret.pos[1].x = nd->left;
			caret.pos[1].y = nd->top;
			caret.height = nd->Height();
			return true;
		}
		if (caret.spos[1] == nd->get_end())
			nend = nd;
	}
	if (nend)
	{
		caret.pos[1].x = nend->right;
		caret.pos[1].y = nend->top;
		caret.height = nend->Height();
	}
	return nend;
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

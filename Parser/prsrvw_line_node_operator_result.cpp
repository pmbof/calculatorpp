
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::node_operator_result::node_operator_result(bnode* parent, const tnode* nd)
	: node(parent, nd)
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
		new_instance(&_left, this, lnd)->set(ss);
		ss->nd = nd;
	}

	CRect rl;
	if (lnd)
	{
		rl = _left->rect();
		left = rl.right;
		right = left + 10;
		top = rl.top;
		bottom = rl.bottom;
		_middle = _left->_middle;
	}
	CFont* pFont = ss->pline->font(type(), ss->index);
	CFont* oldFont = ss->pDC->SelectObject(pFont);
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
	pFont->GetLogFont(&lf);
	if (lf.lfItalic)
		right += 2;

	CParserDoc* pDoc = ss->pline->_parent->GetDocument();
	if (pDoc && !pDoc->m_result.empty())
	{
		const tnode* nd = ss->nd;
		ss->nd = rnd;
		ss->pnd = this;

		_right = ss->pline->_result = new node_result(this, nullptr);
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
			ss->pline->_ndres = new_instance(&static_cast<node_operator_result*>(bmr)->bnode::_right, bmr, rnd);
			ss->pline->_ndres->set(ss);
			CRect rmr = ss->pline->_ndres->rect();
			if (rmr.left != _right->right)
				ss->pline->_ndres->rect_move(_right->right - rmr.left, 0);
		}
		else
			new_instance(&_right, this, rnd)->set(ss);
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
	pen.CreatePen(PS_SOLID, 1, sd->pline->color(type()));
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




CParserView::line::bnodetypes CParserView::line::node_operator_result::type() const
{
	return bndOprResult;
}


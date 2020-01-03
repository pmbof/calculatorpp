
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


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
			CFont* pFont = ss->pline->font(type(), ss->index);
			CFont* oldFont = ss->pDC->SelectObject(pFont);
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
		++ss->index;
		_right->set(ss);
		--ss->index;
		CRect rr = _right->rec_rect();
		_right->rec_move(0, rl.top - rr.top - rr.Height() + _right->_middle - rr.top);
		if (_left->_middle < _right->_middle + 10)
			_right->rec_move(0, rl.top - _right->_middle);
		if (_left->_middle < _right->bottom)
			_right->rec_move(0, _left->_middle - _right->bottom);

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
		COLORREF oldColor = sd->pDC->SetTextColor(sd->pline->color(type()));
		CFont* pFont = sd->pline->font(type(), sd->index);
		CFont* oldFont = sd->pDC->SelectObject(pFont);
		CString sn(sd->pstr + _ini, _end - _ini);

		sd->pDC->DrawText(sn, const_cast<CRect*>(static_cast<const CRect*>(this)), DT_LEFT | DT_TOP | DT_SINGLELINE);
		sd->pDC->SelectObject(oldFont);
		sd->pDC->SetTextColor(oldColor);
	}
	if (_right)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		++sd->index;
		_right->draw(sd);
		--sd->index;
		sd->pnd = pnd;
	}
	sd->end_expr(this);
}




CParserView::line::bnodetypes CParserView::line::node_operator_power::type() const
{
	return bndOprPower;
}


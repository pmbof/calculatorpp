
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::node_operator_root::node_operator_root(bnode* parent, const tnode* nd)
	: node(parent, nd)
{
}




void CParserView::line::node_operator_root::set(sset* ss)
{
	const tnode* lnd = ss->nd->getLeft();
	const tnode* rnd = ss->nd->getRight();

	if (rnd)
	{
		if (_parent)
		{
			top = _parent->top;
			bottom = _parent->bottom;
			left = right = _parent->right;
			_middle = _parent->_middle;
		}
		else
		{
			top = ss->pline->top;
			bottom = ss->pline->bottom;

			left = right = ss->pline->left;
			_middle = top + Height() / 2;
		}

		const tnode* nd = ss->nd;
		CRect rl;
		if (lnd)
		{
			ss->nd = lnd;
			new_instance(&_left, this, lnd);
			++ss->index;
			_left->set(ss);
			--ss->index;
			rl = _left->rect();
			left = rl.right;
			right = left + 2;
			top = rl.top - 1;
			bottom = rl.bottom;
		}
		{
			CFont* pFont = ss->pline->font(type(), ss->index);
			CFont* oldFont = ss->pDC->SelectObject(pFont);
			CString sn(ss->pstr + _ini, nd->len());

			CRect cr(this);
			ss->pDC->DrawText(sn, cr, DT_CALCRECT | DT_LEFT | DT_TOP | DT_SINGLELINE);
			right = left + cr.Width() + 1;
			bottom = top + cr.Height();
			ss->pDC->SelectObject(oldFont);
		}
		ss->nd = rnd;
		new_instance(&_right, this, rnd)->set(ss);
		CRect rr = _right->rect();
		bool bdouble = 3 * Height() < 2 * rr.Height();
		top = rr.top - 2;
		bottom = rr.bottom;
		rright = rr.right;
		if (bdouble)
		{
			const int dy = Width();
			rright += dy;
			right += dy;
			_right->rect_move(dy, 0);
		}
		if (_left)
		{
			if (2 * rl.Height() < 3 * (rr.Height() - rl.Height()) * (rr.Height() < rl.Height() ? -1 : 1))
				_left->rect_move(0, rr.top - rl.top);
			else
				_left->rect_move(0, rr.top - rl.top - rl.Height() / 2);
		}
		check_error(ss);
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
		++sd->index;
		_left->draw(sd);
		--sd->index;
		sd->pnd = pnd;
	}
	if (!_right)
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
		CPen pen;
		pen.CreatePen(PS_SOLID, 1, sd->pline->color(type()));
		CPen* oldPen = sd->pDC->SelectObject(&pen);

		int t;
		if (_left)
		{
			CRect rl = _left->rect();
			t = rl.bottom;
			sd->pDC->MoveTo(rl.left + rl.Width() / 2, t + 1);
		}
		else
		{
			t = top + Height() / 3;
			sd->pDC->MoveTo(left - 1, t + 1);
		}
		sd->pDC->LineTo(left + 1, t);
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




CParserView::line::bnodetypes CParserView::line::node_operator_root::type() const
{
	return bndOprRoot;
}

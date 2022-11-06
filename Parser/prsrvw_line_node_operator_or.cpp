
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::node_operator_or::node_operator_or(const tnode* nd, bnode* parent)
	: node(nd, parent)
{
}



void CParserView::line::node_operator_or::set(sset* ss)
{
	const tnode* lnd = _ptnd->getLeft();
	const tnode* rnd = _ptnd->getRight();

	if (_parent)
		set_rect_fromparent();
	else
	{
		top = ss->pline->top;
		bottom = ss->pline->bottom;

		left = right = ss->pline->left;
		_middle = top + Height() / 2;
	}

	if (lnd)
	{
		ss->tnd = lnd;
		new_instance(&_left, this, lnd)->set(ss);
		ss->tnd = _ptnd;
	}

	CRect rl;
	if (lnd)
	{
		rl = _left->rect();
		left = rl.right;
		right = left;
		top = rl.top;
		bottom = rl.bottom;
		_middle = _left->_middle;
	}
	if (get_ini() < get_end())
	{
		CFont* pFont = ss->pline->font(type(), ss->index);
		CFont* oldFont = ss->pDC->SelectObject(pFont);

		CSize te;
		CStringA str = "&";
		GetTextExtentPointA(ss->pDC->m_hDC, str, get_length(), &te);

		right = left + 2 * te.cx / 3;
		if (lnd && rl.Height() != te.cy)
		{
			top = _middle - te.cy / 2 - te.cy % 2;
			bottom = top + te.cy;
		}
		else if (!lnd)
		{
			top = _middle - te.cy / 2;
			bottom = top + te.cy;
		}
		ss->pDC->SelectObject(oldFont);
	}
	if (rnd)
	{
		ss->tnd = rnd;
		new_instance(&_right, this, rnd)->set(ss);
		ss->tnd = _ptnd;
	}
	check_error(ss);
}



void CParserView::line::node_operator_or::draw(sdraw* sd) const
{
	sd->begin_expr(this);
	if (_left)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_left->draw(sd);
		sd->pnd = pnd;
	}

	const short w2 = Width() / 2;
	const short w4 = Width() / 4;
	const short mx = left + w2;
	const bool bw = Height() < Width() - 2;
	const short hw = bw ? Height() / 2 : w2 - 1;
	const short mh = Height() / 5;

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, sd->pline->color(type()));
	CPen* oldPen = sd->pDC->SelectObject(&pen);

	sd->pDC->MoveTo(left + w4, _middle - hw);
	sd->pDC->LineTo(mx, _middle + mh);
	sd->pDC->LineTo(right - w4, _middle - hw);

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




inline CParserView::line::bnodetypes CParserView::line::node_operator_or::type() const
{
	return bndOprOr;
}

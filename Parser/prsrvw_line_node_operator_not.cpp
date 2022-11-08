
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes



CParserView::line::node_operator_not::node_operator_not(const tnode* nd, bnode* parent)
	: node(nd, parent)
{
}



void CParserView::line::node_operator_not::set(sset* ss)
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

	if (lnd)
	{
		CRect rl;
		rl = _left->rect();
		left = rl.right;
		right = left;
		top = rl.top;
		bottom = rl.bottom;
		_middle = _left->_middle;
	}

	if (get_ini() < get_end())
	{
		in_set(ss);
	}

	if (rnd)
	{
		ss->tnd = rnd;
		new_instance(&_right, this, rnd)->set(ss);
		ss->tnd = _ptnd;
	}
	check_error(ss);
}



void CParserView::line::node_operator_not::in_set(sset* ss)
{
	const tnode* lnd = _ptnd->getLeft();
	const tnode* rnd = _ptnd->getRight();

	CRect rl;
	if (lnd)
		rl = _left->rect();

	CFont* pFont = ss->pline->font(type(), ss->index);
	CFont* oldFont = ss->pDC->SelectObject(pFont);

	CSize te;
	GetTextExtentPointA(ss->pDC->m_hDC, ss->pstr + get_ini(), get_length(), &te);

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



void CParserView::line::node_operator_not::draw(sdraw* sd) const
{
	sd->begin_expr(this);
	if (_left)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_left->draw(sd);
		sd->pnd = pnd;
	}

	in_draw(sd);

	if (_right)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_right->draw(sd);
		sd->pnd = pnd;
	}
	sd->end_expr(this);
}



void CParserView::line::node_operator_not::in_draw(sdraw* sd) const
{
	COLORREF oldColor = sd->pDC->SetTextColor(sd->pline->color(type()));
	CFont* oldFont = sd->pDC->SelectObject(sd->pline->font(type(), sd->index));
	CString sn(sd->pstr + get_ini(), get_length());

	sd->pDC->DrawText(sn, const_cast<CRect*>(static_cast<const CRect*>(this)), DT_LEFT | DT_TOP | DT_SINGLELINE | DT_NOPREFIX);
	sd->pDC->SetTextColor(oldColor);
	sd->pDC->SelectObject(oldFont);
}



inline CParserView::line::bnodetypes CParserView::line::node_operator_not::type() const
{
	return bndOprLogical;
}

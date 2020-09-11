
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::node_number::node_number(bnode* parent, const tnode* nd)
	: node(parent, nd)
{
}



void CParserView::line::node_number::set(sset* ss)
{
	const tnode* lnd = ss->nd->getLeft();
	const tnode* rnd = ss->nd->getRight();

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
		const tnode* nd = ss->nd;
		ss->nd = lnd;
		new_instance(&_left, this, lnd)->set(ss);
		ss->nd = nd;
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
	if (_ini < _end)
	{
		CFont* pFont = ss->pline->font(type(), ss->index);
		CFont* oldFont = ss->pDC->SelectObject(pFont);

		CSize te;
		GetTextExtentPointA(ss->pDC->m_hDC, ss->pstr + _ini, ss->nd->len(), &te);

		right = left + te.cx;
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
		LOGFONT lf;
		pFont->GetLogFont(&lf);
		if (lf.lfItalic)
			right += 2;
		ss->pDC->SelectObject(oldFont);
	}
	if (rnd)
	{
		const tnode* nd = ss->nd;
		ss->nd = rnd;
		new_instance(&_right, this, rnd)->set(ss);
		ss->nd = nd;
	}
	check_error(ss);
}




inline CParserView::line::bnodetypes CParserView::line::node_number::type() const
{
	return bndNumber;
}


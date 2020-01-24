
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::node_operator_division_inline::node_operator_division_inline(bnode* parent, const tnode* nd)
	: node(parent, nd)
{
}




void CParserView::line::node_operator_division_inline::set(sset* ss)
{
	node::set(ss);
	CRect rl;
	if (_left)
		rl = _left->rect();
	if (_right)
	{
		CRect rr = _right->rect();
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
	pen.CreatePen(PS_SOLID, 1, sd->pline->color(type()));
	CPen* oldPen = sd->pDC->SelectObject(&pen);
	if (sd->bEditing)
	{
		sd->pDC->MoveTo(left + 1, bottom - 2);
		sd->pDC->LineTo(right - 3, top + 2);
		sd->pDC->SetPixel(right - 2, bottom - 2, sd->pline->color(type()));
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



CParserView::line::bnodetypes CParserView::line::node_operator_division_inline::type() const
{
	return bndOprDivisionIl;
}

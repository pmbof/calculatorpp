
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::node_operator_plus::node_operator_plus(const tnode* nd, bnode* parent)
	: node(nd, parent)
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

	const short w2 = Width() / 2;
	const short mx = left + w2;
	const bool bw = Height() < Width() - 2;
	const short hw = bw ? Height() / 2 : w2;

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, sd->pline->color(type()));
	CPen* oldPen = sd->pDC->SelectObject(&pen);

	sd->pDC->MoveTo(mx,         _middle - hw);
	sd->pDC->LineTo(mx,         _middle + hw + 1);

	sd->pDC->MoveTo(mx - hw,    _middle);
	sd->pDC->LineTo(mx+ hw + 1, _middle);

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




inline CParserView::line::bnodetypes CParserView::line::node_operator_plus::type() const
{
	return bndOprPlus;
}


#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::node_operator_product::node_operator_product(const tnode* nd, bnode* parent)
	: node(nd, parent)
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

	const short w2 = Width() / 2;
	const short mx = left + w2;
	const bool bw = Height() < Width() - 2;
	const short hw = bw ? Height() / 2 : w2 - 1;
	const short mh = bw ? 0 : 1;
	const short mw = bw ? 1 : 0;

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, sd->pline->color(type()));
	CPen* oldPen = sd->pDC->SelectObject(&pen);

	sd->pDC->MoveTo(mx - hw,      _middle - hw);
	sd->pDC->LineTo(mx + hw + mw, _middle + hw + mh);

	sd->pDC->MoveTo(mx - hw,      _middle + hw + mh);
	sd->pDC->LineTo(mx + hw - mw, _middle - hw);

	sd->pDC->SetPixel(mx + hw + mw, _middle + hw + mh, sd->pline->color(type()));
	sd->pDC->SetPixel(mx + hw - mw, _middle - hw, sd->pline->color(type()));

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




inline CParserView::line::bnodetypes CParserView::line::node_operator_product::type() const
{
	return bndOprProduct;
}


#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::node_operator_product::node_operator_product(const tnode* nd)
	: node(nd)
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
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, sd->pline->color(type()));
	CPen* oldPen = sd->pDC->SelectObject(&pen);
	short hm = Height() % 2;
	short wm = Width() % 2;
	sd->pDC->MoveTo(left + 1 - wm, top + Height() / 2 - Width() / 2 + 1);
	sd->pDC->LineTo(right - 1, top + Height() / 2 + Width() / 2 - 1 + hm);
	sd->pDC->MoveTo(left + 1 - wm, top + Height() / 2 + Width() / 2 - 1);
	sd->pDC->LineTo(right - 1, top + Height() / 2 - Width() / 2 + 1 - hm);
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




CParserView::line::bnodetypes CParserView::line::node_operator_product::type() const
{
	return bndOprProduct;
}

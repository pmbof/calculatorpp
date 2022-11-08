
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes



CParserView::line::node_operator_or::node_operator_or(const tnode* nd, bnode* parent)
	: node_operator_and(nd, parent)
{
}



void CParserView::line::node_operator_or::in_draw(sdraw* sd) const
{
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
}


#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes



CParserView::line::node_operator_implication_reverse::node_operator_implication_reverse(const tnode* nd, bnode* parent)
	: node_operator_implication(nd, parent)
{
}



void CParserView::line::node_operator_implication_reverse::in_draw(sdraw* sd) const
{
	const short wf = 3 * Width() / 8;
	const short hf = Height() / 5;

	const short mrx = 4 < wf ? 2 : 2 < wf ? 1 : 0;

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, sd->pline->color(type()));
	CPen* oldPen = sd->pDC->SelectObject(&pen);

	sd->pDC->MoveTo(right - mrx, _middle);
	sd->pDC->LineTo(left + mrx, _middle);
	sd->pDC->LineTo(left + mrx + wf, _middle - hf);
	sd->pDC->MoveTo(left + mrx, _middle);
	sd->pDC->LineTo(left + mrx + wf, _middle + hf);

	sd->pDC->SelectObject(oldPen);
}

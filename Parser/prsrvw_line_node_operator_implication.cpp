
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes



CParserView::line::node_operator_implication::node_operator_implication(const tnode* nd, bnode* parent)
	: node_operator_not(nd, parent)
{
}



void CParserView::line::node_operator_implication::in_set(sset* ss)
{
	const tnode* lnd = _ptnd->getLeft();
	const tnode* rnd = _ptnd->getRight();

	CRect rl;
	if (lnd)
		rl = _left->rect();

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



void CParserView::line::node_operator_implication::in_draw(sdraw* sd) const
{
	const short wf = 3 * Width() / 8;
	const short hf = Height() / 5;

	const short mrx = 4 < wf ? 2: 2 < wf ? 1 : 0;

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, sd->pline->color(type()));
	CPen* oldPen = sd->pDC->SelectObject(&pen);

	sd->pDC->MoveTo(left + mrx, _middle);
	sd->pDC->LineTo(right - mrx, _middle);
	sd->pDC->LineTo(right - mrx - wf, _middle - hf);
	sd->pDC->MoveTo(right - mrx, _middle);
	sd->pDC->LineTo(right - mrx - wf, _middle + hf);

	sd->pDC->SelectObject(oldPen);
}

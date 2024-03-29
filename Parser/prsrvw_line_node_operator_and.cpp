
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes



CParserView::line::node_operator_and::node_operator_and(const tnode* nd, bnode* parent)
	: node_operator_not(nd, parent)
{
}



void CParserView::line::node_operator_and::in_set(sset* ss)
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



void CParserView::line::node_operator_and::in_draw(sdraw* sd) const
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

	sd->pDC->MoveTo(left + w4, _middle + mh);
	sd->pDC->LineTo(mx, _middle - hw);
	sd->pDC->LineTo(right - w4, _middle + mh);

	sd->pDC->SelectObject(oldPen);
}


#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::node_result::node_result(bnode* parent, const tnode* nd)
	: node(parent)
{
	if (nd)
	{
		_ini = nd->getIni();
		_end = nd->getEnd();
	}
	else
		_end = _ini = -1;
}




void CParserView::line::node_result::set(sset* ss)
{
	if (_parent)
		set_rect_fromparent();
	else
	{
		top = ss->pline->top;
		bottom = ss->pline->bottom;

		left = right = ss->pline->left;
		_middle = top + Height() / 2;
	}
	CParserDoc* pDoc = ss->pline->_parent->GetDocument();
	if (!pDoc)
		return;

	const tnode* resnd = pDoc->getNodeResult();
	sset nss(ss->pline, nullptr, ss->pDC, pDoc->m_result.c_str(), false);

	_bNodes = ss->pline->_result == this;
	ss->pline->_result = this;

	if (!_bNodes)
	{
		CRect rl;
		if (_left)
		{
			rl = _left->rect();
			right = left = rl.right + 10;
			top = rl.top;
			bottom = rl.bottom;
			_middle = _left->_middle;
		}
		else
		{
			right = left += 10;
			rl.top = top;
			rl.bottom = bottom;
		}

		CFont* pFont = ss->pline->font(type(), ss->index);
		CFont* oldFont = ss->pDC->SelectObject(pFont);

		CSize te;
		GetTextExtentPointA(ss->pDC->m_hDC, " =", 2, &te);

		right += te.cx;
		if (rl.Height() != te.cy)
		{
			top = _middle - te.cy / 2 - te.cy % 2;
			bottom = top + te.cy;
		}
		ss->pDC->SelectObject(oldFont);
	}

	if (resnd)
	{
		_ini = _end = 0;

		nss.nd = resnd;
		node::new_instance(&_right, this, nss.nd);
		bool bEditing = nss.bEditing;
		nss.bEditing = false;
		_right->set(&nss);
		nss.bEditing = bEditing;
		CRect rr = _right->rect();
		max_rect(*this, rr);
	}
}



void CParserView::line::node_result::draw(sdraw* sd) const
{
	sd->begin_expr(this);
	COLORREF bkColor = sd->pDC->GetBkColor();
	if (sd->pline->_parent->m_bEditing)
		sd->pDC->FillSolidRect(this, RGB(0xD0, 0xD0, 0xD0));
	else
		sd->pDC->FillSolidRect(this, RGB(0xF8, 0xF8, 0xF8));

	int oMode = sd->pDC->SetBkMode(TRANSPARENT);

	CParserDoc* pDoc = sd->pline->_parent->GetDocument();
	if (pDoc)
	{
		sdraw nsd(sd->pline, this, sd->pDC, pDoc->m_result.c_str(), false);

		if (_left)
			_left->draw(sd);

		if (!_parent)
		{
			CPen pen;
			pen.CreatePen(PS_SOLID, 1, sd->pline->color(type()));
			CPen* oldPen = sd->pDC->SelectObject(&pen);

			LONG r = (_right ? _right->get_first_child()->left : right) - 2;
			sd->pDC->MoveTo(left + 2, _middle - 2);
			sd->pDC->LineTo(r - 2, _middle - 2);
			sd->pDC->MoveTo(left + 2, _middle + 2);
			sd->pDC->LineTo(r - 2, _middle + 2);
			sd->pDC->SelectObject(oldPen);
		}
		if (_right)
			_right->draw(&nsd);
	}
	else if (_right)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_right->draw(sd);
		sd->pnd = pnd;
	}
	sd->pDC->SetBkColor(bkColor);
	sd->pDC->SetBkMode(oMode);
	sd->end_expr(this);
}




inline bool CParserView::line::node_result::empty() const
{
	return false;
}



inline CParserView::line::bnodetypes CParserView::line::node_result::type() const
{
	return bndResult;
}




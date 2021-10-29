
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::node_operator_equal::node_operator_equal(bnode* parent, const tnode* nd)
	: node(parent, nd)
{
}




void CParserView::line::node_operator_equal::set(sset* ss)
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
		right = left + 10;
		top = rl.top;
		bottom = rl.bottom;
		_middle = _left->_middle;
	}
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
		bottom = top + te.cy;
		_middle = top + te.cy / 2;
	}
	LOGFONT lf;
	pFont->GetLogFont(&lf);
	if (lf.lfItalic)
		right += 2;

	if (rnd)
	{
		const tnode* nd = ss->nd;
		ss->nd = rnd;
		new_instance(&_right, this, rnd)->set(ss);
		ss->nd = nd;
	}
	else
	{
		CParserDoc* pDoc = ss->pline->_parent->GetDocument();
		if (pDoc && !pDoc->m_result.empty())
		{
			const tnode* nd = ss->nd;
			ss->nd = rnd;

			_right = ss->pline->_result = new node_result(this, nullptr);
			_right->set(ss);
			ss->nd = nd;
		}
	}
	check_error(ss);
}




void CParserView::line::node_operator_equal::draw(sdraw* sd) const
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
	sd->pDC->MoveTo(left + 1, _middle - 2);
	sd->pDC->LineTo(right - 1, _middle - 2);
	sd->pDC->MoveTo(left + 1, _middle + 2);
	sd->pDC->LineTo(right - 1, _middle + 2);
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



bool CParserView::line::node_operator_equal::set_caret_pos(sdraw* sd, scaret& caret) const
{
	bool bOk = false;
	if (_left)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		bOk = _left->set_caret_pos(sd, caret);
		sd->pnd = pnd;
	}
	if (!bOk && _ini <= caret.spos[1] && caret.spos[1] <= _end)
	{
		int cx;
		if (_ini < caret.spos[1])
			cx = Width();
		else
			cx = 0;
		caret.pos[1].x = left + cx;
		caret.pos[1].y = top;
		caret.height = Height();
		bOk = true;
	}

	if (!bOk && _right)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		bOk = _right->set_caret_pos(sd, caret);
		sd->pnd = pnd;
	}
	return bOk;
}



inline CParserView::line::bnodetypes CParserView::line::node_operator_equal::type() const
{
	return bndOprEqual;
}


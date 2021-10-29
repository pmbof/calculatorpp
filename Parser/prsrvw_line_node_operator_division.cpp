
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::node_operator_division::node_operator_division(bnode* parent, const tnode* nd)
	: node(parent, nd)
{
}




void CParserView::line::node_operator_division::set(sset* ss)
{
	const tnode* lnd = ss->nd->getLeft();
	const tnode* rnd = ss->nd->getRight();

	if (lnd && rnd && ss->pline->_parent->m_fstyle & PMB_STYLE_CPPEDIT_MULTILINE)
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

		const tnode* nd = ss->nd;
		if (lnd)
		{
			ss->nd = lnd;
			new_instance(&_left, this, lnd)->set(ss);
		}

		CRect rl = _left->rect();

		ss->nd = rnd;
		new_instance(&_right, this, rnd)->set(ss);
		ss->nd = nd;

		CRect rr = _right->rect();
		int width = rr.Width() < rl.Width() ? rl.Width() : rr.Width();
		right = left + width;
		top += _middle - 2;
		bottom = _middle + 4;
		_left->rect_move(width / 2 - rl.Width() / 2, _middle - rl.bottom - 2);
		_right->rect_move(width / 2 - rr.Width() / 2, _middle - rr.top + 2);
		check_error(ss);
	}
	else
		node::set(ss);
}




void CParserView::line::node_operator_division::draw(sdraw* sd) const
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
	if (_left && _right && sd->pline->_parent->m_fstyle & PMB_STYLE_CPPEDIT_MULTILINE)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_right->draw(sd);
		sd->pnd = pnd;
		CPen* oldPen = sd->pDC->SelectObject(&pen);
		sd->pDC->MoveTo(left + 1, _middle);
		sd->pDC->LineTo(right - 1, _middle);
		sd->pDC->SelectObject(oldPen);
	}
	else
	{
		CPen* oldPen = sd->pDC->SelectObject(&pen);
		sd->pDC->MoveTo(right - 1, top - 1);
		sd->pDC->LineTo(left + 1, bottom + 1);
		sd->pDC->SelectObject(oldPen);
		if (_right)
		{
			const bnode* pnd = sd->pnd;
			sd->pnd = this;
			_right->draw(sd);
			sd->pnd = pnd;
		}
	}
	sd->end_expr(this);
}



bool CParserView::line::node_operator_division::set_caret_pos(sdraw* sd, scaret& caret) const
{
	bool bOk = false;
	if (_left)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		bOk = _left->set_caret_pos(sd, caret);
		sd->pnd = pnd;
	}
	if (!bOk)
	{
		if (_left && _right && sd->pline->_parent->m_fstyle & PMB_STYLE_CPPEDIT_MULTILINE)
		{
			const bnode* pnd = sd->pnd;
			sd->pnd = this;
			bOk = _right->set_caret_pos(sd, caret);
			sd->pnd = pnd;
		}
		else
		{
			if (_right)
			{
				const bnode* pnd = sd->pnd;
				sd->pnd = this;
				bOk = _right->set_caret_pos(sd, caret);
				sd->pnd = pnd;
			}
		}
	}
	return bOk;
}



CParserView::line::bnodetypes CParserView::line::node_operator_division::type() const
{
	return bndOprDivision;
}


#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::node_parentheses::node_parentheses(const tnode* nd, bnode* parent)
	: node(nd, parent)
{
}




void CParserView::line::node_parentheses::set(sset* ss)
{
	assert(ss->tnd == _ptnd);

	const tnode* lnd = _ptnd->getLeft();
	const tnode* rnd = _ptnd->getRight();

	if (_parent)
		set_rect_fromparent();
	else
	{
		top = ss->pline->top;
		bottom = ss->pline->bottom;

		left = right = ss->pline->left;
		_middle = top + Height() / 2;
	}

	short np;
	get_np(ss, _parent, np, false);

	if (lnd)
	{
		ss->tnd = lnd;
		new_instance(&_left, this, lnd)->set(ss);
		ss->tnd = _ptnd;

		CRect rr = _left->rect();
		left = right = rr.right;
		top = rr.top;
		bottom = rr.bottom;
		_middle = _left->_middle;
	}

	CFont* pFont = ss->pline->font(type(), ss->index);
	CFont* oldFont = ss->pDC->SelectObject(pFont);

	CSize te;
	GetTextExtentPointA(ss->pDC->m_hDC, ss->pstr + get_ini(), np, &te);
	right = left + te.cx;

	if (!_right && !_left)
		bottom = top + te.cy;

	if (rnd)
	{
		ss->tnd = rnd;
		new_instance(&_right, this, rnd)->set(ss);
		ss->tnd = _ptnd;

		CRect rr = _right->rect();
		top = rr.top;
		bottom = rr.bottom;
		if (rr.left != right || _middle != _right->_middle)
			_right->rect_move(_right->left - right, _right->_middle - _middle);
		_middle = _right->_middle;
	}

	check_error(ss);
}





void CParserView::line::node_parentheses::draw(sdraw* sd) const
{
	sd->begin_expr(this);
	if (_left)
	{
		const bnode* pnd = sd->pnd;
		//sd->pnd = nullptr;
		_left->draw(sd);
		sd->pnd = pnd;
	}
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, sd->pline->color(type()));
	CPen* oldPen = sd->pDC->SelectObject(&pen);
	CRect r(this);
	r.top -= Height();
	r.bottom += Height();

	short np;
	if (get_np(sd, sd->pnd, np, true))
	{
		short nPrnths = nparentheses();
		if (!_left || !nPrnths)
		{
			for (short n = 0; n < np; ++n)
			{
				r.right = ++r.left + 3 * Height();
				sd->pDC->Arc(r, CPoint(right, top + 1), CPoint(right, bottom - 1));
				r.left = r.left + Width() / np;
			}
		}
		if (_left || !nPrnths)
		{
			for (short n = 0; n < np; ++n)
			{
				r.left = --r.right - 3 * Height();
				sd->pDC->Arc(r, CPoint(left, bottom - 1), CPoint(left, top + 1));
				r.right = r.right - Width() / np;
			}
		}
	}
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



bool CParserView::line::node_parentheses::set_caret_pos(sdraw* sd, scaret& caret) const
{
	bool bOk = false;
	if (_left)
	{
		const bnode* pnd = sd->pnd;
		//sd->pnd = nullptr;
		bOk = _left->set_caret_pos(sd, caret);
		sd->pnd = pnd;
	}

	if (!bOk && get_ini() <= caret.spos[1] && caret.spos[1] <= get_end())
	{
		CRect r(this);

		int cx = 0;
		if (caret.spos[1] == get_ini())
			bOk = true;
		else if (caret.spos[1] < get_end())
		{
			short nPrnths = nparentheses();
			if (nPrnths < 0)
				nPrnths *= -1;
			cx = Width() / nPrnths * (caret.spos[1] - get_ini());
			bOk = true;
		}
		else if (!_right && get_end() == caret.spos[1])
		{
			cx = Width();
			bOk = true;
		}

		if (bOk)
		{
			caret.pos[1].x = left + cx;
			caret.pos[1].y = r.top;
			caret.height = r.Height();
		}
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



bool CParserView::line::node_parentheses::get_np(sbase* sb, const bnode* pnd, short& np, bool bDraw) const
{
	short nPrnths = nparentheses();
	if (sb->bEditing || !pnd || pnd->type() != bndOprDivision && pnd->type() != bndOprRoot
		&& (pnd->type() != bndOprPower || pnd->is_left_parentheses(this))
		|| nPrnths != 0)
	{
		if (!sb->bEditing && pnd)
		{
			bnodetypes tp = pnd->type();
			np = tp == bndOprDivision || tp == bndOprRoot || tp == bndParentheses && pnd->nparentheses() < 0 || tp == bndOprPower && !pnd->is_left_parentheses(this)
				? nPrnths < 0 ? -nPrnths - 1 : nPrnths - 1 : nPrnths < 0 ? -nPrnths : nPrnths;
		}
		else
			np = nPrnths < 0 ? -nPrnths : nPrnths;

		if (!np)
		{
			if (sb->bEditing)
			{
				if (bDraw)
					for (short i = 0; sb->pstr[get_ini() + i] == '('; ++i, ++np)
						;
				else
					np = get_length();
			}
			else
				np = 0;
		}
		return true;
	}
	np = 0;
	return false;
}





inline bool CParserView::line::node_parentheses::parentheses() const
{
	return true;
}




inline short CParserView::line::node_parentheses::nparentheses() const
{
	return static_cast<const pmb::parser::nodes::parentheses<item, ndtype>*>(_ptnd)->getOpened();
}




inline CParserView::line::bnodetypes CParserView::line::node_parentheses::type() const
{
	return bndParentheses;
}


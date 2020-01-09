
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::node_parentheses::node_parentheses(bnode* parent, const tnode* nd)
	: node(parent, nd), _nparentheses(static_cast<const pmb::parser::nodes::parentheses<item, ndtype>*>(nd)->getOpened())
{
}




void CParserView::line::node_parentheses::set(sset* ss)
{
	const tnode* lnd = ss->nd->getLeft();
	const tnode* rnd = ss->nd->getRight();

	ss->parents.push_back(this);
	if (ss->pnd)
	{
		top = ss->pnd->top;
		bottom = ss->pnd->bottom;
		left = right = ss->pnd->right;
		_middle = ss->pnd->_middle;
	}
	else
	{
		top = ss->pline->top;
		bottom = ss->pline->bottom;

		left = right = ss->pline->left;
		_middle = top + Height() / 2;
	}

	CFont* pFont = ss->pline->font(type(), ss->index);
	CFont* oldFont = ss->pDC->SelectObject(pFont);
	CString sn(ss->pstr + _ini, ss->nd->len());

	if (lnd)
	{
		const tnode* nd = ss->nd;
		bnode* pnd = ss->pnd;
		ss->nd = lnd;
		ss->pnd = this;
		new_instance(&_left, this, lnd)->set(ss);
		ss->nd = nd;
		ss->pnd = pnd;

		CRect rr = _left->rec_rect();
		left = right = rr.left;
		top = rr.top;
		bottom = rr.bottom;
	}

	if (rnd)
	{
		const tnode* nd = ss->nd;
		bnode* pnd = ss->pnd;
		ss->nd = rnd;
		ss->pnd = this;
		new_instance(&_right, this, rnd)->set(ss);
		ss->nd = nd;
		ss->pnd = pnd;

		CRect rr = _right->rec_rect();
		left = right = rr.right;
		top = rr.top;
		bottom = rr.bottom;
	}

	CRect cr(this);
	ss->pDC->DrawText(sn, cr, DT_CALCRECT | DT_LEFT | DT_TOP | DT_SINGLELINE);
	right = left + cr.Width();
	LOGFONT lf;
	pFont->GetLogFont(&lf);
	if (lf.lfItalic)
		right += 2;
	if (lnd)
		_left->rec_move(right - left, 0);

	check_error(ss);
	ss->parents.pop_back();
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

	if (sd->bEditing || !sd->pnd || sd->pnd->type() != bndOprDivision && sd->pnd->type() != bndOprRoot
		&& (sd->pnd->type() != bndOprPower || sd->pnd->is_left_parentheses(this))
		|| 1 < _nparentheses || _nparentheses < -1)
	{
		short np = !sd->bEditing && sd->pnd && (sd->pnd->type() == bndOprDivision || sd->pnd->type() == bndOprRoot || sd->pnd->type() == bndOprPower && !sd->pnd->is_left_parentheses(this))
			? _nparentheses < 0 ? -_nparentheses - 1 : _nparentheses - 1 : _nparentheses < 0 ? -_nparentheses : _nparentheses;
		if (_left)
		{
			for (short n = 0; n < np; ++n)
			{
				r.right = ++r.left + 3 * Height();
				sd->pDC->Arc(r, CPoint(right, top + 1), CPoint(right, bottom - 1));
				r.left = r.left + Width() / np;
			}
		}
		else
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




bool CParserView::line::node_parentheses::parentheses() const
{
	return true;
}




short CParserView::line::node_parentheses::nparentheses() const
{
	return _nparentheses;
}




CParserView::line::bnodetypes CParserView::line::node_parentheses::type() const
{
	return bndParentheses;
}


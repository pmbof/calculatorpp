
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::node_alpha::node_alpha(bnode* parent, const tnode* nd)
	: node(parent, nd)
{
}



void CParserView::line::node_alpha::set(sset* ss)
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

	if (lnd)
	{
		ASSERT(false); // because doble left in  a.0
		const tnode* nd = ss->nd;
		ss->nd = lnd;
		ss->pnd = this;
		new_instance(&_left, this, lnd)->set(ss);
		ss->nd = nd;
	}

	CRect rl;
	if (lnd)
	{
		ASSERT(false); // because doble left in  a.0
		rl = _left->rect();
		left = rl.right;
		right = left;
		top = rl.top;
		bottom = rl.bottom;
		_middle = _left->_middle;
	}
	if (_ini < _end)
	{
		CFont* pFont = ss->pline->font(type(), ss->index);
		CFont* oldFont = ss->pDC->SelectObject(pFont);
		CString sn(ss->pstr + _ini, _end - _ini);

		int pt = sn.Find(L".");
		if (0 < pt && pt + 1 < sn.GetLength())
			sn = sn.Mid(0, pt);
		else
			pt = -1;

		CRect cr(this);
		ss->pDC->DrawText(sn, cr, DT_CALCRECT | DT_LEFT | DT_TOP | DT_SINGLELINE);
		right = left + cr.Width();
		if (lnd && rl.Height() != cr.Height())
		{
			top = _middle - cr.Height() / 2 - cr.Height() % 2;
			bottom = top + cr.Height();
		}
		else if (!lnd)
		{
			top = _middle - cr.Height() / 2;
			bottom = top + cr.Height();
		}
		LOGFONT lf;
		pFont->GetLogFont(&lf);
		if (lf.lfItalic)
			right += 2;
		ss->pDC->SelectObject(oldFont);
		if (0 < pt)
		{
			_right = new node_alpha(this, ss->nd);;
			static_cast<node_alpha*>(_right)->_ini += pt + (ss->bEditing ? 0 : 1);

			ss->pnd = this;
			ss->index += 2;
			_right->set(ss);
			ss->index -= 2;
			_right->rect_move(lf.lfItalic ? -2 : 0, Height() / 2);
			rnd = nullptr;
		}
	}
	if (rnd)
	{
		const tnode* nd = ss->nd;
		ss->nd = rnd;
		ss->pnd = this;
		bnode* lright;
		for (lright = this; static_cast<node_alpha*>(lright)->bnode::_right; lright = static_cast<node_alpha*>(lright)->bnode::_right)
			;
		new_instance(&static_cast<node_alpha*>(lright)->bnode::_right, this, rnd)->set(ss);
		ss->nd = nd;
	}
	check_error(ss);
	ss->parents.pop_back();
}



void CParserView::line::node_alpha::draw(sdraw* sd) const
{
	sd->begin_expr(this);
	if (_left)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_left->draw(sd);
		sd->pnd = pnd;
	}
	if (_ini < _end)
	{
		COLORREF oldColor = sd->pDC->SetTextColor(sd->pline->color(type()));
		CFont* oldFont = sd->pDC->SelectObject(sd->pline->font(type(), sd->index));
		CString sn(sd->pstr + _ini, _end - _ini);

		sd->pDC->DrawText(sn, const_cast<CRect*>(static_cast<const CRect*>(this)), DT_LEFT | DT_TOP | DT_SINGLELINE);
		sd->pDC->SetTextColor(oldColor);
		sd->pDC->SelectObject(oldFont);
	}
	if (_right)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		sd->index += 2;
		_right->draw(sd);
		sd->index -= 2;
		sd->pnd = pnd;
	}
	sd->end_expr(this);
}



CParserView::line::bnodetypes CParserView::line::node_alpha::type() const
{
	return bndAlpha;
}


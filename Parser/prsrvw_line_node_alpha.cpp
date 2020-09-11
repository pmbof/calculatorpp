
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
		ASSERT(false); // because doble left in  a.0
		const tnode* nd = ss->nd;
		ss->nd = lnd;
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

		item::SIZETP end;
		for (end = _ini + 1; end < _end && ss->pstr[end] != '.'; ++end)
			;

/*		CSize te = ss->pDC->GetOutputTextExtent(sn);
		ABCFLOAT vectA[256];
		ss->pDC->GetCharABCWidths(0, 255, vectA);
		float widthA = 0;
		for (int k = 0; k < sn.GetLength(); ++k)
			widthA += (0 < vectA[sn[k]].abcfA ? vectA[sn[k]].abcfA : 0) + vectA[sn[k]].abcfB + vectA[sn[k]].abcfC;*/
		CSize te;
		GetTextExtentPointA(ss->pDC->m_hDC, ss->pstr + _ini, end - _ini, &te);

		right = left + te.cx + 1;
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
		if (end < _end)
		{
			_right = new node_alpha(this, ss->nd);;
			static_cast<node_alpha*>(_right)->_ini = end + (ss->bEditing ? 0 : 1);
			_end = end;

			ss->index += 2;
			_right->set(ss);
			ss->index -= 2;
			_right->rect_move(0, Height() / 2);
			rnd = nullptr;
		}
	}
	if (rnd)
	{
		const tnode* nd = ss->nd;
		ss->nd = rnd;
		bnode* lright;
		for (lright = this; static_cast<node_alpha*>(lright)->bnode::_right; lright = static_cast<node_alpha*>(lright)->bnode::_right)
			;
		new_instance(&static_cast<node_alpha*>(lright)->bnode::_right, this, rnd)->set(ss);
		ss->nd = nd;
	}
	check_error(ss);
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



inline CParserView::line::bnodetypes CParserView::line::node_alpha::type() const
{
	return bndAlpha;
}



#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::node_operator_root::node_operator_root(const tnode* nd, bnode* parent)
	: node(nd, parent)
{
}




void CParserView::line::node_operator_root::set(sset* ss)
{
	assert(ss->tnd == _ptnd);

	const tnode* lnd = _ptnd->getLeft();
	const tnode* rnd = _ptnd->getRight();

	if (rnd)
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

		CRect rl;
		if (lnd)
		{
			ss->tnd = lnd;
			new_instance(&_left, this, lnd);
			++ss->index;
			_left->set(ss);
			--ss->index;
			rl = _left->rect();
			left = rl.right;
			right = left + 2;
			top = rl.top - 1;
			bottom = rl.bottom;
		}
		{
			CFont* pFont = ss->pline->font(type(), ss->index);
			CFont* oldFont = ss->pDC->SelectObject(pFont);

			CSize te;
			GetTextExtentPointA(ss->pDC->m_hDC, ss->pstr + get_ini(), get_length(), &te);

			right = left + te.cx + 1;
			bottom = top + te.cy;
			ss->pDC->SelectObject(oldFont);
		}
		ss->tnd = rnd;
		new_instance(&_right, this, rnd)->set(ss);
		ss->tnd = _ptnd;
		CRect rr = _right->rect();
		bool bdouble = 3 * Height() < 2 * rr.Height();
		top = rr.top - 2;
		bottom = rr.bottom;
		rright = rr.right;
		if (bdouble)
		{
			const int dy = Width();
			rright += dy;
			right += dy;
			_right->rect_move(dy, 0);
		}
		if (_left)
		{
			if (2 * rl.Height() < 3 * (rr.Height() - rl.Height()) * (rr.Height() < rl.Height() ? -1 : 1))
				_left->rect_move(0, rr.top - rl.top);
			else
				_left->rect_move(0, rr.top - rl.top - rl.Height() / 2);
		}
		check_error(ss);
	}
	else
		node::set(ss);
}




void CParserView::line::node_operator_root::draw(sdraw* sd) const
{
	sd->begin_expr(this);
	if (_left)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		++sd->index;
		_left->draw(sd);
		--sd->index;
		sd->pnd = pnd;
	}
	if (!_right)
	{
		COLORREF oldColor = sd->pDC->SetTextColor(sd->pline->color(type()));
		CFont* pFont = sd->pline->font(type(), sd->index);
		CFont* oldFont = sd->pDC->SelectObject(pFont);
		CString sn(sd->pstr + get_ini(), get_length());

		sd->pDC->DrawText(sn, const_cast<CRect*>(static_cast<const CRect*>(this)), DT_LEFT | DT_TOP | DT_SINGLELINE);
		sd->pDC->SelectObject(oldFont);
		sd->pDC->SetTextColor(oldColor);
	}
	if (_right)
	{
		CPen pen;
		pen.CreatePen(PS_SOLID, 1, sd->pline->color(type()));
		CPen* oldPen = sd->pDC->SelectObject(&pen);

		int t;
		if (_left)
		{
			CRect rl = _left->rect();
			t = rl.bottom;
			sd->pDC->MoveTo(rl.left + rl.Width() / 2, t + 1);
		}
		else
		{
			t = top + Height() / 3;
			sd->pDC->MoveTo(left - 1, t + 1);
		}
		sd->pDC->LineTo(left + 1, t);
		sd->pDC->LineTo(left + 1 + Width() / 2, bottom);
		sd->pDC->LineTo(right - 1, top + 1);

		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_right->draw(sd);
		sd->pnd = pnd;
		sd->pDC->MoveTo(right - 1, top + 1);
		sd->pDC->LineTo(rright - 4, top + 1);
		sd->pDC->LineTo(rright - 1, top + 6);
		sd->pDC->SelectObject(oldPen);
	}
	sd->end_expr(this);
}



bool CParserView::line::node_operator_root::set_caret_pos(sdraw* sd, scaret& caret) const
{
	bool bOk = false;
	if (_left)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		++sd->index;
		bOk = _left->set_caret_pos(sd, caret);
		--sd->index;
		sd->pnd = pnd;
	}
	if (!bOk && !_right && get_ini() <= caret.spos[1] && caret.spos[1] <= get_end())
	{
		int cx;
		if (get_ini() < caret.spos[1])
		{
			CFont* pFont = sd->pline->font(type(), sd->index);
			CFont* oldFont = sd->pDC->SelectObject(pFont);
			CString sn(sd->pstr + get_ini(), caret.spos[1] - get_ini());

			cx = sd->pDC->GetTextExtent(sn).cx;
			sd->pDC->SelectObject(oldFont);
		}
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



inline CParserView::line::bnodetypes CParserView::line::node_operator_root::type() const
{
	return bndOprRoot;
}


#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::node_operator_power::node_operator_power(const tnode* nd, bnode* parent)
	: node(nd, parent)
{
}




void CParserView::line::node_operator_power::set(sset* ss)
{
	assert(ss->tnd == _ptnd);

	const tnode* lnd = _ptnd->getLeft();
	const tnode* rnd = _ptnd->getRight();

	if (lnd && rnd)
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

		ss->tnd = lnd;
		new_instance(&_left, this, lnd)->set(ss);
		CRect rl = _left->rect();
		left = rl.right;
		right = left;
		top = rl.top;
		bottom = rl.bottom;

		CFont* pFont = ss->pline->font(type(), ss->index);
		CFont* oldFont = ss->pDC->SelectObject(pFont);
		CSize te;
		if (false && ss->bEditing)
		{
			GetTextExtentPointA(ss->pDC->m_hDC, ss->pstr + get_ini(), get_length(), &te);
			right = left + te.cx;
			bottom = top + te.cy;
			ss->pDC->SelectObject(oldFont);
		}
		else
		{
			GetTextExtentPointA(ss->pDC->m_hDC, "2", 1, &te);
		}
		_middle = _left->_middle;
		ss->tnd = rnd;
		new_instance(&_right, this, rnd);
		++ss->index;
		_right->set(ss);
		--ss->index;
		CRect rr = _right->rect();
		_right->rect_move(0, rl.top - rr.top - rr.Height() + te.cy / 3);// +_right->_middle + _right->_middle / 5);// -rr.top + height2 / 2 - (1 + height2 % 2));
		//if (_left->_middle < _right->_middle + 10)
		//	_right->rect_move(0, rl.top - _right->_middle);
		//if (_left->_middle < _right->bottom)
		//	_right->rect_move(0, _left->_middle - _right->bottom);
		ss->tnd = _ptnd;
		check_error(ss);
	}
	else
		node::set(ss);
}




void CParserView::line::node_operator_power::draw(sdraw* sd) const
{
	sd->begin_expr(this);
	if (_left)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_left->draw(sd);
		sd->pnd = pnd;
	}
	if (sd->bEditing || !_right)
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
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		++sd->index;
		_right->draw(sd);
		--sd->index;
		sd->pnd = pnd;
	}
	sd->end_expr(this);
}



bool CParserView::line::node_operator_power::set_caret_pos(sdraw* sd, scaret& caret) const
{
	bool bOk = false;
	if (_left)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		bOk = _left->set_caret_pos(sd, caret);
		sd->pnd = pnd;
	}
	if (!bOk && (sd->bEditing || !_right) && get_ini() <= caret.spos[1] && caret.spos[1] <= get_end())
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
		++sd->index;
		bOk = _right->set_caret_pos(sd, caret);
		--sd->index;
		sd->pnd = pnd;
	}
	return bOk;
}



inline CParserView::line::bnodetypes CParserView::line::node_operator_power::type() const
{
	return bndOprPower;
}


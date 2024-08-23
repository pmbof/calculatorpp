
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::node_operator_result::node_operator_result(const tnode* nd, bnode* parent)
	: node(nd, parent), _ini(nd->getIni()), _end(nd->getEnd())
{
}




void CParserView::line::node_operator_result::set(sset* ss)
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

	if (lnd)
	{
		ss->tnd = lnd;
		new_instance(&_left, this, lnd)->set(ss);
		ss->tnd = _ptnd;
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
	CParserDoc* pDoc = ss->pline->_parent->GetDocument();

	CFont* pFont = ss->pline->font(type(), ss->index);
	CFont* oldFont = ss->pDC->SelectObject(pFont);

	CSize te;
	GetTextExtentPointA(ss->pDC->m_hDC, ss->pstr + _ini, _end - _ini - (rnd && pDoc && !pDoc->m_result.empty() ? 1 : 0), &te);

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

	if (rnd)
	{
		ss->tnd = rnd;
		if (pDoc && !pDoc->m_result.empty())
		{
			// split operator =. to = \ .
			node_operator_result* nrnr;
			_right = nrnr = new node_operator_result(rnd->getParent(), this);
			++nrnr->_ini;
			--_end;
			nrnr->set_rect_fromparent();

			nrnr->_left = ss->pline->_result = new node_result(nullptr, nrnr);
			nrnr->_left->set(ss);

			CRect rect = nrnr->_left->rect();
			nrnr->_rect_move(rect.Width() + 1, 0);
			nrnr->right = nrnr->left + 8;

			ss->pline->_ndres = new_instance(&nrnr->_right, nrnr, rnd);
			ss->pline->_ndres->set(ss);
		}
		else
			new_instance(&_right, this, rnd)->set(ss);
		ss->tnd = _ptnd;
	}
	else if (pDoc && !pDoc->m_result.empty())
	{
		ss->tnd = rnd;

		_right = ss->pline->_result = new node_result(rnd, this); // old call: (null, this);
		_right->set(ss);
		ss->tnd = _ptnd;
	}
	check_error(ss);
}




void CParserView::line::node_operator_result::draw(sdraw* sd) const
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
	if (sd->pstr[_ini] == '=')
	{
		sd->pDC->MoveTo(left + 2, _middle - 2);
		sd->pDC->LineTo(right - 2, _middle - 2);
		sd->pDC->MoveTo(left + 2, _middle + 2);
		sd->pDC->LineTo(right - 2, _middle + 2);
	}
	else // dot
		sd->pDC->Ellipse(left + Width() / 2 - 1, _middle - 1, left + Width() / 2 + 1, _middle + 1);
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



bool CParserView::line::node_operator_result::set_caret_pos(sdraw* sd, scaret& caret) const
{
	bool bOk = false;
	if (_left)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		bOk = _left->set_caret_pos(sd, caret);
		sd->pnd = pnd;
	}
	if (!bOk && get_ini() <= caret.spos[1] && caret.spos[1] <= get_end())
	{
		int cx;
		if (get_ini() < caret.spos[1])
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



inline CParserView::line::bnodetypes CParserView::line::node_operator_result::type() const
{
	return bndOprResult;
}


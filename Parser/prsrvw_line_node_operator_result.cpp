
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::node_operator_result::node_operator_result(bnode* parent, const tnode* nd)
	: node(parent, nd)
{
}




void CParserView::line::node_operator_result::set(sset* ss)
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
	CParserDoc* pDoc = ss->pline->_parent->GetDocument();

	CFont* pFont = ss->pline->font(type(), ss->index);
	CFont* oldFont = ss->pDC->SelectObject(pFont);

	CString sn(ss->pstr + _ini, ss->nd->len() - (rnd && pDoc && !pDoc->m_result.empty() ? 1 : 0));

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
		bottom = top + cr.Height();
		_middle = top + cr.Height() / 2;
	}
	LOGFONT lf;
	pFont->GetLogFont(&lf);
	if (lf.lfItalic)
		right += 2;

	if (rnd)
	{
		const tnode* nd = ss->nd;
		ss->nd = rnd;
		if (pDoc && !pDoc->m_result.empty())
		{
			// split operator =. to = \ .
			node_operator_result* nrnr;
			_right = nrnr = new node_operator_result(this, rnd->getParent());
			++nrnr->_ini;
			--_end;
			nrnr->set_rect_fromparent();

			nrnr->_left = ss->pline->_result = new node_result(nrnr, nullptr);
			nrnr->_left->set(ss);

			CRect rect = nrnr->_left->rect();
			nrnr->_rect_move(rect.Width() + 1, 0);
			nrnr->right = nrnr->left + 8;

			ss->pline->_ndres = new_instance(&nrnr->_right, nrnr, rnd);
			ss->pline->_ndres->set(ss);
		}
		else
			new_instance(&_right, this, rnd)->set(ss);
		ss->nd = nd;
	}
	else if (pDoc && !pDoc->m_result.empty())
	{
		const tnode* nd = ss->nd;
		ss->nd = rnd;

		_right = ss->pline->_result = new node_result(this, nullptr);
		_right->set(ss);
		ss->nd = nd;
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




CParserView::line::bnodetypes CParserView::line::node_operator_result::type() const
{
	return bndOprResult;
}


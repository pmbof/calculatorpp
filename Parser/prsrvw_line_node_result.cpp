
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::node_result::node_result(const tnode* nd)
	: node()
{
	if (nd)
	{
		_ini = nd->getIni();
		_end = nd->getEnd();
	}
	else
		_end = _ini = 0;
}




void CParserView::line::node_result::set(sset* ss)
{
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
	CParserDoc* pDoc = ss->pline->_parent->GetDocument();
	if (!pDoc)
		return;

	const tnode* resnd = pDoc->getNodeResult();
	sset nss = { ss->pline, nullptr, this, ss->pDC, pDoc->m_result.c_str(), false };

	_bNodes = ss->pline->_result == this;
	ss->pline->_result = this;

	if (!_bNodes)
	{
		right = left += 10;
		CFont* pFont = ss->pline->font(type(), ss->index);
		CFont* oldFont = ss->pDC->SelectObject(pFont);
		CRect cr(this);
		ss->pDC->DrawText(L" = ", cr, DT_CALCRECT | DT_LEFT | DT_TOP | DT_SINGLELINE);
		right += cr.Width();
		if (Height() != cr.Height())
		{
			top = _middle - cr.Height() / 2 - cr.Height() % 2;
			bottom = top + cr.Height();
		}
		ss->pDC->SelectObject(oldFont);
	}

	if (resnd)
	{
		_ini = _end = 0;

		nss.nd = resnd;
		_right = node::new_instance(nss.nd);
		_right->set(&nss);
		CRect rr = _right->rec_rect();
		max_rect(*this, rr);
	}
}



void CParserView::line::node_result::draw(sdraw* sd) const
{
	sd->begin_expr(this);
	COLORREF bkColor = sd->pDC->GetBkColor();
	if (sd->pline->_parent->m_bEditing)
		sd->pDC->FillSolidRect(this, RGB(0xD0, 0xD0, 0xD0));
	else
		sd->pDC->FillSolidRect(this, RGB(0xF8, 0xF8, 0xF8));

	int oMode = sd->pDC->SetBkMode(TRANSPARENT);

	CParserDoc* pDoc = sd->pline->_parent->GetDocument();
	if (pDoc)
	{
		sdraw nsd = { sd->pline, this, sd->pDC, pDoc->m_result.c_str(), false };

		if (_left)
			_left->draw(&nsd);

		CString sn;
		if (!_bNodes)
			sn = L" = ";
		if (_ini < _end)
			sn += CString(nsd.pstr + _ini, _end - _ini);
		if (!sn.IsEmpty())
		{
			COLORREF oldColor = sd->pDC->SetTextColor(sd->pline->color(type()));
			CFont* pFont = sd->pline->font(type(), sd->index);
			CFont* oldFont = sd->pDC->SelectObject(pFont);
			CRect r(this);
			if (_left)
			{
				r.left = _left->right;
				r.top = _left->_middle - Height() / 2;
				r.bottom = r.top + Height();
			}
			if (_right)
			{
				r.right = _right->left;
				if (!_left)
				{
					r.top = _right->_middle - Height() / 2;
					r.bottom = r.top + Height();
				}
			}
			sd->pDC->DrawText(sn, r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			sd->pDC->SelectObject(oldFont);
			sd->pDC->SetTextColor(oldColor);
			sd->pDC->SetBkColor(bkColor);
		}
		if (_right)
			_right->draw(&nsd);
	}
	else if (_right)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_right->draw(sd);
		sd->pnd = pnd;
	}
	sd->pDC->SetBkMode(oMode);
	sd->end_expr(this);
}




bool CParserView::line::node_result::empty() const
{
	return false;
}



CParserView::line::bnodetypes CParserView::line::node_result::type() const
{
	return bndResult;
}




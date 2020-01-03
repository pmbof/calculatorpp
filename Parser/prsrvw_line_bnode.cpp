
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::bnode::bnode()
	: _left(nullptr), _right(nullptr)
{
}



CParserView::line::bnode::~bnode()
{
	if (_left)
		delete _left;
	if (_right)
		delete _right;
}



void CParserView::line::bnode::clear()
{
	if (_left)
	{
		delete _left;
		_left = nullptr;
	}
	if (_right)
	{
		delete _right;
		_right = nullptr;
	}
}



bool CParserView::line::bnode::is_left_parentheses(const bnode* parent) const
{
	return _left == parent || _left && _left->_left == parent;
}



CParserView::line::bnode& CParserView::line::bnode::operator=(const CRect& right)
{
	*static_cast<CRect*>(this) = right;
	return *this;
}



CParserView::line::bnode* CParserView::line::bnode::node_mright()
{
	bnode* rnd;
	for (rnd = this; rnd && rnd->_right; rnd = rnd->_right)
		;
	return rnd;
}


void CParserView::line::bnode::rec_move(int dx, int dy)
{
	_middle += dy;
	top += dy;
	left += dx;
	bottom += dy;
	right += dx;
	if (_left)
		_left->rec_move(dx, dy);
	if (_right)
		_right->rec_move(dx, dy);
}



CRect CParserView::line::bnode::rec_rect() const
{
	if (!_left && !_right)
		return *this;
	CRect cr;
	if (_left)
	{
		cr = _left->rec_rect();
		max_rect(cr, *this);
	}
	else
		cr = *this;
	if (_right)
	{
		CRect rr = _right->rec_rect();
		max_rect(cr, rr);
	}
	return cr;
}



void CParserView::line::bnode::max_rect(CRect& r, const CRect& r2) const
{
	if (r2.top < r.top)
		r.top = r2.top;
	if (r.bottom < r2.bottom)
		r.bottom = r2.bottom;
	if (r2.left < r.left)
		r.left = r2.left;
	if (r.right < r2.right)
		r.right = r2.right;
}



void CParserView::line::bnode::check_error(sset* ss)
{
	ss->pline->check_error(ss->nd, this);
}



void CParserView::line::bnode::end(sset* ss)
{
	CParserDoc* pDoc = ss->pline->_parent->GetDocument();
	if (!ss->pline->_result && pDoc && ss->bEditing && !pDoc->m_result.empty())
	{
		ss->nd = nullptr;
		bnode* nd;
		for (nd = this; nd->_right; nd = nd->_right)
			;
		ss->pnd = nd;

		nd->_right = new node_result(nullptr);
		nd->_right->set(ss);
		nd->_right->rec_move(0, ss->pline->_root->_middle - nd->_right->_middle);
	}
}



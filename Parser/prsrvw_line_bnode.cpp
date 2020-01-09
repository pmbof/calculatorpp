
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::bnode::bnode(bnode* parent)
	: _parent(parent), _left(nullptr), _right(nullptr)
{
}



CParserView::line::bnode::~bnode()
{
}




void CParserView::line::bnode::debug_check() const
{
	pmb::log* lg = pmb::log::instance();
	lg->trace(pmb::logInf, "bnode this    = 0x%08X\n", this);
	lg->trace(pmb::logInf, "bnode _parent = 0x%08X\n", _parent);
	lg->trace(pmb::logInf, "bnode _left   = 0x%08X\n", _left);
	lg->trace(pmb::logInf, "bnode _right  = 0x%08X\n", _right);
	if (_left)
	{
		if (_left->_parent != this)
			lg->trace(pmb::logError, "   - bnode _left->_parent (0x%08X) != this (0x%08X)\n", _left->_parent, this);
		ASSERT(_left->_parent == this);
	}
	if (_right)
	{
		if (_right->_parent != this)
			lg->trace(pmb::logError, "   - bnode _right->_parent (0x%08X) != this (0x%08X)\n", _right->_parent, this);
		ASSERT(_right->_parent == this);
	}
	if (_parent)
	{
		if (_parent->_left != this && _parent->_right != this)
			lg->trace(pmb::logError, "   - bnode none _parent->_left (0x%08X), _parent->_right (0x%08X) != this (0x%08X)\n", _parent->_left, _parent->_right, this);
		if (_parent->_left == _parent->_right)
			lg->trace(pmb::logError, "   - bnode none _parent->_left (0x%08X) == _parent->_right (0x%08X)\n", _parent->_left, _parent->_right, this);
		ASSERT((_parent->_left == this || _parent->_right == this) && _parent->_left != _parent->_right);
	}
}



void CParserView::line::bnode::debug_check_all() const
{
	const bnode* nd = get_root();
	for (nd = nd->get_first_left(); nd; nd = nd->get_next())
		nd->debug_check();
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

		nd->_right = new node_result(this, nullptr);
		nd->_right->set(ss);
		nd->_right->rec_move(0, ss->pline->_root->_middle - nd->_right->_middle);
	}
}






const CParserView::line::bnode* CParserView::line::bnode::get_first() const
{
	return const_cast<bnode*>(this)->get_first();
}

const CParserView::line::bnode* CParserView::line::bnode::get_root() const
{
	return const_cast<bnode*>(this)->get_root();
}

const CParserView::line::bnode* CParserView::line::bnode::get_first_left() const
{
	return const_cast<bnode*>(this)->get_first_left();
}

const CParserView::line::bnode* CParserView::line::bnode::get_next() const
{
	return const_cast<bnode*>(this)->get_next();
}


CParserView::line::bnode* CParserView::line::bnode::get_first()
{
	return get_root()->get_first_left();
}

CParserView::line::bnode* CParserView::line::bnode::get_root()
{
	bnode* ndr;
	for (ndr = this; ndr && ndr->_parent; ndr = ndr->_parent)
		;
	return ndr;
}


CParserView::line::bnode* CParserView::line::bnode::get_first_left()
{
	bnode* ndl = this;
	do
	{
		for ( ; ndl && ndl->_left; ndl = ndl->_left)
			;
		if (ndl && ndl->_right)
			ndl = ndl->_right;
		else
			break;
	} while (ndl);
	return ndl;
}


CParserView::line::bnode* CParserView::line::bnode::get_next()
{
	bnode* ndl = _parent;
	if (ndl && ndl->_right != this)
	{
		if (ndl->_right)
			ndl = ndl->_right->get_first_left();
	}
	return ndl;
}



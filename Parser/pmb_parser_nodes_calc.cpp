#include "StdAfx.h"
#include "pmb_parser_nodes_calc.h"


namespace pmb
{
namespace parser
{
namespace nodes
{



template<class _ITEM, class _NDTYPE>
calc<_ITEM, _NDTYPE>::calc(typename cnode::cNdType type, ISIZE ini, ISIZE end)
	: node<_ITEM, _NDTYPE>(type, ini, end)
{
}


template<class _ITEM, class _NDTYPE>
calc<_ITEM, _NDTYPE>::~calc()
{
}




template<class _ITEM, class _NDTYPE>
const calc<_ITEM, _NDTYPE>* calc<_ITEM, _NDTYPE>::firstCalc() const
{
	const calc<_ITEM, _NDTYPE>* c,
		*nc;
	for (c = this, nc = this; c && nc; )
	{
		nc = nullptr;
		if (c->_type == ndUnknown)
		{
			const unknown<_ITEM, _NDTYPE>* uk = static_cast<const unknown<_ITEM, _NDTYPE>*>(c);
			if (!uk->isValid() || uk->isFirstLeft())
			{
				if (uk->_left && uk->_left->isCalcType())
					nc = static_cast<calc<_ITEM, _NDTYPE>*>(uk->_left);
				else if (uk->_right && uk->_right->isCalcType())
					nc = static_cast<calc<_ITEM, _NDTYPE>*>(uk->_right);
			}
			else
			{
				if (uk->_right && uk->_right->isCalcType())
					nc = static_cast<calc<_ITEM, _NDTYPE>*>(uk->_right);
				else if (uk->_left && uk->_left->isCalcType())
					nc = static_cast<calc<_ITEM, _NDTYPE>*>(uk->_left);
			}
		}
		else if (c->isCalcType())
		{
			if (c->_left && c->_left->isCalcType())
				nc = static_cast<calc<_ITEM, _NDTYPE>*>(c->_left);
			else if (c->_right && c->_right->isCalcType())
				nc = static_cast<calc<_ITEM, _NDTYPE>*>(c->_right);
		}
		if (nc)
			c = nc;
	}
	return !nc ? c : nc;
}




template<class _ITEM, class _NDTYPE>
const calc<_ITEM, _NDTYPE>* calc<_ITEM, _NDTYPE>::nextCalc(bool& stopedIteration) const
{
	typedef const calc<_ITEM, _NDTYPE>* const_calc;

	const_calc nc;

	const_calc parent = static_cast<const_calc>(_parent);

	if (_parent && _parent->isCalcType())
	{
		if (parent->getType() == ndUnknown)
		{
			typedef const unknown<_ITEM, _NDTYPE>* const_unknown;

			bool bStopedItr = false;
			const_unknown parent_uk = static_cast<const_unknown>(parent);
			if (parent_uk->isValid())
			{
				bool chkOpr = parent_uk->checkOperator();
				if (parent_uk->isFirstLeft() && (parent_uk->_left == this && !chkOpr || stopedIteration))
					nc = static_cast<const_calc>(parent->_right && parent->_right->isCalcType() ? parent->_right : parent);
				else if (parent_uk->isFirstRight() && (parent_uk->_right == this && !chkOpr || stopedIteration))
					nc = static_cast<const_calc>(parent->_left && parent->_left->isCalcType() ? parent->_left : parent);
				else
				{
					nc = parent;
					bStopedItr = chkOpr && (parent_uk->isFirstLeft() && parent_uk->_left == this || parent_uk->isFirstRight() && parent_uk->_right == this);
				}
			}
			else
				nc = parent;

			stopedIteration = bStopedItr;
		}
		else if (parent->getType() == ndList)
		{
			if (parent->_left == this)
				nc = static_cast<const_calc>(parent->_right && parent->_right->isCalcType() ? parent->_right : parent);
			else
				nc = parent;
		}
		else
			nc = parent;
	}
	else
		nc = nullptr;
	
	return nc == _parent || !nc ? nc : nc->firstCalc();
}




}
}
}
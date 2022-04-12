#pragma once
#include "pmb_parser_iterator.h"


namespace pmb
{
namespace parser
{
namespace iterators
{


template <class _IT, class _baseIterator, class _ITEM, typename _NDTYPE>
struct list: public _baseIterator
{
	list(_IT& it): s(it) { }


	node<_ITEM, _NDTYPE>* operator()()
	{
		return s() == ',' ? node<_ITEM, _NDTYPE>::newNodeList((typename _IT::idx)++s) : nullptr;
	}


protected:
	_IT& s;
};


}
}
}
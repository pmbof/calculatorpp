#pragma once
#include "pmb_parser_iterator.h"


namespace pmb
{
namespace parser
{
namespace iterators
{


template <class _IT, class _baseIterator, class _TVALUE>
struct list: public _baseIterator
{
	list(_IT& it): s(it) { }


	node<_TVALUE>* operator()()
	{
		return s() == ',' ? node<_TVALUE>::newNodeList(++s): NULL;
	}


protected:
	_IT& s;
};


}
}
}
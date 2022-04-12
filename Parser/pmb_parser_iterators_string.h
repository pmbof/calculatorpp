#pragma once
#include "pmb_parser_iterator.h"

namespace pmb
{
namespace parser
{
namespace iterators
{



template <class _IT, char _scape, class _baseIterator, class _ITEM, typename _NDTYPE>
struct string: public _baseIterator
{
	string(_IT& it): s(it) { }


	node<_ITEM, _NDTYPE>* operator()()
	{
		typename _IT::idx a = s._i;
		if(s() == '"')
		{
			bool bScape = false;
			for(++s ; s() != '"' && !bScape; ++s)
				bScape = s() == _scape && !bScape;
		}
		return a < s ? node<_ITEM, _NDTYPE>::newNodeString(a, s._i) : nullptr;
	}


protected:
	_IT& s;
};


}
}
}
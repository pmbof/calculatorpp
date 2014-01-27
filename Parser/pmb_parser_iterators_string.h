#pragma once
#include "pmb_parser_iterator.h"

namespace pmb
{
namespace parser
{
namespace iterators
{



template <class _IT, char _scape, class _baseIterator, class _TVALUE>
struct string: public _baseIterator
{
	string(_IT& it): s(it) { }


	node<_TVALUE>* operator()()
	{
		int a = s;
		if(s() == '"')
		{
			bool bScape = false;
			for(++s ; s() != '"' && !bScape; ++s)
				bScape = s() == _scape && !bScape;
		}
		return a < s ? node<_TVALUE>::newNodeString(a, s): NULL;
	}


protected:
	_IT& s;
};


}
}
}
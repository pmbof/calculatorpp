#pragma once
#include "pmb_parser_iterator.h"

namespace pmb
{
namespace parser
{
namespace iterators
{



template <class _IT, class _baseIterator, class _TVALUE>
struct alpha: public _baseIterator
{
	alpha(_IT& it): s(it) { }

	node<_TVALUE>* operator()()
	{
		bool bDecimal = false;
		int a = s;
		for( ; s() && ('A' <= s() && s() <= 'Z' || 'a' <= s() && s() <= 'z' || s() == '_'
					|| a < s && ('0' <= s() && s() <= '9' || !bDecimal && s() == '.')); ++s)
			bDecimal = s() == '.' ? true: bDecimal;
		return a < s ? node<_TVALUE>::newNodeAlpha(a, s): NULL;
	}

protected:
	_IT& s;
};


}
}
}
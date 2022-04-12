#pragma once
#include "pmb_parser_iterator.h"

namespace pmb
{
namespace parser
{
namespace iterators
{



template <class _IT, class _baseIterator, class _ITEM, typename _NDTYPE>
struct alpha: public _baseIterator
{
	alpha(_IT& it): s(it) { }

	node<_ITEM, _NDTYPE>* operator()()
	{
		bool bDecimal = false;
		typename _IT::idx a = (typename _IT::idx)s;
		for( ; s() && ('A' <= s() && s() <= 'Z' || 'a' <= s() && s() <= 'z' || s() == '_'
					|| a < s && ('0' <= s() && s() <= '9' || !bDecimal && s() == '.')); ++s)
			bDecimal = s() == '.' ? true: bDecimal;
		return a < s ? node<_ITEM, _NDTYPE>::newNodeAlpha(a, (typename _IT::idx)s) : nullptr;
	}

protected:
	_IT& s;
};


}
}
}
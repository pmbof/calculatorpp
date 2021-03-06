#pragma once
#include "pmb_parser_iterator.h"


namespace pmb
{
namespace parser
{
namespace iterators
{



template <class _IT, class _baseIterator, class _ITEM, typename _NDTYPE>
struct number: public _baseIterator
{
	number(_IT& it): s(it) { }

	node<_ITEM, _NDTYPE>* operator()()
	{
		typename _IT::idx n0 = (typename _IT::idx)s;

		int base = 10;
		if(s() == '0' && s(1) && s(2))
		{
			++s;
			switch(s(1))
			{
			case 'x':
				base = 16;
				++s;
				break;
			case 'o':
				base = 8;
				++s;
				break;
			case 'b':
				base = 2;
				++s;
				break;
			default:
				--s;
				break;
			}
		}

		bool bDecimal = false;
		bool bExponential = false;
		typename _IT::idx n = (typename _IT::idx)s;
		for( ; s() && ('0' <= s() && s() < '0' + (base < 10 ? base: 10) || 10 < base && 'a' <= s.lower() && s.lower() < 'a' + base - 10)
				|| !bDecimal && s() == '.' || !bExponential && base == 10 && s.lower() == 'e'; ++s)
		{
			if(s() == '.')
				bDecimal = true;
			else if(s.lower() == 'e')
			{
				if((s(1) < '0' || '9' < s(1)) && (s(1) != '+' && s(1) != '-' || s(2) < '0' || '9' < s(2)))
					break;
				bDecimal = false;
				bExponential = true;
			}
		}
		if(bDecimal && n + 1 < (typename _IT::idx)s || !bDecimal && n < s)
			return node<_ITEM, _NDTYPE>::newNodeNumber(n, s._i);
		else if(n0 < s)
			--s;
		return n0 < s ? node<_ITEM, _NDTYPE>::newNodeNumber(n0, s._i) : nullptr;
	}

protected:
	_IT& s;
};


}
}
}
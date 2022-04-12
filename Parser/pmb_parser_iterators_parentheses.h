#pragma once
#include "pmb_parser_iterator.h"


namespace pmb
{
namespace parser
{
namespace iterators
{


template <class _IT, class _baseIterator, class _ITEM, typename _NDTYPE>
struct parentheses: public _baseIterator
{
	parentheses(_IT& it): s(it) { }

	node<_ITEM, _NDTYPE>* operator()()
	{
		typename _IT::idx p = s._i;
		int opened = 0;
		char open[] = {'(', '[', '{'};
		char close[] = {')', ']', '}'};
		int type;
		for(type = 0; s() && type < sizeof(open) / sizeof(*open) && s() != open[type] && s() != close[type]; ++type)
			;
		if(s() && type < sizeof(open) / sizeof(*open))
		{
			typename _IT::idx space = s._i;
			for( ; s() && (0 <= opened && open[type] == s() || s() == close[type]); )
			{
				opened += s() == open[type] ? 1: s() == close[type] ? -1: 0;
				space = (typename _IT::idx)++s;
				for(; s.isSpace(); ++s)
					;
			}
			if(space < s)
				s._i = space;

		}
		return p < s ? new nodes::parentheses<_ITEM, _NDTYPE>(p, s._i, open[type], opened): nullptr;
	}

protected:
	_IT& s;
};




}
}
}
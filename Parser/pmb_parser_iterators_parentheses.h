#pragma once
#include "pmb_parser_iterator.h"


namespace pmb
{
namespace parser
{
namespace iterators
{


template <class _IT, class _baseIterator, class _TVALUE>
struct parentheses: public _baseIterator
{
	parentheses(_IT& it): s(it) { }

	node<_TVALUE>* operator()()
	{
		int p = s;
		int opened = 0;
		char open[] = {'(', '[', '{'};
		char close[] = {')', ']', '}'};
		int type;
		for(type = 0; s() && type < sizeof(open) / sizeof(*open) && s() != open[type] && s() != close[type]; ++type)
			;
		if(s() && type < sizeof(open) / sizeof(*open))
		{
			int space = s;
			for( ; s() && (0 <= opened && open[type] == s() || s() == close[type]); )
			{
				opened += s() == open[type] ? 1: s() == close[type] ? -1: 0;
				space = ++s;
				for(; s.isSpace(); ++s)
					;
			}
			if(space < s)
				s._i = space;

		}
		return p < s ? new nodes::parentheses<_TVALUE>(p, s, open[type], opened): NULL;
	}

protected:
	_IT& s;
};




}
}
}
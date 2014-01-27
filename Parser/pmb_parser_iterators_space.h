#pragma once
#include "pmb_parser_iterator.h"


namespace pmb
{
namespace parser
{
namespace iterators
{


template <class _IT, class _baseIterator, class _TVALUE>
struct space: public _baseIterator
{
	space(_IT& it): s(it), _ini(0), _end(0) { }


	node<_TVALUE>* operator()()
	{
		_ini = s;
		for( ; s.isSpace(); ++s)
			;
		_end = s;
		return NULL;

//		return is < s && is != 0 && s() ? node<_TVALUE>::newNodeSpace(is, s): NULL;
	}


	bool release()
	{
		bool bRet = _ini < _end;
		_ini = _end;
		return bRet;
	}


protected:
	_IT& s;
	int _ini;
	int _end;
};


}
}
}
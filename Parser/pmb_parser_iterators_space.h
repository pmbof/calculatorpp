#pragma once
#include "pmb_parser_iterator.h"


namespace pmb
{
namespace parser
{
namespace iterators
{


template <class _IT, class _baseIterator, class _ITEM, typename _NDTYPE>
struct space: public _baseIterator
{
	space(_IT& it): s(it), _ini(0), _end(0) { }


	node<_ITEM, _NDTYPE>* operator()()
	{
		_ini = (typename _IT::idx)s;
		for( ; s.isSpace(); ++s)
			;
		_end = (typename _IT::idx)s;
		return NULL;

//		return is < s && is != 0 && s() ? node<_ITEM, _NDTYPE>::newNodeSpace(is, s): NULL;
	}


	bool release()
	{
		bool bRet = _ini < _end;
		_ini = _end;
		return bRet;
	}


protected:
	_IT& s;
	typename _IT::idx _ini;
	typename _IT::idx _end;
};


}
}
}
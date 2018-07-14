#pragma once
#include "pmb_parser_iterator.h"


namespace pmb
{
namespace parser
{
namespace iterators
{



template <class _IT, class _baseIterator, class _ITEM, typename _NDTYPE>
struct unknown: public _baseIterator
{
	unknown(_IT& it): s(it), _valid(false) { }


	node<_ITEM, _NDTYPE>* operator()()
	{
		if(_i0 == s && s())
		{
			if(!_valid)
			{
				_ini = _i0;
				_valid = true;
			}
			++s;
		}
		return NULL;
	}


	void initLoop()
	{
		_i0 = (typename _IT::idx)s;
	}


	node<_ITEM, _NDTYPE>* release()
	{
		node<_ITEM, _NDTYPE>* nd = _valid ? node<_ITEM, _NDTYPE>::newNodeUnknown(_ini, _i0) : NULL;
		_valid = false;
		return nd;
	}


protected:
	bool _valid;
	typename _IT::idx _i0, _ini;
	_IT& s;
};



}
}
}
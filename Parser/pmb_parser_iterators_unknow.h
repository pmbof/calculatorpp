#pragma once
#include "pmb_parser_iterator.h"


namespace pmb
{
namespace parser
{
namespace iterators
{



template <class _IT, class _baseIterator, class _TVALUE>
struct unknow: public _baseIterator
{
	unknow(_IT& it): s(it), _valid(false) { }


	node<_TVALUE>* operator()()
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
		_i0 = s;
	}


	node<_TVALUE>* release()
	{
		node<_TVALUE>* nd = _valid ? node<_TVALUE>::newNodeUnknow(_ini, _i0): NULL;
		_valid = false;
		return nd;
	}


protected:
	bool _valid;
	int _i0, _ini;
	_IT& s;
};



}
}
}
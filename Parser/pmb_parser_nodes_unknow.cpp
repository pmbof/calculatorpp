#include "StdAfx.h"
#include "pmb_parser_nodes_unknow.h"

#pragma once

namespace pmb
{
namespace parser
{
namespace nodes
{


template<class _TVALUE>
unknow<_TVALUE>::unknow(int ini, int end)
	: calc(ndUnknow, ini, end), _opr(NULL), _value(true)
{
}


template<class _TVALUE>
unknow<_TVALUE>::~unknow()
{
}


template<class _TVALUE>
const operation<_TVALUE>* unknow<_TVALUE>::getOperation() const
{
	return _opr;
}


template<class _TVALUE>
void unknow<_TVALUE>::setOperation(const operation<_TVALUE>* opr)
{
	_opr = opr;
	for(unknow<_TVALUE>* parent = static_cast<unknow<_TVALUE>*>(_parent); _opr && parent && parent->_type == ndUnknow && parent->_opr &&
					(*_opr < *parent->_opr || _opr->isLeftToRight() && !(*parent->_opr < *_opr)); 
				parent = static_cast<unknow<_TVALUE>*>(_parent))
		switchToUpNode();
}





template<class _TVALUE>
_TVALUE& unknow<_TVALUE>::getValue()
{
	return _value;
}

template<class _TVALUE>
const _TVALUE& unknow<_TVALUE>::getValue() const
{
	return _value;
}


}
}
}
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
	: calc(ndUnknow, ini, end), _opr(NULL), tvalue_type(0)//_value()
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
const function<_TVALUE>* unknow<_TVALUE>::getFunction() const
{
	return _fnc;
}

template<class _TVALUE>
int unknow<_TVALUE>::getPrecedence() const
{
	return _fnc ? function_table<_TVALUE>::getPrecedence(): _opr->getPrecedence();
}


template<class _TVALUE>
void unknow<_TVALUE>::setOperation(const operation<_TVALUE>* opr, const function<_TVALUE>* fnc)
{
	_opr = opr;
	_fnc = fnc;
	int precedence = getPrecedence();
	for(unknow<_TVALUE>* parent = static_cast<unknow<_TVALUE>*>(_parent); _opr && parent && parent->_type == ndUnknow && parent->_opr &&
					(precedence < parent->getPrecedence() || _opr->isLeftToRight() && !(parent->getPrecedence() < precedence)); 
				parent = static_cast<unknow<_TVALUE>*>(_parent))
		switchToUpNode();
}


template<class _TVALUE>
_TVALUE* unknow<_TVALUE>::getArguments() const
{
	if(!_fnc)
		return NULL;
	_TVALUE* args = new _TVALUE[_fnc->getNArgs()];

	node<_TVALUE>* ndArg = _opr->isLeftToRight() ? _right: _left;
	for(int i = 0; ndArg; ndArg = ndArg->getRight())
	{
		if(ndArg->getType() == ndParentheses)
		{
//			if(...)
			continue;
		}
//		if(ndArg->getType() == ndList)
//			args[i++] = ndArg->getLeft();
	}

	return args;
}


template<class _TVALUE>
_TVALUE& unknow<_TVALUE>::getValue()
{
	return *this;//_value;
}

template<class _TVALUE>
const _TVALUE& unknow<_TVALUE>::getValue() const
{
	return *this;//_value;
}


}
}
}
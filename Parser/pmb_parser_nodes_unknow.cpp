#include "StdAfx.h"
#include "pmb_parser_nodes_unknow.h"
#include "pmb_parser_nodes_parentheses.h"

#pragma once

#define PMB_PARSER_NODES_UNKNOWN_BINARY					0x01
#define PMB_PARSER_NODES_UNKNOWN_LEFTTORIGHT			0x02
#define PMB_PARSER_NODES_UNKNOWN_OPERATOR				0x04
#define PMB_PARSER_NODES_UNKNOWN_BUILDINFUNCTION		0x08
#define PMB_PARSER_NODES_UNKNOWN_USERDEFFUNCTION		0x10
#define PMB_PARSER_NODES_UNKNOWN_CANCREATELVARIABLE		0x20
#define PMB_PARSER_NODES_UNKNOWN_CANCREATERVARIABLE		0x40
#define PMB_PARSER_NODES_UNKNOWN_CHECKOPERATOR			0x80




namespace pmb
{
namespace parser
{
namespace nodes
{


template<class _ITEM, class _NDTYPE>
unknown<_ITEM, _NDTYPE>::unknown(ISIZE ini, ISIZE end)
	: calc(ndUnknown, ini, end), _pointer(nullptr)
{
}


template<class _ITEM, class _NDTYPE>
unknown<_ITEM, _NDTYPE>::~unknown()
{
}


template<class _ITEM, class _NDTYPE>
const void* unknown<_ITEM, _NDTYPE>::pointer() const
{
	return _pointer;
}




template<class _ITEM, class _NDTYPE>
void unknown<_ITEM, _NDTYPE>::set(const void* opr, bool binary, bool isLeftToRight, int precedence, bool canCreateLVariable, bool canCreateRVariable, const void* buildInFnc, const void* userFnc)
{
	_flag = (binary ? PMB_PARSER_NODES_UNKNOWN_BINARY : 0) | (isLeftToRight ? PMB_PARSER_NODES_UNKNOWN_LEFTTORIGHT : 0)
		| (canCreateLVariable ? PMB_PARSER_NODES_UNKNOWN_CANCREATELVARIABLE : 0)
		| (canCreateRVariable ? PMB_PARSER_NODES_UNKNOWN_CANCREATERVARIABLE : 0);
	_precedence = precedence;
	if (userFnc)
	{
		_flag |= PMB_PARSER_NODES_UNKNOWN_USERDEFFUNCTION;
		_pointer = userFnc;
	}
	else if (buildInFnc)
	{
		_flag |= PMB_PARSER_NODES_UNKNOWN_BUILDINFUNCTION;
		_pointer = buildInFnc;
	}
	else if (opr)
	{
		_flag |= PMB_PARSER_NODES_UNKNOWN_OPERATOR;
		_pointer = opr;
	}

	switch2UpNode();
}



template<class _ITEM, class _NDTYPE>
void unknown<_ITEM, _NDTYPE>::switch2UpNode()
{
	bool isLeftToRight = _flag & PMB_PARSER_NODES_UNKNOWN_LEFTTORIGHT;
	if (!(_flag & PMB_PARSER_NODES_UNKNOWN_BINARY) && _left && _right)
	{
		if (isLeftToRight)
			static_cast<unknown<_ITEM, _NDTYPE>*>(_right)->node<_ITEM, _NDTYPE>::switchToUpNode();
		else
			static_cast<unknown<_ITEM, _NDTYPE>*>(_left)->node<_ITEM, _NDTYPE>::switchToUpNode();
	}

	for (unknown<_ITEM, _NDTYPE>* parent = static_cast<unknown<_ITEM, _NDTYPE>*>(_parent);
				_pointer && parent && parent->_type == ndUnknown && parent->_pointer &&
					!(_flag & (PMB_PARSER_NODES_UNKNOWN_USERDEFFUNCTION | PMB_PARSER_NODES_UNKNOWN_BUILDINFUNCTION))
					&& (_precedence < parent->_precedence || _precedence == parent->_precedence && isLeftToRight);
						//	|| !_fnc && _opr->isLeftToRight() && !(precedence < parent->getPrecedence() < precedence));
				parent = static_cast<unknown<_ITEM, _NDTYPE>*>(_parent))
		switchToUpNode();
}



template<class _ITEM, class _NDTYPE>
void unknown<_ITEM, _NDTYPE>::set(const void* opr, bool isLeftToRight, int precedence, bool canCreateLVariable, bool canCreateRVariable)
{
	_flag = PMB_PARSER_NODES_UNKNOWN_BINARY | (isLeftToRight ? PMB_PARSER_NODES_UNKNOWN_LEFTTORIGHT : 0)
		| (canCreateLVariable ? PMB_PARSER_NODES_UNKNOWN_CANCREATELVARIABLE : 0)
		| (canCreateRVariable ? PMB_PARSER_NODES_UNKNOWN_CANCREATERVARIABLE : 0);
	_precedence = precedence;
	if (opr)
	{
		_flag |= PMB_PARSER_NODES_UNKNOWN_OPERATOR | PMB_PARSER_NODES_UNKNOWN_CHECKOPERATOR;
		_pointer = opr;
	}

	switch2UpNode();
}



template<class _ITEM, class _NDTYPE>
unknown<_ITEM, _NDTYPE>* unknown<_ITEM, _NDTYPE>::add(ISIZE ini)
{
	return nullptr;
}



template<class _ITEM, class _NDTYPE>
bool unknown<_ITEM, _NDTYPE>::isFirstLeft() const
{
	return _flag & PMB_PARSER_NODES_UNKNOWN_BINARY && _flag & PMB_PARSER_NODES_UNKNOWN_LEFTTORIGHT 
		|| !(_flag & PMB_PARSER_NODES_UNKNOWN_BINARY) && !(_flag & PMB_PARSER_NODES_UNKNOWN_LEFTTORIGHT);
}

template<class _ITEM, class _NDTYPE>
bool unknown<_ITEM, _NDTYPE>::isFirstRight() const
{
	return _flag & PMB_PARSER_NODES_UNKNOWN_BINARY && !(_flag & PMB_PARSER_NODES_UNKNOWN_LEFTTORIGHT)
		|| !(_flag & PMB_PARSER_NODES_UNKNOWN_BINARY) && _flag & PMB_PARSER_NODES_UNKNOWN_LEFTTORIGHT;
}

template<class _ITEM, class _NDTYPE>
int unknown<_ITEM, _NDTYPE>::precedence() const
{
	return _precedence;
}

template<class _ITEM, class _NDTYPE>
bool unknown<_ITEM, _NDTYPE>::isValid() const
{
	return _pointer;
}

template<class _ITEM, class _NDTYPE>
bool unknown<_ITEM, _NDTYPE>::operator!() const
{
	return !_pointer;
}

template<class _ITEM, class _NDTYPE>
bool unknown<_ITEM, _NDTYPE>::isBinary() const
{
	return _flag & PMB_PARSER_NODES_UNKNOWN_BINARY;
}

template<class _ITEM, class _NDTYPE>
bool unknown<_ITEM, _NDTYPE>::isLeftToRight() const
{
	return _flag & PMB_PARSER_NODES_UNKNOWN_LEFTTORIGHT;
}

template<class _ITEM, class _NDTYPE>
bool unknown<_ITEM, _NDTYPE>::isCallFunction() const
{
	return _flag & PMB_PARSER_NODES_UNKNOWN_BUILDINFUNCTION || _flag & PMB_PARSER_NODES_UNKNOWN_USERDEFFUNCTION;
}

template<class _ITEM, class _NDTYPE>
bool unknown<_ITEM, _NDTYPE>::isCallBuildInFunction() const
{
	return _flag & PMB_PARSER_NODES_UNKNOWN_BUILDINFUNCTION;
}

template<class _ITEM, class _NDTYPE>
bool unknown<_ITEM, _NDTYPE>::isCallUserDefFunction() const
{
	return _flag & PMB_PARSER_NODES_UNKNOWN_USERDEFFUNCTION;
}


template<class _ITEM, class _NDTYPE>
bool unknown<_ITEM, _NDTYPE>::canCreateVariable() const
{
	return _flag & (PMB_PARSER_NODES_UNKNOWN_CANCREATELVARIABLE | PMB_PARSER_NODES_UNKNOWN_CANCREATERVARIABLE);
}


template<class _ITEM, class _NDTYPE>
bool unknown<_ITEM, _NDTYPE>::canCreateLVariable() const
{
	return _flag & PMB_PARSER_NODES_UNKNOWN_CANCREATELVARIABLE;
}

template<class _ITEM, class _NDTYPE>
bool unknown<_ITEM, _NDTYPE>::canCreateRVariable() const
{
	return _flag & PMB_PARSER_NODES_UNKNOWN_CANCREATERVARIABLE;
}


template<class _ITEM, class _NDTYPE>
bool unknown<_ITEM, _NDTYPE>::checkOperator() const
{
	return _flag & PMB_PARSER_NODES_UNKNOWN_CHECKOPERATOR;
}


/*
template<class _ITEM, class _NDTYPE>
const _TVALUE* unknown<_ITEM, _NDTYPE>::getArguments() const
{
	if(!_fnc)
		return NULL;
	const _TVALUE* args;// = new _TVALUE[_fnc->getNArgs()];

	node<_TVALUE, _ITEM, _NDTYPE>* ndArg = _opr->isLeftToRight() ? _right : _left;
	if (ndArg->getType() == ndParentheses)
		args = &static_cast<parentheses<_TVALUE, _ITEM, _NDTYPE>*>(ndArg)->getValue();
	else
		args = NULL;
	return args;
}*/




}
}
}
#include "StdAfx.h"

#pragma once
#include "pmb_parser_nodes_parentheses.h"


namespace pmb
{
namespace parser
{
namespace nodes
{



template<class _ITEM, class _NDTYPE>
parentheses<_ITEM, _NDTYPE>::parentheses(int ini, int end, char type, int opened)
	: calc(ndParentheses, ini, end), _prtype(type), _opened(opened)
{
}


template<class _ITEM, class _NDTYPE>
parentheses<_ITEM, _NDTYPE>::~parentheses()
{
}

template<class _ITEM, class _NDTYPE>
inline short parentheses<_ITEM, _NDTYPE>::getOpened() const
{
	return _opened;
}


template<class _ITEM, class _NDTYPE>
inline bool parentheses<_ITEM, _NDTYPE>::isThisType(const cnode* nd) const
{
	const parentheses* pr = static_cast<const parentheses*>(nd);
	return nd && pr->_type == ndParentheses && pr->_prtype == _prtype;
}


template<class _ITEM, class _NDTYPE>
char parentheses<_ITEM, _NDTYPE>::getClass() const
{
	return _prtype;
}

template<class _ITEM, class _NDTYPE>
int parentheses<_ITEM, _NDTYPE>::countListChildNodes() const
{
	int count = _right ? 1: 0;
	for (const node<_ITEM, _NDTYPE>* pList = _right; pList && pList->getType() == ndList; ++count, pList = pList->getLeft())
		;
	return count;
}

template<class _ITEM, class _NDTYPE>
parentheses<_ITEM, _NDTYPE>* parentheses<_ITEM, _NDTYPE>::split(short count)
{
	parentheses* newParentheses = new parentheses(_end - count, _end, _prtype, count);
	insertInRight(newParentheses);
	_opened -= count;
	_end -= count;
	return newParentheses;
}


/*
template<class _ITEM, class _NDTYPE>
_TVALUE& parentheses<_ITEM, _NDTYPE>::getValue()
{
	return *this;
	//return _value;  prueba 2018-05-07
}


template<class _ITEM, class _NDTYPE>
const _TVALUE& parentheses<_ITEM, _NDTYPE>::getValue() const
{
	return *this;
	//return _value;  prueba 2018-05-07
}
*/

}
}
}
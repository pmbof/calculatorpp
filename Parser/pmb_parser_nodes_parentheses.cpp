#include "StdAfx.h"
#include "pmb_parser_nodes_parentheses.h"


namespace pmb
{
namespace parser
{
namespace nodes
{



template<class _TVALUE>
parentheses<_TVALUE>::parentheses(int ini, int end, char type, int opened)
	: calc(ndParentheses, ini, end), _prtype(type), _opened(opened)
{
}


template<class _TVALUE>
parentheses<_TVALUE>::~parentheses()
{
}


template<class _TVALUE>
int parentheses<_TVALUE>::countListChildNodes() const
{
	int count = 0;
	for(const parentheses<_TVALUE>* pList = _right; pList && pList->getType() == ndList; ++count, pList = pList->_right)
		;
	return count;
}


template<class _TVALUE>
_TVALUE& parentheses<_TVALUE>::getValue()
{
	return _value;
}


template<class _TVALUE>
const _TVALUE& parentheses<_TVALUE>::getValue() const
{
	return _value;
}


}
}
}
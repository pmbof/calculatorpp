#include "StdAfx.h"
#include "pmb_parser_node.h"
#include "pmb_parser_operation.h"
#include <string.h>

namespace pmb
{
namespace parser
{


template<class _TVALUE>
const operation<_TVALUE> operation_table<_TVALUE>::_opr[] = {
	operation<_TVALUE>("+", 250, false, "positive", "positive", operations<_TVALUE>::positive),
	operation<_TVALUE>("-", 250, false, "negative", "negative", operations<_TVALUE>::negative),
	operation<_TVALUE>("^", 200, true, "power", "exponentiation", operations<_TVALUE>::exponentiation),
	operation<_TVALUE>("¨", 200, true, "root", "root", operations<_TVALUE>::root),
	operation<_TVALUE>("", 110, true, "product implicit", "multiplication implicit or call function", operations<_TVALUE>::multiplication),
	operation<_TVALUE>(" ", 110, true, "product space", "multiplication space or call function", operations<_TVALUE>::multiplication),
	operation<_TVALUE>("*", 100, true, "product", "multiplication", operations<_TVALUE>::multiplication),
	operation<_TVALUE>("/", 100, true, "cocient", "division", operations<_TVALUE>::division),
	operation<_TVALUE>("\\", 100, true, "modulo", "congruence relation", operations<_TVALUE>::modulo),
	operation<_TVALUE>("+", 50, true, "add", "addition", operations<_TVALUE>::addition),
	operation<_TVALUE>("-", 50, true, "substract", "substraction", operations<_TVALUE>::substraction),
	operation<_TVALUE>("=", 10, false, "assignation", "assignation", operations<_TVALUE>::assignation, true, false),
	operation<_TVALUE>("=", 0, true, "result", "result", operations<_TVALUE>::result)
};

template<class _TVALUE>
const int operation_table<_TVALUE>::_oprSize = 13;


template<class _TVALUE>
const operation<_TVALUE>* operation_table<_TVALUE>::find(const node<_TVALUE>* nd, const char* expr) const
{
	for(int i = 0; i < _oprSize; ++i)
	{
		if((_opr[i].isBinary() && nd->getLeft() && nd->getRight() || 
			!_opr[i].isBinary() && _opr[i].isLeftToRight() && nd->getLeft() && !nd->getRight() || 
			!_opr[i].isBinary() && !_opr[i].isLeftToRight() && !nd->getLeft() && nd->getRight())
				&& _opr[i].compare(nd->getCharPtr(expr), nd->len()))
		{
			return _opr + i;
		}
	}
	return NULL;
}




}
}
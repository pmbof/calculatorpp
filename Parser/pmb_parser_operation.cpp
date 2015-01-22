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
	operation<_TVALUE>("+", 250, false, "positive", "positive", &_TVALUE::VALUE::positive, &_TVALUE::VALUE::place),
	operation<_TVALUE>("-", 250, false, "negative", "negative", &_TVALUE::VALUE::negative, &_TVALUE::VALUE::place),
	operation<_TVALUE>("!", 250, true, "factorial", "factorial", &_TVALUE::VALUE::factorial, &_TVALUE::VALUE::place),
	operation<_TVALUE>("^", 200, true, "power", "exponentiation", &_TVALUE::VALUE::exponentiation, &_TVALUE::VALUE::place),
	operation<_TVALUE>("¨", 200, true, "root", "root", &_TVALUE::VALUE::root, &_TVALUE::VALUE::place),
	operation<_TVALUE>("", 110, true, "product implicit", "multiplication implicit or call function", &_TVALUE::VALUE::multiplication, &_TVALUE::VALUE::place, true),
	operation<_TVALUE>(" ", 110, true, "product space", "multiplication space or call function", &_TVALUE::VALUE::multiplication, &_TVALUE::VALUE::place, true),
	operation<_TVALUE>(" ", 110, false, "product space inverse", "multiplication space or call function right to left", &_TVALUE::VALUE::multiplication, &_TVALUE::VALUE::place, true),
	operation<_TVALUE>("*", 100, true, "product", "multiplication", &_TVALUE::VALUE::multiplication, &_TVALUE::VALUE::place),
	operation<_TVALUE>("/", 100, true, "cocient", "division", &_TVALUE::VALUE::division, &_TVALUE::VALUE::place),
	operation<_TVALUE>("\\", 100, true, "modulo", "congruence relation", &_TVALUE::VALUE::modulo, &_TVALUE::VALUE::place),
	operation<_TVALUE>("+", 50, true, "add", "addition", &_TVALUE::VALUE::addition, &_TVALUE::VALUE::place),
	operation<_TVALUE>("-", 50, true, "substract", "substraction", &_TVALUE::VALUE::substraction, &_TVALUE::VALUE::place),
	operation<_TVALUE>("=", 10, false, "assignation", "assignation", &_TVALUE::VALUE::assignation, &_TVALUE::VALUE::assignation_place, true, false),
	operation<_TVALUE>("=", 0, true, "result", "result", &_TVALUE::VALUE::result, &_TVALUE::VALUE::place)
};

template<class _TVALUE>
const int operation_table<_TVALUE>::_oprSize = 14;


template<class _TVALUE>
operation<_TVALUE>::operation(const char* symbol, int precedence, bool leftToRight, const char* name, const char* description,
					tpOprFunc2 oprFunc2, tpOprPlace oprPlace, bool canCallFunction)
	: _precedence(precedence), _leftToRight(leftToRight), _binary(true), _oprFunc2(oprFunc2), _oprPlace(oprPlace), _canCallFunction(canCallFunction), 
				_canCreateLVariable(false), _canCreateRVariable(false) 
{
	_symbol = new char[(_len = strlen(symbol)) + 1];
	strcpy(_symbol, symbol);
	_name = new char[strlen(name) + 1];
	strcpy(_name, name);
	_description = new char[strlen(description) + 1];
	strcpy(_description, description);
}

template<class _TVALUE>
operation<_TVALUE>::operation(const char* symbol, int precedence, bool leftToRight, const char* name, const char* description,
						tpOprFunc2 oprFunc2, tpOprPlace oprPlace, bool canCreateLVariable, bool canCreateRVariable)
	: _precedence(precedence), _leftToRight(leftToRight), _binary(true), _oprFunc2(oprFunc2), _oprPlace(oprPlace), _canCallFunction(false),
			_canCreateLVariable(canCreateLVariable), _canCreateRVariable(canCreateRVariable)
{
	_symbol = new char[(_len = strlen(symbol)) + 1];
	strcpy(_symbol, symbol);
	_name = new char[strlen(name) + 1];
	strcpy(_name, name);
	_description = new char[strlen(description) + 1];
	strcpy(_description, description);
}

template<class _TVALUE>
operation<_TVALUE>::operation(const char* symbol, int precedence, bool leftToRight, const char* name, const char* description,
					tpOprFunc1 oprFunc1, tpOprPlace oprPlace)
			: _precedence(precedence), _leftToRight(leftToRight), _binary(false), _oprFunc1(oprFunc1), _oprPlace(oprPlace), _canCreateLVariable(false), _canCreateRVariable(false),
					_canCallFunction(false)
{
	_symbol = new char[(_len = strlen(symbol)) + 1];
	strcpy(_symbol, symbol);
	_name = new char[strlen(name) + 1];
	strcpy(_name, name);
	_description = new char[strlen(description) + 1];
	strcpy(_description, description);
}

template<class _TVALUE>
operation<_TVALUE>::~operation()
{
	delete [] _symbol;
	if(_name)
		delete [] _name;
	if(_description)
		delete [] _description;
}

template<class _TVALUE>
bool operation<_TVALUE>::compare(const char* expr, int len) const
{
	return _len == len && !strncmp(_symbol, expr, len);
}

template<class _TVALUE>
bool operation<_TVALUE>::operator<(const operation<_TVALUE>& opr) const
{
	return _precedence < opr._precedence;
}

template<class _TVALUE>
bool operation<_TVALUE>::operator<(int rPrecedence) const
{
	return _precedence < rPrecedence;
}

template<class _TVALUE>
bool operation<_TVALUE>::isFirstLeft() const
{
	return _binary && _leftToRight || !_binary && !_leftToRight;
}

template<class _TVALUE>
bool operation<_TVALUE>::isFirstRight() const
{
	return _binary && !_leftToRight || !_binary && _leftToRight;
}

template<class _TVALUE>
bool operation<_TVALUE>::canCallFunction() const
{
	return _canCallFunction;
}

template<class _TVALUE>
const char* operation<_TVALUE>::getSymbol() const
{
	return _symbol;
}

template<class _TVALUE>
const char* operation<_TVALUE>::getName() const
{
	return _name;
}

template<class _TVALUE>
const char* operation<_TVALUE>::getDescription() const
{
	return _description;
}

template<class _TVALUE>
bool operation<_TVALUE>::isLeftToRight() const
{
	return _leftToRight;
}

template<class _TVALUE>
bool operation<_TVALUE>::isBinary() const
{
	return _binary;
}

template<class _TVALUE>
bool operation<_TVALUE>::canCreateLVariable() const
{
	return _canCreateLVariable;
}

template<class _TVALUE>
bool operation<_TVALUE>::canCreateRVariable() const
{
	return _canCreateRVariable;
}

template<class _TVALUE>
int operation<_TVALUE>::getPrecedence() const
{
	return _precedence;
}

template<class _TVALUE>
unsigned int operation<_TVALUE>::getFunctor1() const
{
	return (int)(void*&)_oprFunc1;
}

template<class _TVALUE>
unsigned int operation<_TVALUE>::getFunctor2() const
{
	return (int)(void*&)_oprFunc2;
}

template<class _TVALUE>
void operation<_TVALUE>::operator()(_TVALUE& result, _TVALUE& value) const
{
	(*result((*result->*_oprPlace)(true, *value, NULL), value)->*_oprFunc1)(**value);
//	(*result->*_oprFunc1)(**value);
}

template<class _TVALUE>
void operation<_TVALUE>::operator()(_TVALUE& result, _TVALUE& left, _TVALUE& right) const
{
	_TVALUE::_valuePtr l = *left;
	_TVALUE::_valuePtr r = *right;
	bool bHaveLPriority = result.haveLeftPriority(left, right);
	
	_TVALUE::_valuePtr place = (*result->*_oprPlace)(bHaveLPriority, l, r);
	_TVALUE::_valuePtr res2 = *result(place, left, right);
	(place->*_oprFunc2)(*l, *r);
	// All this is equivalet to:
//	(*result((*result->*_oprPlace)(bHaveLPriority, l, r), left, right)->*_oprFunc2)(*l, *r);
}


template<class _TVALUE>
bool operator<(int lPrecedence, const operation<_TVALUE>& opr)
{
	return lPrecedence < opr._precedence;
}





template<class _TVALUE>
const operation<_TVALUE>* operation_table<_TVALUE>::find(const node<_TVALUE>* nd, const char* expr) const
{
	for(int i = 0; i < _oprSize; ++i)
	{
		if((_opr[i].isBinary() && nd->getLeft() && nd->getRight() || 
				!_opr[i].isBinary() && _opr[i].isLeftToRight() && nd->getLeft() && !nd->getRight() || 
				!_opr[i].isBinary() && !_opr[i].isLeftToRight() && !nd->getLeft() && nd->getRight())
					&& _opr[i].compare(nd->getCharPtr(expr), nd->len()))
			return _opr + i;
	}
	return NULL;
}


template<class _TVALUE>
const operation<_TVALUE>* operation_table<_TVALUE>::get(int i) const
{
	return _opr + i;
}

template<class _TVALUE>
int operation_table<_TVALUE>::size() const
{
	return _oprSize;
}



}
}
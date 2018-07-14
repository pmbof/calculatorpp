#include "StdAfx.h"

#include "pmb_parser_node.h"
#include "pmb_parser_operation.h"

#include <string.h>


namespace pmb
{
namespace parser
{


/*template<class _TVALUE, class _NODE>
const operation<_TVALUE> operation_table<_TVALUE, _NODE>::_opr[] = {
	pmb::parser::operation<_TVALUE>("+", 250, false, "positive", "positive", &_TVALUE::tpvalue::positive, &_TVALUE::tpvalue::place),
	pmb::parser::operation<_TVALUE>("-", 250, false, "negative", "negative", &_TVALUE::tpvalue::negative, &_TVALUE::tpvalue::place),
	pmb::parser::operation<_TVALUE>("!", 250, true, "factorial", "factorial", &_TVALUE::tpvalue::factorial, &_TVALUE::tpvalue::place),
	pmb::parser::operation<_TVALUE>("^", 200, true, "power", "exponentiation", &_TVALUE::tpvalue::exponentiation, &_TVALUE::tpvalue::place),
	pmb::parser::operation<_TVALUE>("¨", 200, true, "root", "root", &_TVALUE::tpvalue::root, &_TVALUE::tpvalue::place),
	pmb::parser::operation<_TVALUE>("", 110, true, "product implicit", "multiplication implicit or call function", &_TVALUE::tpvalue::multiplication, &_TVALUE::tpvalue::place, true),
	pmb::parser::operation<_TVALUE>(" ", 110, true, "product space", "multiplication space or call function", &_TVALUE::tpvalue::multiplication, &_TVALUE::tpvalue::place, true),
	pmb::parser::operation<_TVALUE>(" ", 110, false, "product space inverse", "multiplication space or call function right to left", &_TVALUE::tpvalue::multiplication, &_TVALUE::tpvalue::place, true),
	pmb::parser::operation<_TVALUE>("*", 100, true, "product", "multiplication", &_TVALUE::tpvalue::multiplication, &_TVALUE::tpvalue::place),
	pmb::parser::operation<_TVALUE>("/", 100, true, "cocient", "division", &_TVALUE::tpvalue::division, &_TVALUE::tpvalue::place),
	pmb::parser::operation<_TVALUE>("\\", 100, true, "modulo", "congruence relation", &_TVALUE::tpvalue::modulo, &_TVALUE::tpvalue::place),
	pmb::parser::operation<_TVALUE>("+", 50, true, "add", "addition", &_TVALUE::tpvalue::addition, &_TVALUE::tpvalue::place),
	pmb::parser::operation<_TVALUE>("-", 50, true, "substract", "substraction", &_TVALUE::tpvalue::substraction, &_TVALUE::tpvalue::place),
	pmb::parser::operation<_TVALUE>("=", 10, false, "assignation", "assignation", &_TVALUE::tpvalue::assignation, &_TVALUE::tpvalue::assignation_place, true, false),
	pmb::parser::operation<_TVALUE>("=", 0, true, "result", "result", &_TVALUE::tpvalue::result, &_TVALUE::tpvalue::place)
};

template<class _TVALUE, class _NODE>
const int operation_table<_TVALUE, _NODE>::_oprSize = 14;*/


template<class _TVALUE>
operation<_TVALUE>::operation(const char * symbol, int precedence, bool leftToRight, bool binary, const char * name, const char * description, tpFunc func,
			bool canCallFunction, bool canCreateLVariable, bool canCreateRVariable)
	: _precedence(precedence), _leftToRight(leftToRight), _binary(binary), _fnc(func), _canCallFunction(canCallFunction),
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
void operation<_TVALUE>::operator()(_TVALUE& values) const
{
	typename transporter_args::nargs nvals = values.nArgs();
	if (_binary && values.size() != 2 && (nvals != 2 && !(_canCreateLVariable || _canCreateRVariable) || nvals != 1 && nvals != 2))
		throw exception(2 < nvals || 2 < values.size() ? "too many arguments for operator %item" : values.haveLeft() ? "missing right value for operator %item" : "missing left value for operator %item");
	else if (!_binary && (nvals != 1 || values.size() != 1))
		throw exception(1 < nvals || 1 < values.size() ? "too many arguments for operator %item" : _leftToRight ? "missing left value for operator %item" : "missing right value for operator %item");

	if (!_canCreateLVariable && !_canCreateRVariable)
		values.placeForResult();
	(*_fnc)(values);
}



template<class _TVALUE>
bool operator<(int lPrecedence, const operation<_TVALUE>& opr)
{
	return lPrecedence < opr._precedence;
}







template<class _OPR, class _NODE>
operation_table<_OPR, _NODE>::operation_table(const operation* opr, size_t size)
	: _opr(opr), _oprSize(size)
{
}


template<class _OPR, class _NODE>
const typename operation_table<_OPR, _NODE>::operation*
	operation_table<_OPR, _NODE>::find(const _NODE* nd, const char* expr) const
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


template<class _OPR, class _NODE>
const typename operation_table<_OPR, _NODE>::operation*
	operation_table<_OPR, _NODE>::get(size_t i) const
{
	return _opr + i;
}

template<class _OPR, class _NODE>
typename operation_table<_OPR, _NODE>::size_t operation_table<_OPR, _NODE>::size() const
{
	return _oprSize;
}



}
}
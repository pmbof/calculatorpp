#include "StdAfx.h"

#include "pmb_parser_node.h"
#include "pmb_parser_operation.h"

#include <string.h>


namespace pmb
{
namespace parser
{




template<class _TVALUE>
operation<_TVALUE>::operation(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func,
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
int operation<_TVALUE>::getSymbolLen() const
{
	return _len;
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
	if (_binary)
	{
		if (values.capacity() != 2)
			throw exception(2 < values.capacity() ? "too many arguments for operator %item" : values.haveLeft() ? "missing right value for operator %item" : "missing left value for operator %item");
		else if (nvals != 2)
		{
			if (!_canCreateLVariable && !values.haveLeft())
				throw exception("missing left value for operator %item");
			else if (!_canCreateRVariable && !values.haveRight())
				throw exception("missing right value for operator %item");
		}
	}
	if (!_binary)
	{
		if (nvals < 1)
		{
			if (!_canCreateLVariable && !_canCreateRVariable)
				throw exception("missing value for operator %item");
		}
	}
	if (!_canCreateLVariable && !_canCreateRVariable)
		values.placeForResult();
	try
	{
		(*_fnc)(values);
	}
	catch (const char* ex)
	{
		throw exception(ex);
	}
}



template<class _TVALUE>
void operation<_TVALUE>::print(pmb::log_type lgtype) const
{
	pmb::log* lg = pmb::log::instance();
	lg->trace(lgtype, "\t\t- symbol: %s[%d]\n", std::string(_symbol, _len).c_str(), _len);
	lg->trace(lgtype, "\t\t- precedence: %d\n", _precedence);
	lg->trace(lgtype, "\t\t- %s\n", _leftToRight ? "left to right (->)" : "right to left (<-)");
	lg->trace(lgtype, "\t\t- %s\n", _binary ? "binary" : "unitary");
	lg->trace(lgtype, "\t\t- can call function: %s\n", _canCallFunction ? "true" : "false");
	lg->trace(lgtype, "\t\t- can create left variable: %s\n", _canCreateLVariable ? "true" : "false");
	lg->trace(lgtype, "\t\t- can create right variable: %s\n", _canCreateRVariable ? "true" : "false");
	lg->trace(lgtype, "\t\t- can create left variable: %s\n", _canCreateLVariable ? "true" : "false");
	lg->trace(lgtype, "\t\t- name: %s\n", _name);
	lg->trace(lgtype, "\t\t- description: %s\n", _description);
}


template<class _TVALUE>
bool operation<_TVALUE>::check() const
{
	return 0 <= _len && _len < 4;
}



template<class _TVALUE>
bool operator<(int lPrecedence, const operation<_TVALUE>& opr)
{
	return lPrecedence < opr._precedence;
}







template<class _OPR, class _NODE>
operation_table<_OPR, _NODE>::operation_table(const operation** opr, size_t size)
	: _opr(opr), _oprSize(size)
{
}




template<class _OPR, class _NODE>
const typename operation_table<_OPR, _NODE>::operation*
	operation_table<_OPR, _NODE>::find(const _NODE* nd, const char* expr) const
{
	std::vector<size_t> vfs;
	for(size_t i = 0; i < _oprSize; ++i)
	{
		if (_opr[i]->compare(nd->getCharPtr(expr), nd->len()))
		{
			vfs.push_back(i);
			if((_opr[i]->isBinary() && nd->getLeft() && nd->getRight() || 
					!_opr[i]->isBinary() && _opr[i]->isLeftToRight() && nd->getLeft() && !nd->getRight() || 
					!_opr[i]->isBinary() && !_opr[i]->isLeftToRight() && !nd->getLeft() && nd->getRight()))
				return _opr[i];
		}
	}

	for (size_t vi = 0; vi < vfs.size(); ++vi)
	{
		size_t i = vfs[vi];
		if ((_opr[i]->isBinary() && nd->getLeft() && nd->getRight() ||
				!_opr[i]->isBinary() && _opr[i]->isLeftToRight() && nd->getLeft() ||
				!_opr[i]->isBinary() && !_opr[i]->isLeftToRight() && nd->getRight()))
			return _opr[i];
	}

	return nullptr;
}




template<class _OPR, class _NODE>
const typename operation_table<_OPR, _NODE>::operation*
	operation_table<_OPR, _NODE>::get(size_t i) const
{
	return _opr[i];
}




template<class _OPR, class _NODE>
typename operation_table<_OPR, _NODE>::size_t operation_table<_OPR, _NODE>::size() const
{
	return _oprSize;
}






}
}
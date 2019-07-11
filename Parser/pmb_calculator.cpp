#include "StdAfx.h"
#include "pmb_calculator.h"
//#include "pmb_parser_algorithm.cpp"

//#include "pmb_parser_symbol.cpp"

namespace pmb
{


template <class _BLOCK, class _OPRTABLE>
calculator<_BLOCK, _OPRTABLE>::calculator(_tdOprTable* operation_table, _BLOCK* pBlock)///_tdSymbols* symbols)
	: _base(operation_table, pBlock), _result(nullptr)
{
}



template <class _BLOCK, class _OPRTABLE>
calculator<_BLOCK, _OPRTABLE>::~calculator()
{
	clear();
}


template<class _BLOCK, class _OPRTABLE>
void calculator<_BLOCK, _OPRTABLE>::clear()
{
	if (_result)
	{
		delete _result;
		_result = nullptr;
		_rexpr = nullptr;
	}
	_base::clear();
}


template<class _BLOCK, class _OPRTABLE>
inline bool calculator<_BLOCK, _OPRTABLE>::add_unit(const typename tpChar* name, const typename tpChar* expression, const typename tpChar* group, bool automatic)
{
	if (!_pBlock->variables()->defining_unit())
		return false;
	_base::calculate(expression);
	return _pBlock->variables()->add_by_name(name, _pBlock->tresult(), automatic, group);
}



template<class _BLOCK, class _OPRTABLE>
inline bool calculator<_BLOCK, _OPRTABLE>::add_unit(const typename tpChar* expression, bool automatic)
{
	if (!_pBlock->variables()->defining_unit())
		return false;
	_base::calculate(expression);
	return _pBlock->variables()->add_by_name(nullptr, _pBlock->tresult(), automatic);
}



template<class _BLOCK, class _OPRTABLE>
bool calculator<_BLOCK, _OPRTABLE>::add_constant(const typename tpChar* name, const typename tpChar* expression)
{
	if (!_pBlock->variables()->is_set_variable())
		return false;
	_base::calculate(expression);
	return _pBlock->variables()->add_constant(name);
}



template<class _BLOCK, class _OPRTABLE>
bool calculator<_BLOCK, _OPRTABLE>::calculate(const tpChar* expr)
{
	if (_result)
	{
		delete _result;
		_result = nullptr;
		_rexpr = nullptr;
	}
	return _base::calculate(expr);
}



template<class _BLOCK, class _OPRTABLE>
bool calculator<_BLOCK, _OPRTABLE>::parser_result(const tpChar* rexpr)
{
	if (_result)
	{
		delete _result;
		_result = nullptr;
		_rexpr = nullptr;
	}

	if (!rexpr)
		return false;
	
	const tpChar* expr = _expr;
	_base::tptree* tree = _tree;
	_tree = nullptr;
	bool bRet = parser(rexpr);
	if (bRet)
	{
		_result = _tree;
		_rexpr = _expr;
	}
	else
		delete _tree;
	_expr = expr;
	_tree = tree;
	return bRet;
}



template<class _BLOCK, class _OPRTABLE>
const typename calculator<_BLOCK, _OPRTABLE>::tptree* calculator<_BLOCK, _OPRTABLE>::getResultNode() const
{
	return _result;
}







}
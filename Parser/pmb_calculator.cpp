#include "StdAfx.h"
#include "pmb_calculator.h"
//#include "pmb_parser_algorithm.cpp"

//#include "pmb_parser_symbol.cpp"

namespace pmb
{


template <class _BLOCK, class _OPRTABLE>
calculator<_BLOCK, _OPRTABLE>::calculator(_tdOprTable* operation_table, _BLOCK* pBlock)///_tdSymbols* symbols)
	: _base(operation_table, pBlock)
{
/*	_symbols->addSetVariable("Units");
	_symbols->addSetVariable("Constants");
	_symbols->addSetVariable("Variables");

	_base::_tdSymbols::_tpList lst;
	lst.push_back("Units");
	lst.push_back("Constants");
	lst.push_back("Variables");
	_symbols->addSetSearch("Units", "Units", lst);

	lst.clear();
	lst.push_back("Constants");
	lst.push_back("Units");
	lst.push_back("Variables");
	_symbols->addSetSearch("Constants", "Constants", lst);

	lst.clear();
	lst.push_back("Variables");
	lst.push_back("Constants");
	lst.push_back("Units");
	_symbols->addSetSearch("Variables", "Variables", lst);*/
}



template <class _BLOCK, class _OPRTABLE>
calculator<_BLOCK, _OPRTABLE>::~calculator()
{
}


template<class _BLOCK, class _OPRTABLE>
inline bool calculator<_BLOCK, _OPRTABLE>::add_unit(const typename tpChar* name, const typename tpChar* expression, const typename tpChar* group)
{
	if (!_pBlock->variables()->defining_unit())
		return false;
	calculate(expression);
	return _pBlock->variables()->add_by_name(name, _pBlock->tresult(), group);
}



template<class _BLOCK, class _OPRTABLE>
inline bool calculator<_BLOCK, _OPRTABLE>::add_unit(const typename tpChar* expression)
{
	if (!_pBlock->variables()->defining_unit())
		return false;
	calculate(expression);
	return _pBlock->variables()->add_by_name(nullptr, _pBlock->tresult());
}



template<class _BLOCK, class _OPRTABLE>
bool calculator<_BLOCK, _OPRTABLE>::add_constant(const typename tpChar* name, const typename tpChar* expression)
{
	if (!_pBlock->variables()->is_set_variable())
		return false;
	calculate(expression);
	return _pBlock->variables()->add_constant(name);
}



}
#include "StdAfx.h"
#include "pmb_calculator.h"
//#include "pmb_parser_algorithm.cpp"

//#include "pmb_parser_symbol.cpp"

namespace pmb
{


template <class _TVALUE>
calculator<_TVALUE>::calculator(_tdSymbols& symbols)
	: _base(symbols)
{
	_symbols.addSetVariable("Units");
	_symbols.addSetVariable("Constants");
	_symbols.addSetVariable("Variables");

	_base::_tdSymbols::_tpList lst;
	lst.push_back("Units");
	lst.push_back("Constants");
	lst.push_back("Variables");
	_symbols.addSetSearch("Units", "Units", lst);

	lst.clear();
	lst.push_back("Constants");
	lst.push_back("Units");
	lst.push_back("Variables");
	_symbols.addSetSearch("Constants", "Constants", lst);

	lst.clear();
	lst.push_back("Variables");
	lst.push_back("Constants");
	lst.push_back("Units");
	_symbols.addSetSearch("Variables", "Variables", lst);
}



template <class _TVALUE>
calculator<_TVALUE>::~calculator()
{
}


}
#pragma once

#include "pmb_parser_algorithm.h"


namespace pmb
{



template <class _TVALUE>
class calculator: public parser::algorithm<_TVALUE>
{
public:
	typedef parser::algorithm<_TVALUE> _base;
	typedef typename _base::_tdSymbols _tdSymbols;
	typedef typename _base::_tdOprTable _tdOprTable;

public:
	calculator(_tdSymbols& symbols);
	~calculator();
};



}
#pragma once

#include "pmb_parser_algorithm.h"


namespace pmb
{



template <class _TVALUE>
class calculator: public parser::algorithm<_TVALUE>
{
public:
	typedef double value;
	typedef parser::number<value> number;
	typedef parser::value<number> container;
	typedef parser::algorithm<container> _base;

public:
	calculator(_base::_tdSymbols& symbols);
	~calculator();
};



}
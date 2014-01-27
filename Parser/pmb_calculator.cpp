#include "StdAfx.h"
#include "pmb_calculator.h"
#include "pmb_parser_algorithm.cpp"

#include "pmb_parser_symbol.cpp"

namespace pmb
{


calculator::calculator(_base::_tdSymbols& symbols)
	: _base(symbols)
{
}


calculator::~calculator()
{
}


}
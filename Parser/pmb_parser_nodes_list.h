#pragma once


#include "pmb_parser_nodes_calc.h"



namespace pmb
{
namespace parser
{
namespace nodes
{




template<class _TVALUE>
class list: public calc<_TVALUE>
{
public:
	list(int ini);
	~list();


	const _TVALUE& getLValue() const;
	_TVALUE& getLValue();
	const _TVALUE& getRValue() const;
	_TVALUE& getRValue();

protected:
	_TVALUE _lValue;
	_TVALUE _rValue;
};





}
}
}
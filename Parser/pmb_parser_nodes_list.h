#pragma once


#include "pmb_parser_nodes_unknow.h"



namespace pmb
{
namespace parser
{
namespace nodes
{




template<class _TVALUE>
class list: public unknow<_TVALUE>
{
public:
	typename typedef unknow<_TVALUE> _classbase;

public:
	list(int ini);
	~list();

	const _TVALUE& getLValue() const;
	_TVALUE& getLValue();
	const _TVALUE* getRValue() const;
	_TVALUE* getRValue();

	void updateNext();

protected:
	_TVALUE _rValue;
};





}
}
}
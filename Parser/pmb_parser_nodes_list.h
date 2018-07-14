#pragma once


#include "pmb_parser_nodes_unknow.h"



namespace pmb
{
namespace parser
{
namespace nodes
{




template<class _ITEM, class _NDTYPE>
class list : public unknown<_ITEM, _NDTYPE>
{
public:
	typename typedef unknown<_ITEM, _NDTYPE> cunknow;

public:
	list(int ini);
	~list();

//	const _TVALUE& getLValue() const;
//	_TVALUE& getLValue();
//	const _TVALUE* getRValue() const;
//	_TVALUE* getRValue();

	void updateNext();
};





}
}
}
#include "StdAfx.h"
#include "pmb_parser_nodes_list.h"


namespace pmb
{
namespace parser
{
namespace nodes
{


template<class _TVALUE>
list<_TVALUE>::list(int ini)
	: calc(ndList, ini - 1, ini)
{
}


template<class _TVALUE>
list<_TVALUE>::~list()
{
}


template<class _TVALUE>
_TVALUE& list<_TVALUE>::getLValue()
{
	return _lValue;
}

template<class _TVALUE>
const _TVALUE& list<_TVALUE>::getLValue() const
{
	return _lValue;
}


template<class _TVALUE>
_TVALUE& list<_TVALUE>::getRValue()
{
	return _rValue;
}

template<class _TVALUE>
const _TVALUE& list<_TVALUE>::getRValue() const
{
	return _rValue;
}


}
}
}
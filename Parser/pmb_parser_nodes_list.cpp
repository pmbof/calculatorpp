#include "StdAfx.h"
#include "pmb_parser_nodes_list.h"


namespace pmb
{
namespace parser
{
namespace nodes
{


template<class _ITEM, class _NDTYPE>
list<_ITEM, _NDTYPE>::list(int ini)
	: unknown(ini - 1, ini)
{
	_type = ndList;
///	_TVALUE::setNext(&_rValue);
}


template<class _ITEM, class _NDTYPE>
list<_ITEM, _NDTYPE>::~list()
{
}

/*
template<class _ITEM, class _NDTYPE>
_TVALUE& list<_ITEM, _NDTYPE>::getLValue()
{
	return cunknow::getValue();
}

template<class _ITEM, class _NDTYPE>
const _TVALUE& list<_ITEM, _NDTYPE>::getLValue() const
{
	return cunknow::getValue();
}


template<class _ITEM, class _NDTYPE>
_TVALUE* list<_ITEM, _NDTYPE>::getRValue()
{
	return &_rValue;
}

template<class _ITEM, class _NDTYPE>
const _TVALUE* list<_ITEM, _NDTYPE>::getRValue() const
{
	return &_rValue;
}
*/

template<class _ITEM, class _NDTYPE>
void list<_ITEM, _NDTYPE>::updateNext()
{
	if (_right)
	{
		if (_right->getType() == ndUnknown)
			setNext(static_cast<cunknow*>(_right));
		else if (_right->getType() == ndList)
			setNext(static_cast<list<_ITEM, _NDTYPE>*>(_right));
	}
}


}
}
}
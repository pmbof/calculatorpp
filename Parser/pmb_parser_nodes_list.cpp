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
	: unknow(ini - 1, ini)
{
	_type = ndList;
	_fnc = NULL;
	_TVALUE::setNext(&_rValue);
}


template<class _TVALUE>
list<_TVALUE>::~list()
{
}


template<class _TVALUE>
_TVALUE& list<_TVALUE>::getLValue()
{
	return _classbase::getValue();
}

template<class _TVALUE>
const _TVALUE& list<_TVALUE>::getLValue() const
{
	return _classbase::getValue();
}


template<class _TVALUE>
_TVALUE* list<_TVALUE>::getRValue()
{
	return &_rValue;
}

template<class _TVALUE>
const _TVALUE* list<_TVALUE>::getRValue() const
{
	return &_rValue;
}

template<class _TVALUE>
void list<_TVALUE>::updateNext()
{
	if (_right)
	{
		if (_right->getType() == ndUnknow)
			setNext(static_cast<unknow<_TVALUE>*>(_right));
		else if (_right->getType() == ndList)
			setNext(static_cast<list<_TVALUE>*>(_right));
	}
}


}
}
}
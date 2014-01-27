#include "StdAfx.h"
#include "pmb_parser_symbol.h"


namespace pmb
{
namespace parser
{


template<class _TVALUE, class _MAP>
symbol<_TVALUE, _MAP>::symbol(int nMaps)
	: _nMaps(nMaps)
{
	_map = new _MAP[_nMaps];
}


template<class _TVALUE, class _MAP>
symbol<_TVALUE, _MAP>::~symbol()
{
	delete [] _map;
}


template<class _TVALUE, class _MAP>
void symbol<_TVALUE, _MAP>::get(_TVALUE& value, bool canCreate)
{
	for(int j = 0; j < _nMaps; ++j)
	{
		_MAP::const_iterator i = _map[j].find(nd->getString(_expr));
		if(i != _variable.end())
		{
			value = const_cast<_TVALUE&>(i->second);
			return;
		}
	}
	if(canCreate)
		value = _map[0][nd->getString(_expr).getString()];
}




}
}
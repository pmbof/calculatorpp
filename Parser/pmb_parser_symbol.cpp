#include "StdAfx.h"
#include "pmb_parser_symbol.h"


namespace pmb
{
namespace parser
{



template<class _TVALUE, class _ITSTRING, class _MAP>
symbol<_TVALUE, _ITSTRING, _MAP>::symbol(int nMaps)
	: _nMaps(nMaps)
{
	_map = new _MAP[_nMaps];
}


template<class _TVALUE, class _ITSTRING, class _MAP>
symbol<_TVALUE, _ITSTRING, _MAP>::~symbol()
{
	delete [] _map;
}


template<class _TVALUE, class _ITSTRING, class _MAP>
void symbol<_TVALUE, _ITSTRING, _MAP>::find(const _ITSTRING& symbol, _TVALUE& value, bool canCreate)
{
	for(int j = 0; j < _nMaps; ++j)
	{
		_MAP::const_iterator i = _map[j].find(symbol);
		if(i != _map[j].end())
		{
			value = const_cast<_TVALUE&>(i->second);
			return;
		}
	}
	if(canCreate)
		value = _map[0][symbol.getString()];
}



template<class _TVALUE, class _ITSTRING, class _MAP>
const _MAP* symbol<_TVALUE, _ITSTRING, _MAP>::get(int index) const
{
	return _map + index;
}



}
}
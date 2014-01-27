#pragma once


#include "pmb_util_map.h"


namespace pmb
{
namespace parser
{


template<class _TVALUE, class _ITSTRING = item::string, class _MAP = util::map<std::string, _TVALUE, _ITSTRING> >
class symbol
{
public:
	typedef _MAP _tpMap;


public:
	symbol(int nMaps);
	~symbol();

	void find(const _ITSTRING& symbol, _TVALUE& value, bool canCreate);

	const _MAP* get(int index) const;

protected:
	_MAP* _map;
	int _nMaps;
};



}
}
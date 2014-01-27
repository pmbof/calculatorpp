#pragma once


#include "pmb_util_map.h"


namespace pmb
{
namespace parser
{


template<class _TVALUE, class _MAP = util::map<std::string, _TVALUE, item::string> >
class symbol
{
public:
	symbol(int nMaps);
	~symbol();

	void get(_TVALUE& value, bool canCreate);

protected:
	_MAP* _map;
	int _nMaps;
};



}
}
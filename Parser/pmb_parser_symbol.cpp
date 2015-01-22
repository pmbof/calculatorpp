#include "StdAfx.h"
#include "pmb_parser_symbol.h"


namespace pmb
{
namespace parser
{



template<class _TVALUE, class _ITSTRING, class _MAP>
symbol<_TVALUE, _ITSTRING, _MAP>::symbol()
	: _defaultInsert(NULL), _defaultSearch(NULL)
{
}


template<class _TVALUE, class _ITSTRING, class _MAP>
symbol<_TVALUE, _ITSTRING, _MAP>::~symbol()
{
	for(_tpMMap::const_iterator it = _map.begin(); it != _map.end(); ++it)
		delete it->second;
}


template<class _TVALUE, class _ITSTRING, class _MAP>
void symbol<_TVALUE, _ITSTRING, _MAP>::addSetVariable(const std::string& set)
{
	_tpMMap::const_iterator it = _map.find(set);
	if(it == _map.end())
	{
		_tpMap* nMap = new _tpMap;
		if(!_defaultInsert)
			_defaultInsert = nMap;
		_map[set] = nMap;
	}
}


template<class _TVALUE, class _ITSTRING, class _MAP>
void symbol<_TVALUE, _ITSTRING, _MAP>::addSetSearch(const std::string& set, const std::string& defaultInsert, const _tpList& lst)
{
	_tpLstMap searchlst;
	for(_tpList::const_iterator it = lst.begin(); it != lst.end(); ++it)
	{
		_tpMMap::const_iterator m = _map.find(*it);
		if(m != _map.end())
			searchlst.push_back(m->second);
	}
	_tpMMap::const_iterator d = _map.find(defaultInsert);
	_MAP* mDef = d != _map.end() ? d->second: NULL;
	if(searchlst.size())
		_setSearch[set] = _tpPairDef(mDef, searchlst);
	else if(mDef)
		_defaultInsert = mDef;
}



template<class _TVALUE, class _ITSTRING, class _MAP>
void symbol<_TVALUE, _ITSTRING, _MAP>::selectSearch(const std::string& set) const
{
	_tpMMapSearch::const_iterator i = _setSearch.find(set);
	if(i != _setSearch.end())
	{
		_defaultInsert = i->second.first;
		_defaultSearch = &i->second.second;
	}
}



template<class _TVALUE, class _ITSTRING, class _MAP>
void symbol<_TVALUE, _ITSTRING, _MAP>::find(const _ITSTRING& symbol, _TVALUE& value, bool canCreate)
{
	if(!_defaultSearch)
	{
		for(_tpMMap::const_iterator it = _map.begin(); it != _map.end(); ++it)
		{
			_MAP::const_iterator i = it->second->find(symbol);
			if(i != it->second->end())
			{
				value = const_cast<_TVALUE&>(i->second);
				return;
			}
		}
	}
	else
	{
		for(_tpLstMap::const_iterator it = _defaultSearch->begin(); it != _defaultSearch->end(); ++it)
		{
			_MAP::const_iterator i = (*it)->find(symbol);
			if(i != (*it)->end())
			{
				value = const_cast<_TVALUE&>(i->second);
				return;
			}
		}
	}
	if(canCreate && _defaultInsert)
		value = (*_defaultInsert)[symbol.getString()];
}


template<class _TVALUE, class _ITSTRING, class _MAP>
bool symbol<_TVALUE, _ITSTRING, _MAP>::find(const _ITSTRING& symbol) const
{
	if(!_defaultSearch)
	{
		for(_tpMMap::const_iterator it = _map.begin(); it != _map.end(); ++it)
		{
			_MAP::const_iterator i = it->second->find(symbol);
			if(i != it->second->end())
				return true;
		}
	}
	else
	{
		for(_tpLstMap::const_iterator it = _defaultSearch->begin(); it != _defaultSearch->end(); ++it)
		{
			_MAP::const_iterator i = (*it)->find(symbol);
			if(i != (*it)->end())
				return true;
		}
	}
	return false;
}


template<class _TVALUE, class _ITSTRING, class _MAP>
const typename symbol<_TVALUE, _ITSTRING, _MAP>::_tpMMap* symbol<_TVALUE, _ITSTRING, _MAP>::get() const
{
	return &_map;
}




}
}
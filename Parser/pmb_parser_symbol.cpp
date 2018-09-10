#include "StdAfx.h"
#include "pmb_parser_symbol.h"


namespace pmb
{
namespace parser
{



template<class _TVALUE, class _ITSTRING, class _MAP>
inline symbol<_TVALUE, _ITSTRING, _MAP>::symbol()
	: _defaultInsert(nullptr), _defaultSearch(nullptr)
{
}


template<class _TVALUE, class _ITSTRING, class _MAP>
inline symbol<_TVALUE, _ITSTRING, _MAP>::~symbol()
{
	for(_tpMMap::const_iterator it = _map.begin(); it != _map.end(); ++it)
		delete it->second;
}


template<class _TVALUE, class _ITSTRING, class _MAP>
inline void symbol<_TVALUE, _ITSTRING, _MAP>::addSetVariable(const std::string& set)
{
	_tpMMap::const_iterator it = _map.find(set);
	if(it == _map.end())
	{
		_tpMap* nMap = new _tpMap;
		_map[set] = _defaultInsert = nMap;
	}
}


template<class _TVALUE, class _ITSTRING, class _MAP>
inline void symbol<_TVALUE, _ITSTRING, _MAP>::addSetVariable(const std::string& set, _tpMap* vars)
{
	_tpMMap::iterator it = _map.find(set);
	if (it == _map.end())
	{
		if (!_defaultInsert)
			_defaultInsert = vars;
		_map[set] = vars;
	}
	else
	{
		for (_tpMap::const_iterator v = vars->begin(); v != vars->end(); ++v)
		{
			_tpMap::iterator oldv = it->second->find(v->first);
			if (oldv != it->second->end())
			{
//				delete oldv->second;
			}
			//(*it->second)[v->first] = v->second;
		}
		delete it->second;
		it->second = vars;
	}
}


template<class _TVALUE, class _ITSTRING, class _MAP>
inline void symbol<_TVALUE, _ITSTRING, _MAP>::addSetSearch(const std::string& set, const std::string& defaultInsert, const _tpList& lst)
{
	_tpLstMap searchlst;
	for(_tpList::const_iterator it = lst.begin(); it != lst.end(); ++it)
	{
		_tpMMap::const_iterator m = _map.find(*it);
		if(m != _map.end())
			searchlst.push_back(m->second);
	}
	_tpMMap::const_iterator d = _map.find(defaultInsert);
	_MAP* mDef = d != _map.end() ? d->second: nullptr;
	if(searchlst.size())
		_setSearch[set] = _tpPairDef(mDef, searchlst);
	else if(mDef)
		_defaultInsert = mDef;
}



template<class _TVALUE, class _ITSTRING, class _MAP>
inline void symbol<_TVALUE, _ITSTRING, _MAP>::selectSearch(const std::string& set) const
{
	_tpMMapSearch::const_iterator i = _setSearch.find(set);
	if(i != _setSearch.end())
	{
		_defaultInsert = i->second.first;
		_defaultSearch = &i->second.second;
	}
}



template<class _TVALUE, class _ITSTRING, class _MAP>
inline bool symbol<_TVALUE, _ITSTRING, _MAP>::find(const _ITSTRING& symbol, _TVALUE& value, bool canCreate)
{
	if(!_defaultSearch)
	{
		for(_tpMMap::const_iterator it = _map.begin(); it != _map.end(); ++it)
		{
			_MAP::const_iterator i = it->second->find(symbol);
			if(i != it->second->end())
			{
				value = const_cast<_TVALUE&>(i->second);
				return true;
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
				return true;
			}
		}
	}
	bool vret = canCreate && _defaultInsert;
	if(vret)
		value = (*_defaultInsert)[symbol.getString()];
	return vret;
}


template<class _TVALUE, class _ITSTRING, class _MAP>
inline bool symbol<_TVALUE, _ITSTRING, _MAP>::find(const _ITSTRING& symbol) const
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
inline const typename symbol<_TVALUE, _ITSTRING, _MAP>::_tpMMap* symbol<_TVALUE, _ITSTRING, _MAP>::get() const
{
	return &_map;
}




}
}
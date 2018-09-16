#include "StdAfx.h"
#include "pmb_parser_symbol.h"


namespace pmb
{
namespace parser
{



template<class _TVALUE, class _ITSTRING, class _MAP>
inline symbol<_TVALUE, _ITSTRING, _MAP>::symbol()
	: _default_insert(nullptr), _default_search(nullptr)
{
}


template<class _TVALUE, class _ITSTRING, class _MAP>
inline symbol<_TVALUE, _ITSTRING, _MAP>::~symbol()
{
	for(_tpMMap::const_iterator it = _map.begin(); it != _map.end(); ++it)
		delete it->second;
}


template<class _TVALUE, class _ITSTRING, class _MAP>
inline void symbol<_TVALUE, _ITSTRING, _MAP>::add_set_variable(const std::string& set)
{
	_tpMMap::const_iterator it = _map.find(set);
	if(it == _map.end())
	{
		_tpMap* nMap = new _tpMap;
		_map[set] = _default_insert = nMap;
	}
}


template<class _TVALUE, class _ITSTRING, class _MAP>
inline void symbol<_TVALUE, _ITSTRING, _MAP>::add_set_variable(const std::string& set, _tpMap* vars)
{
	_tpMMap::iterator it = _map.find(set);
	if (it == _map.end())
	{
		if (!_defaultInsert)
			_default_insert = vars;
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
inline void symbol<_TVALUE, _ITSTRING, _MAP>::add_set_search(const std::string& set, const std::string& defaultInsert, const _tpList& lst)
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
		_set_search[set] = _tpPairDef(mDef, searchlst);
	else if(mDef)
		_default_insert = mDef;
}



template<class _TVALUE, class _ITSTRING, class _MAP>
inline void symbol<_TVALUE, _ITSTRING, _MAP>::select_search(const std::string& set) const
{
	_tpMMapSearch::const_iterator i = _setSearch.find(set);
	if(i != _setSearch.end())
	{
		_default_insert = i->second.first;
		_default_search = &i->second.second;
	}
}



template<class _TVALUE, class _ITSTRING, class _MAP>
inline bool symbol<_TVALUE, _ITSTRING, _MAP>::find(const _ITSTRING& symbol, _TVALUE& value, bool canCreate)
{
	if(!_default_search)
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
		for(_tpLstMap::const_iterator it = _default_search->begin(); it != _default_search->end(); ++it)
		{
			_MAP::const_iterator i = (*it)->find(symbol);
			if(i != (*it)->end())
			{
				value = const_cast<_TVALUE&>(i->second);
				return true;
			}
		}
	}
	bool vret = canCreate && _default_insert;
	if(vret)
		value = (*_default_insert)[symbol.getString()];
	return vret;
}


template<class _TVALUE, class _ITSTRING, class _MAP>
inline bool symbol<_TVALUE, _ITSTRING, _MAP>::find(const _ITSTRING& symbol) const
{
	if(!_default_search)
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
		for(_tpLstMap::const_iterator it = _default_search->begin(); it != _default_search->end(); ++it)
		{
			_MAP::const_iterator i = (*it)->find(symbol);
			if(i != (*it)->end())
				return true;
		}
	}
	return false;
}

template<class _TVALUE, class _ITSTRING, class _MAP>
bool symbol<_TVALUE, _ITSTRING, _MAP>::is_set_variable() const
{
	return _default_insert;
}


template<class _TVALUE, class _ITSTRING, class _MAP>
inline const typename symbol<_TVALUE, _ITSTRING, _MAP>::_tpMMap* symbol<_TVALUE, _ITSTRING, _MAP>::get() const
{
	return &_map;
}




}
}
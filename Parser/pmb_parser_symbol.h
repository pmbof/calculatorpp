#pragma once


#include "pmb_util_map.h"
#include <list>


namespace pmb
{
namespace parser
{


template<class _TVALUE, class _ITSTRING = item<char, short>::string, class _MAP = util::map<std::string, _TVALUE, _ITSTRING> >
class symbol
{
public:
	typedef typename _TVALUE::_TypeValue _TypeValue;
	typedef _MAP _tpMap;
	typedef std::map<std::string, _MAP*> _tpMMap;
	typedef std::list<_MAP*> _tpLstMap;
	typedef std::pair<_MAP*, _tpLstMap> _tpPairDef;
	typedef std::map<std::string, _tpPairDef> _tpMMapSearch;
	typedef std::list<std::string> _tpList;

public:
	symbol();
	~symbol();

	void add_set_variable(const std::string& set);
	void add_set_variable(const std::string& set, _tpMap* vars);
	void add_set_search(const std::string& set, const std::string& defaultInsert, const _tpList& lst);
	
	void select_search(const std::string& set) const;

	bool find(const _ITSTRING& symbol, _TVALUE& value, bool canCreate);
	bool find(const _ITSTRING& symbol) const;

	bool is_set_variable() const;

	const _tpMMap* get() const;

protected:
	_tpMMap _map;
	mutable _MAP* _default_insert;

	_tpMMapSearch _set_search;
	mutable const _tpLstMap* _default_search;
};



}
}
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
	typedef _MAP _tpMap;
	typedef std::map<std::string, _MAP*> _tpMMap;
	typedef std::list<_MAP*> _tpLstMap;
	typedef std::pair<_MAP*, _tpLstMap> _tpPairDef;
	typedef std::map<std::string, _tpPairDef> _tpMMapSearch;
	typedef std::list<std::string> _tpList;

public:
	symbol();
	~symbol();

	void addSetVariable(const std::string& set);
	void addSetVariable(const std::string& set, _tpMap* vars);
	void addSetSearch(const std::string& set, const std::string& defaultInsert, const _tpList& lst);
	
	void selectSearch(const std::string& set) const;

	bool find(const _ITSTRING& symbol, _TVALUE& value, bool canCreate);
	bool find(const _ITSTRING& symbol) const;

	const _tpMMap* get() const;

protected:
	_tpMMap _map;
	mutable _MAP* _defaultInsert;

	_tpMMapSearch _setSearch;
	mutable const _tpLstMap* _defaultSearch;
};



}
}
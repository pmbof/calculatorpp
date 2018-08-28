#pragma once

#include "pmb_parser_symbol.h"
#include "pmb_calculate_magnitude.h"


namespace pmb
{
namespace calculate
{
namespace units
{




template<typename _CHAR, typename _POWER>
class prefix
{
public:
	prefix(const _CHAR* symbol, const _CHAR* name, _POWER power);
	~prefix();

	bool operator<(const prefix& right) const;
	bool less(const _CHAR* right, unsigned int len) const;
	bool greater(const _CHAR* right, unsigned int len) const;
	bool less_name(const prefix& right) const;
	bool less_name(const _CHAR* right, unsigned int len) const;
	bool greater_name(const _CHAR* right, unsigned int len) const;

	short find(const _CHAR* u) const;
	short findName(const _CHAR* u) const;

	double getFactor(short base) const;

private:
	_CHAR* _symbol;
	_CHAR* _name;
	_POWER _power;
};









template<class _Ty, class _TyR>
struct less_object
{	// functor for operator<
	bool operator()(const _Ty& left, const _Ty& right) const
	{
		return *left < *right;
	}
	bool operator()(const _Ty& left, const _TyR& right) const
	{
		return left->less(right.str(), right.size());
	}
	bool operator()(const _TyR& left, const _Ty& right) const
	{
		return right->greater(left.str(), left.size());
	}
};



template<class _Ty, class _TyR>
struct less_object_name
{	// functor for operator<
	bool operator()(const _Ty& left, const _Ty& right) const
	{
		return left->less_name(*right);
	}
	bool operator()(const _Ty& left, const _TyR& right) const
	{
		return left->less_name(right.str(), right.size());
	}
	bool operator()(const _TyR& left, const _Ty& right) const
	{
		return right->greater_name(left.str(), left.size());
	}
};









template<typename _CHAR, typename _POWER, typename _BASE, class _ITSTRING = parser::item<char, short>::string,
	class _MAP = util::map<prefix<_CHAR, _POWER>*, bool, _ITSTRING, less_object<prefix<_CHAR, _POWER>*, _ITSTRING>>>
struct prefix_base : public _MAP
{
	typedef prefix<_CHAR, _POWER> prefix;
	typedef util::map<prefix*, bool, _ITSTRING, less_object_name<prefix*, _ITSTRING>> mapName;

public:
	prefix_base(const _BASE& inbase);
	~prefix_base();

	bool insert(const _CHAR* symbol, const _POWER& power, const _CHAR* name);

	_BASE base() const;

	typename mapName::const_iterator find_by_name(const _ITSTRING& sfind) const;
	typename mapName::const_iterator end_by_name() const;

protected:
	_BASE _base;

	mapName _byName;
};












template<class _DIMENSION, class _ITSTRING = parser::item<char, short>::string, 
		class _MAP = util::map<_DIMENSION*, bool, _ITSTRING, less_object<_DIMENSION*, _ITSTRING>>>
class map_dimension: public _MAP
{
public:
	typedef typename _DIMENSION tpDimension;
	typedef typename tpDimension::tpChar tpChar;
	typedef typename _ITSTRING tpItString;
	typedef typename _ITSTRING::tpSize tpSize;
	typedef typename _MAP base;
	typedef util::map<_DIMENSION*, bool, _ITSTRING, less_object_name<_DIMENSION*, _ITSTRING>> mapName;
public:
	~map_dimension();

	bool insert(const tpChar* symbol, const tpChar* name = nullptr);
	bool insert(const tpChar* symbol, tpSize len, const tpChar* name = nullptr);

	typename mapName::const_iterator find_by_name(const _ITSTRING& sfind) const;
	typename mapName::const_iterator end_by_name() const;

protected:
	mapName _byName;
};






template<typename _POWER, typename _BASE, class _TVALUE,
	class _ITSTRING = parser::item<char, short>::string, class _MAP = util::map<std::string, _TVALUE, _ITSTRING>>
class system: protected _MAP
{
public:
	typedef typename _TVALUE::tpvalue tpvalue;
	typedef typename tpvalue::unit tpUnit;
	typedef typename _ITSTRING::tpChar tpChar;
	typedef typename tpUnit::_tpInt tpInt;
	typedef typename tpUnit::dimension dimension;
	typedef typename tpUnit::ndim ndim;

	typedef map_dimension<dimension, _ITSTRING> map_dimension;

	typedef prefix_base<tpChar, _POWER, _BASE, _ITSTRING> prefix;
	typedef _MAP base;

public:
	system(const map_dimension* dim, const prefix* pPrefix);
	~system();


	void unit_definition(bool bDef = true);

	bool find(const _ITSTRING& symbol, _TVALUE& value, bool canCreate);


protected:
	const map_dimension* _dimension;
	const prefix* _prefix;
	base _by_name;
	bool _unit_def;
	bool _search_by_names;
};











template<typename _POWER, typename _BASE, class _TVALUE, 
	class _ITSTRING = parser::item<char, short>::string, class _MAP = util::map<std::string, _TVALUE, _ITSTRING>>
class symbol : public parser::symbol<_TVALUE, _ITSTRING, _MAP>
{
public:
	typedef _MAP _tpMap;

	typedef typename _ITSTRING::tpChar tpChar;
	typedef typename _TVALUE::tpvalue tpvalue;
	typedef typename tpvalue::unit unit;
	typedef typename unit::power_dimension power_dimension;
	typedef typename unit::dimension dimension;
	
	typedef system<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP> system;
	typedef typename system::map_dimension map_dimension;
	typedef typename system::prefix prefix_base;
	typedef std::map<std::string, prefix_base*> map_prefix;
	typedef std::map<std::string, system*> map_system;

public:


	symbol();
	~symbol();

	bool add_dimension(const tpChar* symb, const tpChar* name);
	bool add_prefix(const tpChar* name, prefix_base* pprefix);
	bool add_system(const tpChar* name, const tpChar* prefix = nullptr);

	bool set_system(const tpChar* name = nullptr);

	bool find(const _ITSTRING& symbol, _TVALUE& value, bool canCreate);
//	bool find(const _ITSTRING& symbol) const;
protected:
	map_dimension _dimension;
	map_prefix _mprefix;

	map_system _msystems;
	system* _default_system;
};









}
}
}

#include "pmb_calculate_symbol.hpp"

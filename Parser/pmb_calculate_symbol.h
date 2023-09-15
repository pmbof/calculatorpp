#pragma once

#include "pmb_parser_symbol.h"
#include "pmb_calculate_magnitude.h"
#include "pmb_calculate_sunitType.h"



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
	prefix(const _CHAR* symbol, const _CHAR* name, _POWER power, bool bShow);
	~prefix();

	const _CHAR* symbol() const;

	bool operator<(const prefix& right) const;
	bool less(const _CHAR* right, unsigned int len) const;
	bool greater(const _CHAR* right, unsigned int len) const;
	bool less_name(const prefix& right) const;
	bool less_name(const _CHAR* right, unsigned int len) const;
	bool greater_name(const _CHAR* right, unsigned int len) const;

	short find(const _CHAR* u) const;
	short findName(const _CHAR* u) const;

	double getFactor(short base) const;

	template<typename _N>
	void getFactor(_POWER pow, _N& result) const;

	template<>
	void getFactor(_POWER pow, double& result) const {
		result = ::pow(result, _power * pow);
	}

	bool show() const;

private:
	_CHAR* _symbol;
	_CHAR* _name;
	_POWER _power;
	bool _show;
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

	bool insert(const _CHAR* symbol, const _POWER& power, const _CHAR* name, bool bShow = true);

	_BASE base() const;

	template <typename _T>
	const prefix* find_prefix(const _T& val, _BASE pow) const;

	template<typename _N>
	void getFactor(const prefix* pr, _BASE pow, _N& result) const;

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
	map_dimension();
	~map_dimension();

	bool insert(const tpChar* symbol, const tpChar* name = nullptr);
	bool insert(const tpChar* symbol, tpSize len, const tpChar* name = nullptr);

	void finalize();

	typename mapName::const_iterator find_by_name(const _ITSTRING& sfind) const;
	typename mapName::const_iterator end_by_name() const;

	const tpDimension* get_dimension(unsigned char index) const;
	tpDimension** const vector_dimensions() const;

protected:
	mapName _byName;
	tpDimension** _vector;
};










template<typename _POWER, typename _BASE, class _TVALUE,
	class _ITSTRING = parser::item<char, short>::string, class _MAP = util::map<std::string, std::pair<_TVALUE, sunitType>, _ITSTRING>>
class system: protected _MAP
{
public:
	typedef typename _TVALUE::tpValue tpValue;
	typedef typename _TVALUE::_TypeValue _TypeValue;
	typedef typename _TVALUE::tpValue::_2TypeValue _2TypeValue;

	typedef typename tpValue::unit tpUnit;
	typedef typename _ITSTRING::tpChar tpChar;
	typedef typename tpUnit::_tpInt tpInt;
	typedef typename tpUnit::dimension dimension;
	typedef typename tpUnit::ndim ndim;

	typedef map_dimension<dimension, _ITSTRING> map_dimension;
	typedef typename map_dimension::tpDimension tpDimension;

	typedef prefix_base<tpChar, _POWER, _BASE, _ITSTRING> prefix;
	typedef _MAP base;
	typedef std::vector<std::string> vstring;


public:
	// For process units:
	struct s_powunit {
		s_powunit(typename base::const_iterator iu, const tpUnit& u, tpInt p, const system* sys);

		typename base::const_iterator iunit;
		tpUnit unit;
		tpInt pow;
		const system* psys;
	};


	struct s_unit {
		typedef std::vector<s_powunit> vs_powunit;

		s_unit(typename base::const_iterator iu, tpValue _unit, tpInt _pow, const system* sys);

		bool operator <(const s_unit& right) const;

		int nDimension() const;
		void insert();

		void set(typename base::const_iterator iu, tpValue& u, tpInt p, const system* sys);

		std::string str(std::string& prefix, bool bSearchPrefix);
		void calc(); 

		typename base::const_iterator iunit;
		tpValue unit;
		vs_powunit vunit;
		tpInt pow;
		tpInt ppow;
		_2TypeValue factor;
		const system* psys;
	};

	typedef std::vector<s_unit> vs_unit;



public:
	system(const map_dimension* dim, const prefix* pPrefix);
	~system();

	bool find(const _ITSTRING& symbol, _TVALUE& value, bool canCreate);

	bool add_by_name(const tpChar* name, const _TVALUE& val, sunitType automatic);

	const prefix* getPrefix() const;

	vstring& last_defined();

	bool value(const tpValue& refVal, vs_unit& result) const;

	const tpDimension** vector_dimensions(unsigned char& nDims) const;

protected:
	tpValue quotient(const tpValue& refVal, const tpValue& unit, tpInt pow) const;

protected:
	const map_dimension* _dimension;
	const prefix* _prefix;
	base _by_name;
	bool _search_by_names;

	vstring _last_defined;
};











template<typename _POWER, typename _BASE, class _TVALUE, 
	class _ITSTRING = parser::item<char, short>::string, class _MAP = util::map<std::string, _TVALUE, _ITSTRING>>
class symbol : public parser::symbol<_TVALUE, _ITSTRING, _MAP>
{
public:
	typedef _MAP _tpMap;

	typedef typename _ITSTRING::tpChar tpChar;
	typedef typename _TVALUE::tpValue tpValue;
	typedef typename tpValue::unit unit;
	typedef typename unit::power_dimension power_dimension;
	typedef typename unit::dimension dimension;
	
	typedef system<_POWER, _BASE, _TVALUE, _ITSTRING> system;
	typedef typename system::map_dimension map_dimension;
	typedef typename system::tpDimension tpDimension;
	typedef typename system::prefix prefix_base;
	typedef std::map<std::string, prefix_base*> map_prefix;
	typedef std::map<std::string, system*> map_system;

	typedef typename system::_2TypeValue _2TypeValue;

	typedef typename system::vs_unit vs_unit;
	struct sunit_find
	{
		enum sufresult: char
		{
			sufr_dimensionless		= 'u',
			sufr_dimensionMatch		= 'M',
			sufr_dimensionNotMatch	= 'n'
		};

		sufresult res;
		vs_unit result;

		bool bFound;
		int nDims;
		int ifound;
		int nFoundDims;

		sunit_find();
		void set(const tpValue& refVal);
		bool algorithm();

		operator bool() const;
	};


	typedef std::list<std::string> slist;
	struct map_unit : std::map<std::string, slist> {
		map_unit() {}
		map_unit(const _TVALUE& val) : _val(val) {}

		_TVALUE _val;
	};
	typedef std::map<std::string, map_unit> ssmapb;

	typedef std::map<std::string, std::string> mapss;
	typedef std::map<std::string, mapss> mapmss;

public:
	symbol();
	~symbol();

	bool exists_dimension(const tpChar* symb) const;
	bool exists_prefix(const tpChar* name) const;
	bool exists_system(const tpChar* name) const;

	bool add_dimension(const tpChar* symb, const tpChar* name);
	void finalize_dimension();
	unsigned char get_dimension_index(const dimension* pDim) const;
	const dimension* get_dimension_by_index(unsigned char index) const;
	tpDimension** const vector_dimensions(unsigned char& size) const;


	bool add_prefix(const tpChar* name, prefix_base* pprefix);
	bool add_system(const tpChar* name, const tpChar* prefix = nullptr);

	bool set_system(const tpChar* name = nullptr);
	bool add_by_name(const tpChar* name, const _TVALUE& val, sunitType automatic, const tpChar* group = nullptr);

	bool set_default_system(const tpChar* name);

	bool set_system_constants(const tpChar* name = nullptr);
	bool add_constant(const tpChar* name);

	bool find(const _ITSTRING& symbol, _TVALUE& value, bool canCreate);

	bool defining_unit() const;

	bool value(const tpChar* path, const tpChar* varname, _TypeValue& val, std::string& suprefix, std::string& sunit, bool bPrefix = true) const;
	bool value(const tpValue& refVal, _TypeValue& val, std::string& suprefix, std::string& sunit, bool bPrefix = true) const;

	void values(const tpValue& refVal, _2TypeValue& val, _2TypeValue& factor) const;

protected:
	sunit_find values(const tpValue& refVal) const;

protected:
	map_dimension _dimension;
	map_prefix _mprefix;

	map_system _msystems;
	system* _define_system;
	system* _default_system;

	ssmapb _grp_unit;
	mapmss _constants;

	bool _save_last_define;
	std::list<std::string> _last_defined;

};











}
}
}

#include "pmb_calculate_symbol.hpp"

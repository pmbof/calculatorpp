#include "pmb_calculate_symbol.h"
#pragma once

#include <algorithm>
#include <cctype>
#include <iomanip>


namespace pmb
{
namespace calculate
{
namespace units
{



#ifdef _WINDOWS
	template<typename _CHAR>
	void strcpy(_CHAR* target, const _CHAR* source)
	{
		for (; *source; ++source)
			*target++ = *source;
		*target = '\0';
	}
#endif



template<typename _CHAR, typename _POWER>
inline prefix<_CHAR, _POWER>::prefix(const _CHAR* symbol, const _CHAR* name, _POWER power, bool bShow)
	: _power(power), _show(bShow)
{
	_symbol = new _CHAR[strlen(symbol) + 1];
	strcpy(_symbol, symbol);
	if (name)
	{
		_name = new _CHAR[strlen(name) + 1];
		strcpy(_name, name);
	}
	else
		_name = nullptr;
}


template<typename _CHAR, typename _POWER>
inline prefix<_CHAR, _POWER>::~prefix()
{
	delete[] _symbol;
	if (_name)
		delete[] _name;
}

template<typename _CHAR, typename _POWER>
inline const _CHAR * prefix<_CHAR, _POWER>::symbol() const
{
	return _symbol;
}


template<typename _CHAR, typename _POWER>
inline bool prefix<_CHAR, _POWER>::operator<(const prefix& right) const
{
	const _CHAR* l, *r;
	for (l = _symbol, r = right._symbol; *l && *r && *l == *r; ++l, ++r)
		;
	return !*l && *r || *l < *r;
}


template<typename _CHAR, typename _POWER>
inline bool prefix<_CHAR, _POWER>::less(const _CHAR* right, unsigned int len) const
{
	const _CHAR* l, *r;
	for (l = _symbol, r = right; *l && r - right < len && *l == *r; ++l, ++r)
		;
	return r - right < len && (!*l || *l < *r);
}


template<typename _CHAR, typename _POWER>
inline bool prefix<_CHAR, _POWER>::greater(const _CHAR* right, unsigned int len) const
{
	const _CHAR* l, *r;
	for (l = _symbol, r = right; *l && r - right < len && *l == *r; ++l, ++r)
		;
	return *l && len <= r - right || r - right < len  && *r < *l;
}


template<typename _CHAR, typename _POWER>
inline bool prefix<_CHAR, _POWER>::less_name(const prefix& right) const
{
	const _CHAR* l, *r;
	for (l = _name, r = right._name; l && r && *l && *r && *l == *r; ++l, ++r)
		;
	return !l && r || l && r && !*l && *r || *l < *r;
}


template<typename _CHAR, typename _POWER>
inline bool prefix<_CHAR, _POWER>::less_name(const _CHAR* right, unsigned int len) const
{
	const _CHAR* l, *r;
	for (l = _name, r = right; l && *l && r - right < len && *l == *r; ++l, ++r)
		;
	return !l || r - right < len && (!*l || *l < *r);
}


template<typename _CHAR, typename _POWER>
inline bool prefix<_CHAR, _POWER>::greater_name(const _CHAR* right, unsigned int len) const
{
	const _CHAR* l, *r;
	for (l = _name, r = right; l && *l && r - right < len && *l == *r; ++l, ++r)
		;
	return l && len && (*l && len <= r - right || r - right < len  && *r < *l);
}


template<typename _CHAR, typename _POWER>
inline short prefix<_CHAR, _POWER>::find(const _CHAR* str) const
{
	short i;
	for (i = 0; _symbol[i] && str[i] && str[i] == _symbol[i]; ++i)
		;
	return !_symbol[i] && str[i] ? i : -1;
}


template<typename _CHAR, typename _POWER>
inline short prefix<_CHAR, _POWER>::findName(const _CHAR* str) const
{
	short i;
	for (i = 0; _name[i] && str[i] && str[i] == _name[i]; ++i)
		;
	return !_name[i] && str[i] ? i : -1;
}


template<typename _CHAR, typename _POWER>
inline double prefix<_CHAR, _POWER>::getFactor(short base) const
{
	return ::pow((double)base, (double)_power);
}


template<typename _CHAR, typename _POWER>
template<typename _N>
inline void prefix<_CHAR, _POWER>::getFactor(_POWER pow, _N& result) const
{
	result = result.pow(_power * pow);
}


/*template<typename _CHAR, typename _POWER>
template<>
inline void prefix<_CHAR, _POWER>::getFactor(_POWER pow, double& result) const
{
	result = ::pow(result, _power * pow);
}//*/


template<typename _CHAR, typename _POWER>
inline bool prefix<_CHAR, _POWER>::show() const
{
	return _show;
}












template<typename _CHAR, typename _POWER, typename _BASE, class _ITSTRING, class _MAP>
inline prefix_base<_CHAR, _POWER, _BASE, _ITSTRING, _MAP>::prefix_base(const _BASE& inbase)
	: _base(inbase)
{
}


template<typename _CHAR, typename _POWER, typename _BASE, class _ITSTRING, class _MAP>
inline prefix_base<_CHAR, _POWER, _BASE, _ITSTRING, _MAP>::~prefix_base()
{
	for (const_iterator it = begin(); it != end(); ++it)
		delete it->first;
}


template<typename _CHAR, typename _POWER, typename _BASE, class _ITSTRING, class _MAP>
inline bool prefix_base<_CHAR, _POWER, _BASE, _ITSTRING, _MAP>::insert(const _CHAR* symbol, const _POWER& power, const _CHAR* name, bool bShow)
{
	_ITSTRING istring(symbol, strlen(symbol));
	const_iterator fs = find(istring);
	bool bRet = fs == end();
	if (bRet)
	{
		prefix* pNew = new prefix(symbol, name, power, bShow);
		(*this)[pNew] = false;
		if (name)
			_byName[pNew] = true;
	}
	return bRet;
}

template<typename _CHAR, typename _POWER, typename _BASE, class _ITSTRING, class _MAP>
inline _BASE prefix_base<_CHAR, _POWER, _BASE, _ITSTRING, _MAP>::base() const
{
	return _base;
}


template<typename _CHAR, typename _POWER, typename _BASE, class _ITSTRING, class _MAP>
template<typename _T>
inline const typename prefix_base<_CHAR, _POWER, _BASE, _ITSTRING, _MAP>::prefix*
prefix_base<_CHAR, _POWER, _BASE, _ITSTRING, _MAP>::find_prefix(const _T& val, _BASE pow) const
{
	const prefix* vret = nullptr;
	_T::_TypeValue last(0);
	for (const_iterator i = begin(); i != end(); ++i)
	{
		if (!i->first->show())
			continue;
		_T::_TypeValue f(_base);
		i->first->getFactor(pow, f);
		_T::_TypeValue zero(0);
		if (zero < val && last < f && f <= val || val < zero && f < last && val <= f)
		{
			last = f;
			vret = i->first;
		}
	}
	return vret;
}


template<typename _CHAR, typename _POWER, typename _BASE, class _ITSTRING, class _MAP>
template<typename _N>
inline void prefix_base<_CHAR, _POWER, _BASE, _ITSTRING, _MAP>::getFactor(const prefix* pr, _BASE pow, _N& result) const
{
	result = _base;
	pr->getFactor(pow, result);
}


template<typename _CHAR, typename _POWER, typename _BASE, class _ITSTRING, class _MAP>
inline typename prefix_base<_CHAR, _POWER, _BASE, _ITSTRING, _MAP>::mapName::const_iterator 
	prefix_base<_CHAR, _POWER, _BASE, _ITSTRING, _MAP>::find_by_name(const _ITSTRING& sfind) const
{
	return _byName.find(sfind);
}


template<typename _CHAR, typename _POWER, typename _BASE, class _ITSTRING, class _MAP>
inline typename prefix_base<_CHAR, _POWER, _BASE, _ITSTRING, _MAP>::mapName::const_iterator 
	prefix_base<_CHAR, _POWER, _BASE, _ITSTRING, _MAP>::end_by_name() const
{
	return _byName.end();
}




































template<class _DIMENSION, class _ITSTRING, class _MAP>
inline map_dimension<_DIMENSION, _ITSTRING, _MAP>::map_dimension()
	: _vector(nullptr)
{
}


template<class _DIMENSION, class _ITSTRING, class _MAP>
inline map_dimension<_DIMENSION, _ITSTRING, _MAP>::~map_dimension()
{
	if (_vector)
		delete[] _vector;

	for (base::const_iterator it = begin(); it != end(); ++it)
		delete it->first;
}


template<class _DIMENSION, class _ITSTRING, class _MAP>
inline bool map_dimension<_DIMENSION, _ITSTRING, _MAP>::insert(const tpChar* symbol, const tpChar* name)
{
	unsigned short len = strlen(symbol);
	return insert(symbol, len, name);
}



template<class _DIMENSION, class _ITSTRING, class _MAP>
inline bool map_dimension<_DIMENSION, _ITSTRING, _MAP>::insert(const tpChar* symbol, tpSize len, const tpChar* name)
{
	_ITSTRING istring(symbol, len);
	base::const_iterator fs = find(istring);
	bool bRet = fs == end();
	if (bRet)
	{
		tpSize nlen = name ? strlen(name) : 0;
		tpDimension* pNew = new tpDimension(symbol, len, name, nlen);
		(*this)[pNew] = true;
		if (name)
			_byName[pNew] = false;
	}
	return bRet;
}



template<class _DIMENSION, class _ITSTRING, class _MAP>
inline void map_dimension<_DIMENSION, _ITSTRING, _MAP>::finalize()
{
	if (_vector)
		delete[] _vector;

	if (0 < size())
	{
		_vector = new tpDimension * [size()];
		unsigned char i = 0;
		for (iterator it = begin(); it != end(); ++it, ++i)
		{
			it->first->set(i);
			_vector[i] = it->first;
		}
	}
	else
		_vector = nullptr;
}



template<class _DIMENSION, class _ITSTRING, class _MAP>
inline typename map_dimension<_DIMENSION, _ITSTRING, _MAP>::mapName::const_iterator 
	map_dimension<_DIMENSION, _ITSTRING, _MAP>::find_by_name(const _ITSTRING& sfind) const
{
	return _byName.find(sfind);
}



template<class _DIMENSION, class _ITSTRING, class _MAP>
inline typename map_dimension<_DIMENSION, _ITSTRING, _MAP>::mapName::const_iterator 
	map_dimension<_DIMENSION, _ITSTRING, _MAP>::end_by_name() const
{
	return _byName.end();
}





template<class _DIMENSION, class _ITSTRING, class _MAP>
inline const typename map_dimension<_DIMENSION, _ITSTRING, _MAP>::tpDimension* map_dimension<_DIMENSION, _ITSTRING, _MAP>::get_dimension(unsigned char index) const
{
	return _vector[index];
}



template<class _DIMENSION, class _ITSTRING, class _MAP>
inline typename map_dimension<_DIMENSION, _ITSTRING, _MAP>::tpDimension** const map_dimension<_DIMENSION, _ITSTRING, _MAP>::vector_dimensions() const
{
	return _vector;
}








































template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline system<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::system(const map_dimension* pDim, const prefix* pPrefix)
	: _dimension(pDim), _prefix(pPrefix), _search_by_names(true)
{
}


template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline system<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::~system()
{
}


template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline bool system<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::find(const _ITSTRING& symbol, _TVALUE& value, bool canCreate)
{
	const_iterator ui = _MAP::find(symbol);
	if (ui != end())
	{
		value = const_cast<_TVALUE&>(ui->second.first);
		return true;
	}
	if (_search_by_names)
	{
		ui = _by_name.find(symbol);
		if (ui != _by_name.end())
		{
			value = const_cast<_TVALUE&>(ui->second.first);
			return true;
		}
	}

	if (_prefix && 1 < symbol.size())
	{
		for (ui = begin(); ui != end(); ++ui)
		{
			if (symbol.size() <= ui->first.size())
				continue;
			_ITSTRING::tpSize p;
			for (p = 0; p < ui->first.size() && symbol[_ITSTRING::tpSize(symbol.size() - p - 1)] == ui->first[ui->first.size() - p - 1]; ++p)
				;
			if (p < ui->first.size())
				continue;
			_ITSTRING sprefix(symbol.str(), symbol.size() - p);
			prefix::const_iterator pf = _prefix->find(sprefix);
			if (pf == _prefix->end())
				continue;

			const typename _TVALUE::_TypeValue& number = ui->second.first->get_number();
			_TVALUE::_TypeValue::_TypeValue factor(pf->first->getFactor(_prefix->base()));
			const typename _TVALUE::_TypeUnit& unit = ui->second.first->get_unit();
			value = _TVALUE(new _TVALUE::tpValue(number * factor, unit));
			return true;
		}

		if (_search_by_names && 2 < symbol.size())
		{
			for (base::const_iterator ui = _by_name.begin(); ui != _by_name.end(); ++ui)
			{
				if (symbol.size() <= ui->first.size())
					continue;
				_ITSTRING::tpSize p;
				for (p = 0; p < ui->first.size() && symbol[_ITSTRING::tpSize(symbol.size() - p - 1)] == ui->first[ui->first.size() - p - 1]; ++p)
					;
				if (p < ui->first.size())
					continue;
				_ITSTRING sprefix(symbol.str(), symbol.size() - p);
				typename prefix::mapName::const_iterator pf = _prefix->find_by_name(sprefix);
				if (pf == _prefix->end_by_name())
					continue;

				const typename _TVALUE::_TypeValue& number = ui->second.first->get_number();
				_TVALUE::_TypeValue::_TypeValue factor(pf->first->getFactor(_prefix->base()));
				const typename _TVALUE::_TypeUnit& unit = ui->second.first->get_unit();
				value = _TVALUE(new _TVALUE::tpValue(number * factor, unit));
				return true;
			}
		}
	}

	if (canCreate)
	{
		_last_defined.push_back(symbol.getString());
		value = (*this)[symbol.getString()].first;
	}
	return canCreate;
}


template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline bool system<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::add_by_name(const tpChar* name, const _TVALUE& val, sunitType automatic)
{
	std::pair<_TVALUE, sunitType> p(const_cast<_TVALUE&>(val), automatic);
	_by_name.insert(base::value_type(std::string(name), p));
	for (int i = 0; i < _last_defined.size(); ++i)
		(*this)[_last_defined[i]].second = automatic;
	_last_defined.clear();
	return true;
}



template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline const typename system<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::prefix* system<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::getPrefix() const
{
	return _prefix;
}



template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline typename system<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::vstring&
	system<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::last_defined()
{
	return _last_defined;
}



template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline bool system<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::value(const tpValue& refVal, vs_unit& result) const
{
	bool bRet;
	int iUnDimession = -1;
	if (this)
	{
		for (base::const_iterator iu = begin(); iu != end(); ++iu)
		{
			if (iu->second.second == sutNoAutomaticPostfix || iu->second.second == sutNoAutomaticSuffix)
				continue;
			if (!*iu->second.first)
				continue;
			tpUnit::_tpInt pow = refVal.get_unit().compare(iu->second.first->get_unit());
			if (pow)
			{
				for (int i = 0; i < result.size(); ++i)
				{
					tpUnit::_tpInt pow2 = result[i].unit.get_unit().compare(iu->second.first->get_unit());
					if (pow2)
					{
						tpValue nunit = quotient(result[i].unit, **iu->second.first, pow2);
						result[i].set(iu, nunit, pow2, this);
						if (refVal.get_unit().nDims() <= result[i].nDimension())
							iUnDimession = i;
					}
				}
				tpValue nunit = quotient(refVal, **iu->second.first, pow);
				result.push_back(s_unit(iu, nunit, pow, this));
				if (pow == 1 && result.back().unit.dimensionless())
					break;
			}
		}
		bRet = result.size();
	}
	else
		bRet = false;

	return bRet;
}



template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
typename system<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::tpValue system<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::quotient(const tpValue& refVal, const tpValue& unit, tpInt pow) const
{
	return pow ==  1 ? refVal / unit :
		   pow == -1 ? refVal * unit :
		    1 < pow  ? refVal / unit.pow(pow) :
					   refVal * unit.pow(-pow);
}




template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline const typename system<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::tpDimension** system<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::vector_dimensions(unsigned char& nDims) const
{
	nDims = _dimensions->size();
	return _dimension->vector_dimensions();
}







































template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::symbol()
	: parser::symbol<_TVALUE, _ITSTRING, _MAP>(), _define_system(nullptr), _save_last_define(false), _default_system(nullptr)
{
}



template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::~symbol()
{
	for (map_system::const_iterator si = _msystems.begin(); si != _msystems.end(); ++si)
		delete si->second;

	for (map_prefix::const_iterator pi = _mprefix.begin(); pi != _mprefix.end(); ++pi)
		delete pi->second;
}



template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline bool symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::add_dimension(const tpChar* symb, const tpChar* name)
{
	return _dimension.insert(symb, name);
}



template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline void symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::finalize_dimension()
{
	_dimension.finalize();
}



template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
unsigned char symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::get_dimension_index(const dimension* pDim) const
{
	return _dimension.find(pDim)->second;
}



template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
const typename symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::dimension* symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::get_dimension_by_index(unsigned char index) const
{
	return _dimension.get_dimension(index);
}



template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
typename symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::tpDimension** const symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::vector_dimensions(unsigned char& size) const
{
	size = _dimension.size();
	return _dimension.vector_dimensions();
}




template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline bool symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::exists_prefix(const tpChar* name) const
{
	return _mprefix.find(name) != _mprefix.end();
}



template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline bool symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::add_prefix(const tpChar* name, prefix_base* pprefix)
{
	map_prefix::iterator pr = _mprefix.find(name);
	if (pr != _mprefix.end())
	{
		log::instance()->trace(logWarning, "Adding prefix %s that alright exists!\n", name);
		delete pr->second;
		pr->second = pprefix;
	}
	else
		_mprefix[name] = pprefix;
	return pr == _mprefix.end();
}



template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline bool symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::exists_system(const tpChar* name) const
{
	return _msystems.find(name) != _msystems.end();
}



template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline bool symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::add_system(const tpChar* name, const tpChar* prefix)
{
	map_system::const_iterator si = _msystems.find(name);
	if (si != _msystems.end())
	{
		log::instance()->trace(logWarning, "System %s that alright exists!\n", name);
		delete si->second;
	}

	bool bRet = true;
	const prefix_base* pb;
	if (prefix)
	{
		map_prefix::const_iterator ipr = _mprefix.find(prefix);
		pb = ipr == _mprefix.end() ? nullptr : ipr->second;
		if (ipr == _mprefix.end())
			bRet = false;
	}
	else
		pb = nullptr;

	_msystems[name] = new system(&_dimension, pb);
	return bRet;
}



template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline bool symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::set_system(const tpChar* name)
{
	if (!name)
	{
		_define_system = nullptr;
		return false;
	}
	map_system::const_iterator is = _msystems.find(name);
	return _define_system = is != _msystems.end() ? is->second : nullptr;
}



template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline bool symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::add_by_name(const tpChar* name, const _TVALUE& val, sunitType automatic, const tpChar* group)
{
	if (!_define_system)
		return false;

	std::string sname(name ? name : "");
	system::vstring last_defined = _define_system->last_defined();
	bool bRet = _define_system->add_by_name(sname.c_str(), val, automatic);
	if (bRet)
	{
		if (group)
		{
			std::stringstream ss(group);
			std::string item;
			while (std::getline(ss, item, ','))
			{
				item.erase(item.begin(), std::find_if(item.begin(), item.end(), [](int ch) {
						return !std::isspace(ch);
					}));
				item.erase(std::find_if(item.rbegin(), item.rend(), [](int ch) {
						return !std::isspace(ch);
					}).base(), item.end());
				ssmapb::const_iterator ci = _grp_unit.find(item);
				if (ci == _grp_unit.end())
					_grp_unit.insert(ssmapb::value_type(item, const_cast<_TVALUE&>(val)));
				else if (!(**(ci->second._val)).equal_unit(**val))
					return false; // invalid unit
				_grp_unit[item][sname].insert(_grp_unit[item][sname].end(), last_defined.begin(), last_defined.end());
			}
		}
		else
		{
			if ((*val)->one_dimension())
			{
				std::string item((**val).get_unit()[0]->name(), (**val).get_unit()[0]->name_size());
				ssmapb::const_iterator ci = _grp_unit.find(item);
				if (ci == _grp_unit.end())
					_grp_unit.insert(ssmapb::value_type(item, const_cast<_TVALUE&>(val)));
				_grp_unit[item][sname].insert(_grp_unit[item][sname].end(), last_defined.begin(), last_defined.end());
			}
			else if (!(*val)->dimensionless())
			{
				bRet = false;
				for (ssmapb::iterator ug = _grp_unit.begin(); ug != _grp_unit.end(); ++ug)
				{
					if ((*ug->second._val)->equal_unit(**val))
					{
						ug->second[sname].insert(ug->second[sname].end(), last_defined.begin(), last_defined.end());
						bRet = true;
					}
				}
			}
		}
	}
	return bRet;
}



template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline bool symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::set_default_system(const tpChar* name)
{
	if (name)
	{
		map_system::const_iterator cs = _msystems.find(name);
		if (cs != _msystems.end())
			_default_system = cs->second;
		else
			_default_system = nullptr;
	}
	else
		_default_system = nullptr;
	return _default_system;
}



template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline bool symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::set_system_constants(const tpChar* name)
{
	_last_defined.clear();
	if (_save_last_define = name)
		add_set_variable(name);
	return _save_last_define;
}



template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline bool symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::add_constant(const tpChar* name)
{
	if (!_default_insert || !name || !*name || !_save_last_define || _last_defined.empty())
		return false;
	bool bRet = !_last_defined.back().empty();
	if (bRet)
	{
		bRet = false;
		for (_tpMMap::const_iterator f = _map.begin(); f != _map.end(); ++f)
		{
			if (f->second == _default_insert)
			{
				bRet = true;
				while (!_last_defined.empty())
				{
					_constants[f->first][name] = _last_defined.back();
					_last_defined.pop_back();
				}
				break;
			}
		}
	}
	return bRet;
}



template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline bool symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::find(const _ITSTRING& symbol, _TVALUE& value, bool canCreate)
{
	if (_define_system)
	{
		if (_define_system->find(symbol, value, canCreate))
			return true;
	}

	if (!_default_search)
	{
		for (_tpMMap::const_iterator it = _map.begin(); it != _map.end(); ++it)
		{
			_MAP::const_iterator i = it->second->find(symbol);
			if (i != it->second->end())
			{
				value = const_cast<_TVALUE&>(i->second);
				return true;
			}
		}
	}
	else
	{
		for (_tpLstMap::const_iterator it = _default_search->begin(); it != _default_search->end(); ++it)
		{
			_MAP::const_iterator i = (*it)->find(symbol);
			if (i != (*it)->end())
			{
				value = const_cast<_TVALUE&>(i->second);
				return true;
			}
		}
	}

	if (!canCreate)
	{
		for (map_system::const_iterator is = _msystems.begin(); is != _msystems.end(); ++is)
		{
			if (is->second == _define_system)
				continue;
			if (is->second->find(symbol, value, false))
				return true;
		}
	}

	if (_define_system && !canCreate)
	{
		map_dimension::const_iterator di = _dimension.find(symbol);
		if (di != _dimension.end())
		{
			value = _TVALUE(new _TVALUE::tpValue(di->first));
			return true;
		}
		map_dimension::mapName::const_iterator din = _dimension.find_by_name(symbol);
		if (din != _dimension.end_by_name())
		{
			value = _TVALUE(new _TVALUE::tpValue(din->first));
			return true;
		}
	}

	bool vret = canCreate && _default_insert;
	if (vret)
	{
		if (_save_last_define)
			_last_defined.push_back(symbol.getString());
		value = (*_default_insert)[symbol.getString()];
	}
	return vret;
}



template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline bool symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::defining_unit() const
{
	return _define_system;
}



template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline bool symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::value(const tpChar* path, const tpChar* varname, _TypeValue& val, std::string& suprefix, std::string& units, bool bPrefix) const
{
	if (!varname)
		return false;
	if (!path)
	{
		if (_default_insert)
		{
			_tpMap::const_iterator iv = _default_insert->find(varname);
			if (iv != _default_insert->end())
				return value(**iv->second, val, suprefix, units);
		}
		for (_tpMMap::const_iterator im = _map.begin(); im != _map.end(); ++im)
		{
			if (im->second == _default_insert)
				continue;
			_tpMap::const_iterator iv = im->second->find(varname);
			if (iv != im->second->end())
				return value(**iv->second, val, suprefix, units);
		}
	}
	else
	{
		_tpMMap::const_iterator im = _map.find(path);
		if (im != _map.end())
		{
			_tpMap::const_iterator iv = im->second->find(varname);
			if (iv != im->second->end())
				return value(**iv->second, val, suprefix, units);
		}
	}
	return false;
}



template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline bool symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::value(const tpValue& refVal, _TypeValue& val, std::string& suprefix, std::string& sunit, bool bPrefix) const
{
	bool bRet;

	sunit_find search = values(refVal);
	switch (search.res)
	{
	case sunit_find::sufr_dimensionless:
		suprefix.clear();
		sunit.clear();
		val = refVal.get_number();
		bRet = false;
		break;

	case sunit_find::sufr_dimensionMatch:
		sunit = search.result[search.ifound].str(suprefix, bPrefix);
		val = search.result[search.ifound].unit.get_number() / search.result[search.ifound].factor;
		bRet = true;
		break;

	case sunit_find::sufr_dimensionNotMatch:
	default:
		suprefix.clear();
		sunit = refVal.get_unit().get_dimension();
		val = refVal.get_number();
		bRet = true;
		break;
	}

	return bRet;
}




template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline void symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::values(const tpValue& refVal, _2TypeValue& val, _2TypeValue& factor) const
{
	sunit_find search = values(refVal);
	switch (search.res)
	{
	case sunit_find::sufr_dimensionless:
		factor = 1;
		val = refVal.get_number();
		break;

	case sunit_find::sufr_dimensionMatch:
		search.result[search.ifound].calc();
		factor = search.result[search.ifound].factor;
		val = search.result[search.ifound].unit.get_number() / factor;
		break;

	case sunit_find::sufr_dimensionNotMatch:
	default:
		//sunit = refVal.get_unit().get_dimension();
		factor = 1;
		val = refVal.get_number();
		break;
	}
}






template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline typename symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::sunit_find
	symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::values(const tpValue& refVal) const
{
	sunit_find search;
	if (refVal.dimensionless())
		search.res = sunit_find::sufr_dimensionless;
	else
	{
		const system* syst = _define_system ? _define_system : _default_system;

		bool bRet = syst ? syst->value(refVal, search.result) : false;

		if (bRet || search.result.empty())
		{
			search.set(refVal);


			if (!search.algorithm())
			{
				for (map_system::const_iterator isyst = _msystems.begin(); isyst != _msystems.end(); ++isyst)
				{
					if (isyst->second == syst)
						continue;
					if (isyst->second->value(refVal, search.result))
					{
						int i;
						for (i = 0; i < search.result.size(); ++i)
						{
							int niDims = search.result[i].nDimension();
							if (!niDims)
								break;
						}
						if (i < search.result.size())
							break;
					}
				}
			}

			search.algorithm();
			search.res = sunit_find::sufr_dimensionMatch;
		}
	}

	return search;
}





template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::sunit_find::sunit_find()
	: res(sufr_dimensionNotMatch)
{
}



template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline void symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::sunit_find::set(const tpValue& refVal)
{
	bFound = false;
	nDims = refVal.get_unit().nDims();
	ifound = 0;
}



template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline bool symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::sunit_find::algorithm()
{
	nFoundDims = !bFound || result.empty() ? 0 : result[ifound].nDimension();
	for (int i = 0; i < result.size(); ++i)
	{
		if (i == ifound && bFound)
			continue;

		int niDims = result[i].nDimension();
		if ((nDims <= niDims || nFoundDims < niDims) && (!bFound || result[i] < result[ifound]) || nDims <= niDims && (!bFound || nFoundDims < niDims))
		{
			nFoundDims = result[ifound = i].nDimension();
			bFound = true;
		}
	}
	return nDims == nFoundDims;
}



template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::sunit_find::operator bool() const
{
	return bFound;
}








template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline system<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::s_powunit::s_powunit(typename base::const_iterator iu, const tpUnit& u, tpInt p, const system* sys)
	: iunit(iu), unit(u), pow(p), psys(sys)
{
}






template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline system<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::s_unit::s_unit(typename base::const_iterator iu, tpValue _unit, tpInt _pow, const system* sys)
	: iunit(iu), unit(_unit), ppow(1), pow(_pow), psys(sys)
{
}



template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline bool system<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::s_unit::operator <(const s_unit& right) const
{
	return unit.get_unit().nDims() == right.unit.get_unit().nDims()
		&& (vunit.size() < right.vunit.size()
			|| vunit.size() == right.vunit.size() && (0 < ppow && ppow < right.ppow || ppow < 0 && right.ppow < ppow
				|| ppow == right.ppow && unit.get_number() < right.unit.get_number()))
		|| unit.get_unit().nDims() < right.unit.get_unit().nDims();
}


template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline int system<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::s_unit::nDimension() const
{
	int nDim = 0;
	for (int i = 0; i < vunit.size(); ++i)
		nDim += vunit[i].unit.nDims();
	nDim += unit.get_unit().nDims();
	return nDim;
}



template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline void system<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::s_unit::insert()
{
	vs_powunit::iterator i;
	for (i = vunit.begin(); i != vunit.end(); ++i)
	{
		if (0 < pow && pow < i->pow || pow < 0 && i->pow < pow)
			break;
	}
	vunit.insert(i, s_powunit(iunit, unit.get_unit(), pow, psys));
}



template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline void system<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::s_unit::set(typename base::const_iterator iu, tpValue& u, tpInt p, const system* sys)
{
	insert();
	iunit = iu;
	unit.copy_release(u);
	pow = p;
	ppow *= p;
	psys = sys;
}



template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline std::string system<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::s_unit::str(std::string& strPrefix, bool bSearchPrefix)
{
	bool bPrefix = !bSearchPrefix;

	insert();
	std::stringstream sprefix, spostfix;
	factor = 1;
	for (int i = 0; i < vunit.size(); ++i)
	{
		bool bPostfix = vunit[i].iunit->second.second == sutNoAutomaticPostfix || vunit[i].iunit->second.second == sutAutomaticPostfix;
		std::stringstream& ss = bPostfix ? spostfix : sprefix;
		if (vunit[i].pow < 0)
			ss << (bPostfix || !ss.str().empty() ? "/" : "1/");

		if (!bPrefix)
		{
			const prefix* prfx = vunit[i].psys->getPrefix();
			if (prfx)
			{
				const prefix::prefix* ppr = prfx->find_prefix(unit.get_number(), vunit[i].pow);
				if (ppr)
				{
					prfx->getFactor(ppr, vunit[i].pow, factor);
					ss << ppr->symbol();
					bPrefix = true;
				}
			}
		}
		ss << vunit[i].iunit->first;
		if (1 < vunit[i].pow || vunit[i].pow < -1)
		{
			ss << "^";
			if (vunit[i].pow < 0)
				ss << -vunit[i].pow;
			else
				ss << vunit[i].pow;
		}
	}

	strPrefix = sprefix.str();
	return spostfix.str();
}



template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline void system<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::s_unit::calc()
{
	insert();
	factor = 1;
	for (int i = 0; i < vunit.size(); ++i)
	{
		const prefix* prfx = vunit[i].psys->getPrefix();
		if (prfx)
		{
			const prefix::prefix* ppr = prfx->find_prefix(unit.get_number(), vunit[i].pow);
			if (ppr)
			{
				prfx->getFactor(ppr, vunit[i].pow, factor);
				break;
			}
		}
	}
}






}
}
}

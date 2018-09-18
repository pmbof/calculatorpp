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
inline double prefix<_CHAR, _POWER>::getFactor(short base, _POWER pow) const
{
	return ::pow((double)base, (double)_power * pow);
}


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
	double last = 0;
	for (const_iterator i = begin(); i != end(); ++i)
	{
		if (!i->first->show())
			continue;
		double f = i->first->getFactor(_base, pow);
		if (0 < val && last < f && f <= val || val < 0 && f < last && val <= f)
		{
			last = f;
			vret = i->first;
		}
	}
	return vret;
}


template<typename _CHAR, typename _POWER, typename _BASE, class _ITSTRING, class _MAP>
inline double prefix_base<_CHAR, _POWER, _BASE, _ITSTRING, _MAP>::getFactor(const prefix* pr, _BASE pow) const
{
	return pr->getFactor(_base, pow);
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
inline map_dimension<_DIMENSION, _ITSTRING, _MAP>::~map_dimension()
{
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
inline bool map_dimension<_DIMENSION, _ITSTRING, _MAP>::insert(const tpChar* symbol, tpSize len, const tpChar* name = nullptr)
{
	_ITSTRING istring(symbol, len);
	base::const_iterator fs = find(istring);
	bool bRet = fs == end();
	if (bRet)
	{
		tpSize nlen = name ? strlen(name) : 0;
		tpDimension* pNew = new tpDimension(symbol, len, name, nlen);
		(*this)[pNew] = false;
		if (name)
			_byName[pNew] = true;
	}
	return bRet;
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
			value = _TVALUE(new _TVALUE::tpValue(*(ui->second.first)));
			(*value)->_number *= pf->first->getFactor(_prefix->base());
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
				value = _TVALUE(new _TVALUE::tpValue(*(ui->second.first)));
				(*value)->_number *= pf->first->getFactor(_prefix->base());
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
inline bool system<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::add_by_name(const tpChar* name, const _TVALUE& val, bool automatic)
{
	std::pair<_TVALUE, bool>p(const_cast<_TVALUE&>(val), automatic);
	_by_name.insert(base::value_type(std::string(name), p));
	for (int i = 0; i < _last_defined.size(); ++i)
		(*this)[_last_defined[i]].second = automatic;
	_last_defined.clear();
	return true;
}

template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline typename system<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::vstring&
	system<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::last_defined()
{
	return _last_defined;
}

template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline bool system<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::value(const _TVALUE& refVal, _TypeValue& val, std::string& sunit, bool bPrefix) const
{
	bool bRet;
	if (refVal->dimensionless())
	{
		sunit.clear();
		val = refVal->_number;
		bRet = false;
	}
	else
	{
		bRet = false;
		if (this)
		{
			struct s_powunit {
				s_powunit(const std::string& s, const tpUnit& u, tpUnit::_tpInt p)
					: symbol(s), unit(u), pow(p) { }

				std::string symbol;
				tpUnit unit;
				tpUnit::_tpInt pow;
			};

			struct s_unit {
				typedef std::vector<s_powunit> vector;

				s_unit(const std::string& s, const tpValue& _unit, tpUnit::_tpInt _pow)
					: symbol(s), unit(_unit), ppow(1), pow(_pow) { }

				bool operator <(const s_unit& right) const {
					return unit._unit.nDims() == right.unit._unit.nDims()
						&& (vunit.size() < right.vunit.size()
							|| vunit.size() == right.vunit.size() && (0 < ppow && ppow < right.ppow || ppow < 0 && right.ppow < ppow
								|| ppow == right.ppow && unit._number < right.unit._number))
						|| unit._unit.nDims() < right.unit._unit.nDims();
				}

				void insert() {
					vector::iterator i;
					for (i = vunit.begin(); i != vunit.end(); ++i)
					{
						if (0 < pow && pow < i->pow || pow < 0 && i->pow < pow)
							break;
					}
					vunit.insert(i, s_powunit(symbol, unit._unit, pow));
				}

				void set(const std::string& s, const tpValue& u, tpUnit::_tpInt p) {
					insert();
					symbol = s;
					unit = u;
					pow = p;
					ppow *= p;
				}

				std::string str(const prefix* pr) {
					insert();
					std::stringstream ss;
					factor = 1;
					for (int i = 0; i < vunit.size(); ++i)
					{
						if (vunit[i].pow < 0)
							ss << (i ? "/" : "1/");
						else if (i)
							ss << " ";
						if (!i && pr)
						{
							const prefix::prefix* ppr = pr->find_prefix(unit._number, vunit[i].pow);
							if (ppr)
							{
								factor = pr->getFactor(ppr, vunit[i].pow);
								ss << ppr->symbol();
							}
						}
						ss << vunit[i].symbol;
						if (1 < vunit[i].pow || vunit[i].pow < -1)
						{
							ss << "^";
							if (vunit[i].pow < 0)
								ss << -vunit[i].pow;
							else
								ss << vunit[i].pow;
						}
					}
					return ss.str();
				}

				std::string symbol;
				tpValue unit;
				vector vunit;
				tpUnit::_tpInt pow;
				tpUnit::_tpInt ppow;
				tpValue::_TypeValue factor;
			};

			std::vector<s_unit> vunit;
			for (base::const_iterator iu = begin(); iu != end(); ++iu)
			{
				if (!iu->second.second)
					continue;
				tpUnit::_tpInt pow = refVal->_unit.compare(iu->second.first->_unit);
				if (pow)
				{
					for (int i = 0; i < vunit.size(); ++i)
					{
						tpUnit::_tpInt pow2 = vunit[i].unit._unit.compare(iu->second.first->_unit);
						if (pow2)
							vunit[i].set(iu->first, vunit[i].unit / iu->second.first->pow(pow2), pow2);
					}
					vunit.push_back(s_unit(iu->first, **refVal / iu->second.first->pow(pow), pow));
					if (pow == 1 && vunit.back().unit.dimensionless())
						break;
				}
			}
			int min;
			for (int i = 0; i < vunit.size(); ++i)
			{
				if (!i)
					min = i;
				else if (vunit[i] < vunit[min])
					min = i;
			}
			if (bRet = vunit.size())
			{
				sunit = vunit[min].str(bPrefix ? _prefix : nullptr);
				val = vunit[min].unit._number / vunit[min].factor;
			}
		}

		if (!bRet)
		{
			sunit = refVal->_unit.get_dimension();
			val = refVal->_number;
		}
		bRet = true;
	}
	return bRet;
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
inline bool symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::add_prefix(const tpChar* name, prefix_base* pprefix)
{
	map_prefix::iterator pr = _mprefix.find(name);
	if (pr != _mprefix.end())
	{
		delete pr->second;
		pr->second = pprefix;
	}
	else
		_mprefix[name] = pprefix;
	return pr == _mprefix.end();
}


template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline bool symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::add_system(const tpChar* name, const tpChar* prefix)
{
	map_system::const_iterator si = _msystems.find(name);
	if (si != _msystems.end())
		delete si->second;

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
	_define_system = is != _msystems.end() ? is->second : nullptr;
}


template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline bool symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::add_by_name(const tpChar* name, const _TVALUE& val, bool automatic, const tpChar* group)
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
				else if ((**(ci->second._val))._unit != (**val)._unit)
					return false; // invalid unit
				_grp_unit[item][sname].insert(_grp_unit[item][sname].end(), last_defined.begin(), last_defined.end());
			}
		}
		else
		{
			if ((*val)->one_dimension())
			{
				std::string item((**val)._unit[0]->name(), (**val)._unit[0]->name_size());
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
					if ((*ug->second._val)->_unit == (*val)->_unit)
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

	for (map_system::const_iterator is = _msystems.begin(); is != _msystems.end(); ++is)
	{
		if (is->second == _define_system)
			continue;
		if (is->second->find(symbol, value, false))
			return true;
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
			value = _TVALUE(new _TVALUE::tpValue(din->second));
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
inline bool symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::value(const tpChar* path, const tpChar* varname, _TypeValue& val, std::string& units, bool bPrefix) const
{
	if (!varname)
		return false;
	if (!path)
	{
		if (_default_insert)
		{
			_tpMap::const_iterator iv = _default_insert->find(varname);
			if (iv != _default_insert->end())
				return value(iv->second, val, units);
		}
		for (_tpMMap::const_iterator im = _map.begin(); im != _map.end(); ++im)
		{
			if (im->second == _default_insert)
				continue;
			_tpMap::const_iterator iv = im->second->find(varname);
			if (iv != im->second->end())
				return value(iv->second, val, units);
		}
	}
	else
	{
		_tpMMap::const_iterator im = _map.find(path);
		if (im != _map.end())
		{
			_tpMap::const_iterator iv = im->second->find(varname);
			if (iv != im->second->end())
				return value(iv->second, val, units);
		}
	}
	return false;
}


template<typename _POWER, typename _BASE, class _TVALUE, class _ITSTRING, class _MAP>
inline bool symbol<_POWER, _BASE, _TVALUE, _ITSTRING, _MAP>::value(const _TVALUE& refVal, _TypeValue& val, std::string& sunit, bool bPrefix) const
{
	return _default_system->value(refVal, val, sunit, bPrefix);
}






}
}
}

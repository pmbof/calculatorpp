#include "StdAfx.h"
#include "pmb_parser_function.h"



namespace pmb
{
namespace parser
{


template<class _TVALUE>
function<_TVALUE>::function(const char* name, const char* description, tpFunc func, int nArgs)
	: _func(func), _nArgs(nArgs)
{
	_len = strlen(name);
	_name = new char[_len + 1];
	strcpy(_name, name);
	_description = new char[strlen(description) + 1];
	strcpy(_description, description);
}


template<class _TVALUE>
function<_TVALUE>::~function(void)
{
	delete []_name;
	delete []_description;
}



template<class _TVALUE>
bool function<_TVALUE>::compare(const char* expr, int len, int nArgs) const {
	return _len == len && _nArgs == nArgs && !strncmp(_name, expr, len);
}


template<class _TVALUE>
const char* function<_TVALUE>::getName() const
{
	return _name;
}


template<class _TVALUE>
const char* function<_TVALUE>::getDescription() const
{
	return _description;
}


template<class _TVALUE>
int function<_TVALUE>::getNArgs() const
{
	return _nArgs;
}



template<class _TVALUE>
void function<_TVALUE>::operator()(_TVALUE& result, int nArgs, const _TVALUE* vals) const {
	functions<_TVALUE> fnc;
	(fnc.*_func)(result, nArgs, vals);
}


template<class _TVALUE, class _ITEM, typename _NDTYPE>
pair_ffnc<_TVALUE, _ITEM, _NDTYPE>::pair_ffnc()
	: ptree(nullptr), pFunc(nullptr)
{
}

template<class _TVALUE, class _ITEM, typename _NDTYPE>
pair_ffnc<_TVALUE, _ITEM, _NDTYPE>::pair_ffnc(tptree* pTree)
	: ptree(pTree), pFunc(nullptr)
{
}

template<class _TVALUE, class _ITEM, typename _NDTYPE>
pair_ffnc<_TVALUE, _ITEM, _NDTYPE>::pair_ffnc(const function<_TVALUE>* pFnc)
	: pFunc(pFnc), ptree(nullptr)
{
}

template<class _TVALUE, class _ITEM, typename _NDTYPE>
bool pair_ffnc<_TVALUE, _ITEM, _NDTYPE>::operator!() const
{
	return !ptree && !pFunc;
}

template<class _TVALUE, class _ITEM, typename _NDTYPE>
pair_ffnc<_TVALUE, _ITEM, _NDTYPE>::operator bool () const
{
	return ptree || pFunc;
}


template<class _TVALUE, class _ITEM, typename _NDTYPE>
const function<_TVALUE> function_table<_TVALUE, _ITEM, _NDTYPE>::_fnc[] = {
	function<_TVALUE>("abs", "absolute value", &functions<_TVALUE>::abs),
	function<_TVALUE>("sgn", "sign", &functions<_TVALUE>::sgn),
	function<_TVALUE>("rnd", "random", &functions<_TVALUE>::rnd),
	function<_TVALUE>("ceil", "ceil", &functions<_TVALUE>::ceil),
	function<_TVALUE>("floor", "sign", &functions<_TVALUE>::floor),
	function<_TVALUE>("round", "round", &functions<_TVALUE>::round),

	function<_TVALUE>("lg", "decimal logarithm", &functions<_TVALUE>::lg),
	function<_TVALUE>("ln", "neperian logarithm", &functions<_TVALUE>::ln),
	function<_TVALUE>("log", "n-esim logarithm", &functions<_TVALUE>::log, 2),
	function<_TVALUE>("exp", "exponential", &functions<_TVALUE>::exp),

	function<_TVALUE>("sin", "sine", &functions<_TVALUE>::sin),
	function<_TVALUE>("cos", "cosine", &functions<_TVALUE>::cos),
	function<_TVALUE>("tg", "tangent", &functions<_TVALUE>::tg),
	function<_TVALUE>("sec", "secant", &functions<_TVALUE>::sec),
	function<_TVALUE>("cosec", "cosecant", &functions<_TVALUE>::cosec),
	function<_TVALUE>("cotg", "cotangent", &functions<_TVALUE>::cotg),
	function<_TVALUE>("asin", "arc sine", &functions<_TVALUE>::asin),
	function<_TVALUE>("asin", "arc sine", &functions<_TVALUE>::asin2, 2),
	function<_TVALUE>("acos", "arc cosine", &functions<_TVALUE>::acos),
	function<_TVALUE>("atg", "arc tangent", &functions<_TVALUE>::atg),
	function<_TVALUE>("asec", "arc secant", &functions<_TVALUE>::asec),
	function<_TVALUE>("acosec", "arc cosecant", &functions<_TVALUE>::acosec),
	function<_TVALUE>("acotg", "arc cotangent", &functions<_TVALUE>::acotg),

	function<_TVALUE>("if", "conditional function", &functions<_TVALUE>::_if, 3)
};

template<class _TVALUE, class _ITEM, typename _NDTYPE>
const int function_table<_TVALUE, _ITEM, _NDTYPE>::_fncSize = 23;


template<class _TVALUE, class _ITEM, typename _NDTYPE>
function_table<_TVALUE, _ITEM, _NDTYPE>::~function_table()
{
	for (mapfnc::const_iterator f = _functions.begin(); f != _functions.end(); ++f)
		for (map_nargs_pair_arg_tree::const_iterator fa = f->second.begin(); fa != f->second.end(); ++fa)
			delete fa->second.second;
}


template<class _TVALUE, class _ITEM, typename _NDTYPE>
int function_table<_TVALUE, _ITEM, _NDTYPE>::getPrecedence()
{
	return 100;
}


template<class _TVALUE, class _ITEM, typename _NDTYPE>
void function_table<_TVALUE, _ITEM, _NDTYPE>::insert(const sitem& fncName, const argmap& args, tptree* tree)
{
	_functions[fncName.getString()][args.size()] = pair_arg_tree(args, tree);
}


template<class _TVALUE, class _ITEM, typename _NDTYPE>
pair_ffnc<_TVALUE, _ITEM, _NDTYPE>
	function_table<_TVALUE, _ITEM, _NDTYPE>::find(const node<_ITEM, _NDTYPE>*nd, const char* expr, bool toRight) const
{
	const node<_ITEM, _NDTYPE>* chFnc = toRight ? nd->getLeft() : nd->getRight();
	if(chFnc)
	{
		int args = nd->nArguments(toRight);
		mapfnc::const_iterator cif = _functions.find(chFnc->getString(expr));
		if (cif != _functions.end())
		{
			map_nargs_pair_arg_tree::const_iterator ciff = cif->second.find(args);
			if (ciff != cif->second.end())
				return pair_ffnc(ciff->second.second);
		}
		for (int i = 0; i < _fncSize; ++i)
		{
			if (_fnc[i].compare(chFnc->getCharPtr(expr), chFnc->len(), args))
				return pair_ffnc(_fnc + i);
		}
	}
	return pair_ffnc();
}


template<class _TVALUE, class _ITEM, typename _NDTYPE>
const function<_TVALUE>* function_table<_TVALUE, _ITEM, _NDTYPE>::get(int i) const
{
	return _fnc + i;
}

template<class _TVALUE, class _ITEM, typename _NDTYPE>
int function_table<_TVALUE, _ITEM, _NDTYPE>::size() const
{
	return _fncSize;
}




}
}
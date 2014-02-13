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
const function<_TVALUE> function_table<_TVALUE>::_fnc[] = {
	function<_TVALUE>("abs", "absolute value", functions<_TVALUE>::abs),
	function<_TVALUE>("sgn", "sign", functions<_TVALUE>::sgn),
	function<_TVALUE>("rnd", "random", functions<_TVALUE>::rnd),
	function<_TVALUE>("ceil", "ceil", functions<_TVALUE>::ceil),
	function<_TVALUE>("floor", "sign", functions<_TVALUE>::floor),
	function<_TVALUE>("round", "round", functions<_TVALUE>::round),

	function<_TVALUE>("lg", "decimal logarithm", functions<_TVALUE>::lg),
	function<_TVALUE>("ln", "neperian logarithm", functions<_TVALUE>::ln),
	function<_TVALUE>("log", "n-esim logarithm", functions<_TVALUE>::log, 2),
	function<_TVALUE>("exp", "exponential", functions<_TVALUE>::exp),

	function<_TVALUE>("sin", "sine", functions<_TVALUE>::sin),
	function<_TVALUE>("cos", "cosine", functions<_TVALUE>::cos),
	function<_TVALUE>("tg", "tangent", functions<_TVALUE>::tg),
	function<_TVALUE>("sec", "secant", functions<_TVALUE>::sec),
	function<_TVALUE>("cosec", "cosecant", functions<_TVALUE>::cosec),
	function<_TVALUE>("cotg", "cotangent", functions<_TVALUE>::cotg),
	function<_TVALUE>("asin", "arc sine", functions<_TVALUE>::asin),
	function<_TVALUE>("acos", "arc cosine", functions<_TVALUE>::acos),
	function<_TVALUE>("atg", "arc tangent", functions<_TVALUE>::atg),
	function<_TVALUE>("asec", "arc secant", functions<_TVALUE>::asec),
	function<_TVALUE>("acosec", "arc cosecant", functions<_TVALUE>::acosec),
	function<_TVALUE>("acotg", "arc cotangent", functions<_TVALUE>::acotg),

	function<_TVALUE>("if", "conditional function", functions<_TVALUE>::_if, 3)
};

template<class _TVALUE>
const int function_table<_TVALUE>::_fncSize = 23;


template<class _TVALUE>
const function<_TVALUE>* function_table<_TVALUE>::find(const node<_TVALUE>*nd, const char* expr, bool toRight) const
{
	const node<_TVALUE>* chFnc = toRight ? nd->getLeft(): nd->getRight();
	if(chFnc)
	{
		for(int i = 0; i < _fncSize; ++i)
		{
			if(_fnc[i].compare(chFnc->getCharPtr(expr), chFnc->len(), nd->nArguments(toRight)))
				return _fnc + i;
		}
	}
	return NULL;
}


template<class _TVALUE>
const function<_TVALUE>* function_table<_TVALUE>::get(int i) const
{
	return _fnc + i;
}

template<class _TVALUE>
int function_table<_TVALUE>::size() const
{
	return _fncSize;
}




}
}
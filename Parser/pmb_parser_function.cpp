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



}
}
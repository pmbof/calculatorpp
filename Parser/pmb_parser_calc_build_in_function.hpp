#include "stdafx.h"
#pragma once

#include "pmb_parser_calc_build_in_function.h"


namespace pmb
{
namespace parser
{
namespace calc
{




template<class _TVARGS, class _CHAR, class _STRLEN>
inline build_in_function<_TVARGS, _CHAR, _STRLEN>::build_in_function(_CSTRING name, _CSTRING description, tpFunc ptrFnc, nargs nArgs)
	: _nArgs(nArgs), _ptrFnc(ptrFnc)
{
	_len = strlen(name);
	_STRLEN size = _len * sizeof(*name);
	_name = new _CHAR[size];
	memcpy(_name, name, size);

	_STRLEN szdescription = description ? strlen(description) : 0;
	if (0 < szdescription)
	{
		size = (szdescription + 1) * sizeof(*description);
		_description = new _CHAR[size];
		memcpy(_description, description, size - sizeof(*description));
		memset(_description + size - sizeof(*description), 0, sizeof(*description));
	}
	else
		_description = nullptr;
}

	// copia de class function file: pmb_parser_function.h
template<class _TVARGS, class _CHAR, class _STRLEN>
build_in_function<_TVARGS, _CHAR, _STRLEN>::build_in_function(_CSTRING name, _STRLEN szname, _CSTRING description, _STRLEN szdescription, tpFunc ptrFnc, nargs nArgs)
	: _len(szname), _ptrFnc(func), _nArgs(nArgs)
{
	_STRLEN size = _len * sizeof(*name);
	_name = (_STRING)new char[size];
	memcpy(_name, name, size);

	if (description && 0 < szdescription)
	{
		size = (szdescription + 1) * sizeof(*description);
		_description = (_STRING)new char[size];
		memcpy(_description, description, size - sizeof(*description));
		memset(_description + size - sizeof(*description), 0, sizeof(*description));
	}
	else
		_description = nullptr;
}


template<class _TVARGS, class _CHAR, class _STRLEN>
build_in_function<_TVARGS, _CHAR, _STRLEN>::~build_in_function()
{
	delete[] _name;
	if (_description)
	delete[] _description;
}



template<class _TVARGS, class _CHAR, class _STRLEN>
bool build_in_function<_TVARGS, _CHAR, _STRLEN>::compare(_CSTRING expr, _STRLEN len, nargs nArgs) const
{
	return _len == len && _nArgs == nArgs && !memcmp(_name, expr, _len);
}



template<class _TVARGS, class _CHAR, class _STRLEN>
inline typename build_in_function<_TVARGS, _CHAR, _STRLEN>::_CSTRING 
	build_in_function<_TVARGS, _CHAR, _STRLEN>::getName() const
{
	return _name;
}


template<class _TVARGS, class _CHAR, class _STRLEN>
inline typename build_in_function<_TVARGS, _CHAR, _STRLEN>::_CSTRING
	build_in_function<_TVARGS, _CHAR, _STRLEN>::getDescription() const
{
	return _description;
}


template<class _TVARGS, class _CHAR, class _STRLEN>
typename build_in_function<_TVARGS, _CHAR, _STRLEN>::nargs build_in_function<_TVARGS, _CHAR, _STRLEN>::getNArgs() const
{
	return _nArgs;
}


template<class _TVARGS, class _CHAR, class _STRLEN>
void build_in_function<_TVARGS, _CHAR, _STRLEN>::operator()(transporter_args& args) const
{
	if (args.nArgs() != _nArgs)
	{
		if (args.nArgs() < _nArgs)
			throw exception("too few arguments");
		else
			throw exception("too many arguments");
	}

	args.placeForResult();
	try
	{
		(*_ptrFnc)(args);
	}
	catch (const char* ex)
	{
		throw exception(ex);
	}
}







template<class _BUILD_IN_FUNCTION, class _SIZEFNCTBL>
inline build_in_function_table<_BUILD_IN_FUNCTION, _SIZEFNCTBL>::build_in_function_table(const build_in_function* fnc, _SIZEFNCTBL szfnc)
	: _fnc(fnc), _fncSize(szfnc)
{
}


template<class _BUILD_IN_FUNCTION, class _SIZEFNCTBL>
inline build_in_function_table<_BUILD_IN_FUNCTION, _SIZEFNCTBL>::~build_in_function_table()
{
}


template<class _BUILD_IN_FUNCTION, class _SIZEFNCTBL>
inline const typename build_in_function_table<_BUILD_IN_FUNCTION, _SIZEFNCTBL>::build_in_function*
	build_in_function_table<_BUILD_IN_FUNCTION, _SIZEFNCTBL>::find(_CSTRING str, _SZSTRING strlen, nargs nArgs) const
{
	for (_SIZEFNCTBL i = 0; i < _fncSize; ++i)
	{
		if (_fnc[i].compare(str, strlen, nArgs))
			return _fnc + i;
	}
	return nullptr;
}


template<class _BUILD_IN_FUNCTION, class _SIZEFNCTBL>
inline const typename build_in_function_table<_BUILD_IN_FUNCTION, _SIZEFNCTBL>::build_in_function*
	build_in_function_table<_BUILD_IN_FUNCTION, _SIZEFNCTBL>::get(_SIZEFNCTBL i) const
{
	return _fnc + i;
}

template<class _BUILD_IN_FUNCTION, class _SIZEFNCTBL>
inline _SIZEFNCTBL build_in_function_table<_BUILD_IN_FUNCTION, _SIZEFNCTBL>::size() const
{
	return _fncSize;
}






}
}
}
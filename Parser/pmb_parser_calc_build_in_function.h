#pragma once

#include <string>


namespace pmb
{
namespace parser
{
namespace calc
{




// copia de class function file: pmb_parser_function.h
template<class _TVARGS, class _CHAR, class _STRLEN>
class build_in_function
{
public:
	typename typedef _TVARGS transporter_args;
	typename typedef _CHAR* _PTRCHAR;
	typename typedef _STRLEN _SZSTRING;
	typename typedef transporter_args::nargs nargs;

	typedef typename build_in_function<_TVARGS, _CHAR, _STRLEN> _THISCLASS;
	typename typedef const _CHAR* _CSTRING;

	typedef void (*tpFunc)(transporter_args& args);

public:
	class exception
	{
	public:
		exception(const char* msg) : _msg(msg) {}

		const char* message() const { return _msg.c_str(); }
	protected:
		std::string _msg;
	};

public:
	build_in_function(_CSTRING name, _CSTRING description, tpFunc ptrFnc, nargs nArgs = 1);
	build_in_function(_CSTRING name, _STRLEN szname, _CSTRING description, _STRLEN szdescription, tpFunc ptrFnc, nargs nArgs = 1);
	~build_in_function();

	bool compare(_CSTRING expr, _STRLEN len, nargs nArgs) const;


	_CSTRING getName() const;
	_CSTRING getDescription() const;
	nargs getNArgs() const;

	void operator()(transporter_args& args) const;

private:
	_PTRCHAR _name;
	_STRLEN _len;
	_PTRCHAR _description;
	tpFunc _ptrFnc;
	nargs _nArgs;
};






template<class _BUILD_IN_FUNCTION, class _SIZEFNCTBL>
class build_in_function_table
{
public:
	typename typedef _BUILD_IN_FUNCTION::transporter_args transporter_args;
	typename typedef _BUILD_IN_FUNCTION::_PTRCHAR _PTRCHAR;
	typename typedef _BUILD_IN_FUNCTION::nargs nargs;
	typedef typename _BUILD_IN_FUNCTION::_CSTRING _CSTRING;
	typedef typename _BUILD_IN_FUNCTION::_SZSTRING _SZSTRING;
	typedef typename _BUILD_IN_FUNCTION build_in_function;

public:
	build_in_function_table(const build_in_function* fnc, _SIZEFNCTBL szfnc);
	~build_in_function_table();

	const build_in_function* find(_CSTRING str, _SZSTRING strlen, nargs nArgs) const;

	const build_in_function* get(_SIZEFNCTBL i) const;
	_SIZEFNCTBL size() const;

protected:
	const build_in_function* _fnc;
	_SIZEFNCTBL _fncSize;
};








}
}
}
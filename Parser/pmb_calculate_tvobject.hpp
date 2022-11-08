#pragma once

#include "pmb_calculate_tvobject.h"



namespace pmb
{
namespace calculate
{



template <class _NUMBER, class _COBJECT, class _STRING>
tvobject<_NUMBER, _COBJECT, _STRING>::tvobject()
	: _type(tvt_null)
{
}



template <class _NUMBER, class _COBJECT, class _STRING>
tvobject<_NUMBER, _COBJECT, _STRING>::tvobject(tvobject& src)
	: _type(src._type)
{
	_vnumeric = src._vnumeric;
	src._type = tvt_null;
}



template <class _NUMBER, class _COBJECT, class _STRING>
tvobject<_NUMBER, _COBJECT, _STRING>::tvobject(const _Number& src)
	: _type(tvt_numeric)
{
	_vnumeric = new _Number(src);

	log::instance()->beginFunction(logDebug, __FUNCTION__)->trace(logDebug, "new _vnumeric = 0x%08x\n", _vnumeric).endFunction();
}



template <class _NUMBER, class _COBJECT, class _STRING>
tvobject<_NUMBER, _COBJECT, _STRING>::tvobject(const _MyT* src)
	: _type(src->_type)
{
	switch (_type)
	{
	case tvt_null:
		break;
	case tvt_boolean:
		_vbool = src->_vbool;
		break;
	case tvt_numeric:
		_vnumeric = new _Number(src->_vnumeric);
		break;
	case tvt_string:
		_vstring = new std::string(src->_vstring);
		break;
	case tvt_object:
		_vobject = new _Cobject(src->_vobject);
	}
}



template <class _NUMBER, class _COBJECT, class _STRING>
tvobject<_NUMBER, _COBJECT, _STRING>::tvobject(const _TypeChar* str, const _TypeSzstr& len)
	: _type(tvt_numeric)
{
	_vnumeric = new _Number(str, len);
}



template <class _NUMBER, class _COBJECT, class _STRING>
tvobject<_NUMBER, _COBJECT, _STRING>::tvobject(const dimension* dim)
	: _type(tvt_numeric)
{
	_vnumeric = new _Number(dim);
}



template <class _NUMBER, class _COBJECT, class _STRING>
tvobject<_NUMBER, _COBJECT, _STRING>::tvobject(const _TypeValue& n, const unit& u)
	: _type(tvt_numeric)
{
	_vnumeric = new _Number(n, u);
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline tvobject<_NUMBER, _COBJECT, _STRING>::~tvobject()
{
	_destructor();
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::_destructor()
{
	switch (_type)
	{
	case tvt_numeric:
		delete _vnumeric;
		break;
	case tvt_string:
		delete _vstring;
		break;
	case tvt_object:
		delete _vobject;
		break;
	default:
		break;
	}
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::clear()
{
	_destructor();
	_type = tvt_null;
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline bool tvobject<_NUMBER, _COBJECT, _STRING>::is_null() const
{
	return _type == tvt_null;
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline bool tvobject<_NUMBER, _COBJECT, _STRING>::is_boolean() const
{
	return _type == tvt_boolean;
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline bool tvobject<_NUMBER, _COBJECT, _STRING>::is_numeric() const
{
	return _type == tvt_numeric;
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline bool tvobject<_NUMBER, _COBJECT, _STRING>::is_string() const
{
	return _type == tvt_string;
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline bool tvobject<_NUMBER, _COBJECT, _STRING>::is_object() const
{
	return _type == tvt_object;
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline bool tvobject<_NUMBER, _COBJECT, _STRING>::boolean() const
{
	if (_type != tvt_boolean)
		throw operation::exception("must be a boolean");
	return _vbool;
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline const _NUMBER* tvobject<_NUMBER, _COBJECT, _STRING>::numeric() const
{
	if (_type != tvt_numeric)
		throw operation::exception("must be a number");
	return _vnumeric;
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline _NUMBER* tvobject<_NUMBER, _COBJECT, _STRING>::numeric()
{
	if (_type != tvt_numeric)
		throw operation::exception("must be a number");
	return _vnumeric;
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline const _STRING* tvobject<_NUMBER, _COBJECT, _STRING>::string() const
{
	if (_type != tvt_string)
		throw operation::exception("must be a string");
	return _vstring;
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline _STRING* tvobject<_NUMBER, _COBJECT, _STRING>::string()
{
	if (_type != tvt_string)
		throw operation::exception("must be a string");
	return _vstring;
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline const _COBJECT* tvobject<_NUMBER, _COBJECT, _STRING>::object() const
{
	if (_type == tvt_object)
		throw operation::exception("must be a object");
	return _vobject;
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline _COBJECT* tvobject<_NUMBER, _COBJECT, _STRING>::object()
{
	if (_type == tvt_object)
		throw operation::exception("must be a object");
	return _vobject;
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline _NUMBER* tvobject<_NUMBER, _COBJECT, _STRING>::_numeric()
{
	if (_type != tvt_null)
		throw operation::exception("must be null");
	_type = tvt_numeric;
	_vnumeric = new _Number;
	return _vnumeric;
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::stringstream(std::stringstream& sr) const
{
	if (_type == tvt_null)
		sr << "<null>";
	else if (_type == tvt_boolean)
		sr << (_vbool ? "True" : "False");
	else if (_type == tvt_numeric)
		_vnumeric->stringstream(sr);
	else if (_type == tvt_string)
		sr << "\"" << *_vstring << "\"";
	else if (_type == tvt_object)
		sr << "<Object>";
	else
		sr << "<Unkown>";
}






// Operations:
template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::positive(const _MyT& right)
{
	const _Number& r = *right.numeric();
	_numeric()->positive(r);
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::negative(const _MyT& right)
{
	const _Number& r = *right.numeric();
	_numeric()->negative(r);
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::factorial(const _MyT& right)
{
	const _Number& r = *right.numeric();
	_numeric()->factorial(r);
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::multiplication(const _MyT& left, const _MyT& right)
{
	const _Number& l = *left.numeric();
	const _Number& r = *right.numeric();
	_numeric()->multiplication(l, r);
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::division(const _MyT& left, const _MyT& right)
{
	const _Number& l = *left.numeric();
	const _Number& r = *right.numeric();
	_numeric()->division(l, r);
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::modulo(const _MyT& left, const _MyT& right)
{
	const _Number& l = *left.numeric();
	const _Number& r = *right.numeric();
	_numeric()->modulo(l, r);
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::addition(const _MyT& left, const _MyT& right)
{
	const _Number& l = *left.numeric();
	const _Number& r = *right.numeric();
	_numeric()->addition(l, r);
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::substraction(const _MyT& left, const _MyT& right)
{
	const _Number& l = *left.numeric();
	const _Number& r = *right.numeric();
	_numeric()->substraction(l, r);
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::equal(const _MyT& left, const _MyT& right)
{
	clear();
	_type = tvt_boolean;

	if (left._type != right._type)
		_vbool = false;
	else if (left._type == tvt_numeric)
	{
		const _Number& l = *left.numeric();
		const _Number& r = *right.numeric();
		_vbool = l.equal(r);
	}
	else if (left._type == tvt_string)
		_vbool = *left._vstring == *right._vstring;
	else if (left._type == tvt_object)
		_vbool = false;
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::not_equal(const _MyT& left, const _MyT& right)
{
	clear();
	_type = tvt_boolean;

	if (left._type != right._type)
		_vbool = true;
	else if (left._type == tvt_numeric)
	{
		const _Number& l = *left.numeric();
		const _Number& r = *right.numeric();
		_vbool = !l.equal(r);
	}
	else if (left._type == tvt_string)
		_vbool = *left._vstring != *right._vstring;
	else if (left._type == tvt_object)
		_vbool = true;
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::less_equal(const _MyT& left, const _MyT& right)
{
	clear();
	_type = tvt_boolean;

	if (left._type != right._type)
		throw operation::exception("Must be some type");
	else if (left._type == tvt_numeric)
	{
		const _Number& l = *left.numeric();
		const _Number& r = *right.numeric();
		_vbool = l.less_equal(r);
	}
	else if (left._type == tvt_string)
		_vbool = *left._vstring <= *right._vstring;
	else if (left._type == tvt_object)
		_vbool = false;
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::less(const _MyT& left, const _MyT& right)
{
	clear();
	_type = tvt_boolean;

	if (left._type != right._type)
		throw operation::exception("Must be some type");
	else if (left._type == tvt_numeric)
	{
		const _Number& l = *left.numeric();
		const _Number& r = *right.numeric();
		_vbool = l.less(r);
	}
	else if (left._type == tvt_string)
		_vbool = *left._vstring < *right._vstring;
	else if (left._type == tvt_object)
		_vbool = false;
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::greater_equal(const _MyT& left, const _MyT& right)
{
	clear();
	_type = tvt_boolean;

	if (left._type != right._type)
		throw operation::exception("Must be some type");
	else if (left._type == tvt_numeric)
	{
		const _Number& l = *left.numeric();
		const _Number& r = *right.numeric();
		_vbool = l.greater_equal(r);
	}
	else if (left._type == tvt_string)
		_vbool = *left._vstring >= *right._vstring;
	else if (left._type == tvt_object)
		_vbool = false;
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::greater(const _MyT& left, const _MyT& right)
{
	clear();
	_type = tvt_boolean;

	if (left._type != right._type)
		throw operation::exception("Must be some type");
	else if (left._type == tvt_numeric)
	{
		const _Number& l = *left.numeric();
		const _Number& r = *right.numeric();
		_vbool = l.greater(r);
	}
	else if (left._type == tvt_string)
		_vbool = *left._vstring > *right._vstring;
	else if (left._type == tvt_object)
		_vbool = false;
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::not(const _MyT& right)
{
	clear();
	_type = tvt_boolean;

	if (_type != right._type)
		throw operation::exception("Must be boolean type");
	_vbool = !right._vbool;
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline bool tvobject<_NUMBER, _COBJECT, _STRING>::and_check(const _MyT& left)
{
	if (left._type != tvt_boolean)
		throw operation::exception("Must be boolean type");

	if (!left._vbool)
	{
		clear();
		_type = tvt_boolean;
		_vbool = false;
		return true;
	}
	return false;
}


template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::and(const _MyT& left, const _MyT& right)
{
	clear();
	_type = tvt_boolean;

	if (_type != left._type || _type != right._type)
		throw operation::exception("Must be boolean type");
	_vbool = left._vbool && right._vbool;
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline bool tvobject<_NUMBER, _COBJECT, _STRING>::nand_check(const _MyT& left)
{
	if (left._type != tvt_boolean)
		throw operation::exception("Must be boolean type");

	if (!left._vbool)
	{
		clear();
		_type = tvt_boolean;
		_vbool = true;
		return true;
	}
	return false;
}


template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::nand(const _MyT& left, const _MyT& right)
{
	clear();
	_type = tvt_boolean;

	if (_type != left._type || _type != right._type)
		throw operation::exception("Must be boolean type");
	_vbool = !(left._vbool && right._vbool);
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::xor(const _MyT& left, const _MyT& right)
{
	clear();
	_type = tvt_boolean;

	if (_type != left._type || _type != right._type)
		throw operation::exception("Must be boolean type");
	_vbool = left._vbool && !right._vbool || !left._vbool && right._vbool;
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline bool tvobject<_NUMBER, _COBJECT, _STRING>::or_check(const _MyT& left)
{
	if (left._type != tvt_boolean)
		throw operation::exception("Must be boolean type");
	if (left._vbool)
	{
		clear();
		_type = tvt_boolean;
		_vbool = true;
		return true;
	}
	return false;
}


template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::or(const _MyT& left, const _MyT& right)
{
	clear();
	_type = tvt_boolean;

	if (_type != left._type || _type != right._type)
		throw operation::exception("Must be boolean type");
	_vbool = left._vbool || right._vbool;
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline bool tvobject<_NUMBER, _COBJECT, _STRING>::nor_check(const _MyT& left)
{
	if (left._type != tvt_boolean)
		throw operation::exception("Must be boolean type");
	if (left._vbool)
	{
		clear();
		_type = tvt_boolean;
		_vbool = false;
		return true;
	}
	return false;
}


template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::nor(const _MyT & left, const _MyT & right)
{
	clear();
	_type = tvt_boolean;

	if (_type != left._type || _type != right._type)
		throw operation::exception("Must be boolean type");
	_vbool = left._vbool || right._vbool;
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline bool tvobject<_NUMBER, _COBJECT, _STRING>::implication_check(const _MyT& left)
{
	if (left._type != tvt_boolean)
		throw operation::exception("Must be boolean type");
	if (!left._vbool)
	{
		clear();
		_type = tvt_boolean;
		_vbool = true;
		return true;
	}
	return false;
}


template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::implication(const _MyT& left, const _MyT& right)
{
	clear();
	_type = tvt_boolean;

	if (_type != left._type || _type != right._type)
		throw operation::exception("Must be boolean type");
	_vbool = !left._vbool || left._vbool && right._vbool;
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline bool tvobject<_NUMBER, _COBJECT, _STRING>::implication_reverse_check(const _MyT& right)
{
	if (right._type != tvt_boolean)
		throw operation::exception("Must be boolean type");
	if (!right._vbool)
	{
		clear();
		_type = tvt_boolean;
		_vbool = true;
		return true;
	}
	return false;
}


template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::implication_reverse(const _MyT& left, const _MyT& right)
{
	clear();
	_type = tvt_boolean;

	if (_type != left._type || _type != right._type)
		throw operation::exception("Must be boolean type");
	_vbool = !right._vbool || right._vbool && left._vbool;
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::implication_double(const _MyT& left, const _MyT& right)
{
	clear();
	_type = tvt_boolean;

	if (_type != left._type || _type != right._type)
		throw operation::exception("Must be boolean type");
	_vbool = !left._vbool && !right._vbool || left._vbool && right._vbool;
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::fnc_if(const _MyT& bValue, _MyT& trueReturn, _MyT& falseReturn)
{
	clear();
	if (bValue._type != tvt_boolean)
		throw operation::exception("The first argument must be boolean type");

	_MyT* src = bValue._vbool ? &trueReturn : &falseReturn;
	_type = src->_type;
	switch (_type)
	{
	case tvt_null:
		break;
	case tvt_boolean:
		_vbool = src->_vbool;
		break;
	case tvt_numeric:
		_vnumeric = src->_vnumeric;
		break;
	case tvt_string:
		_vstring = src->_vstring;
		break;
	case tvt_object:
		_vobject = src->_vobject;
	}

	src->_type = tvt_null;
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::pow(const _MyT& left, const _MyT& right)
{
	const _Number& l = *left.numeric();
	const _Number& r = *right.numeric();
	_numeric()->pow(l, r);
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::sqrroot(const _MyT& right)
{
	const _Number& r = *right.numeric();
	_numeric()->sqrroot(r);
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::root(const _MyT& left, const _MyT& right)
{
	const _Number& l = *left.numeric();
	const _Number& r = *right.numeric();
	_numeric()->root(l, r);
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::assignation(const _MyT& left, const _MyT& right)
{
	const _Number& l = *left.numeric();
	const _Number& r = *right.numeric();
	_numeric()->assignation(l, r);
}



// Functions:
template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::abs(const _MyT& arg)
{
	const _Number& a = *arg.numeric();
	_numeric()->abs(a);
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::sgn(const _MyT& arg)
{
	const _Number& a = *arg.numeric();
	_numeric()->sgn(a);
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::ln(const _MyT& arg)
{
	const _Number& a = *arg.numeric();
	_numeric()->ln(a);
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::lg(const _MyT& arg)
{
	const _Number& a = *arg.numeric();
	_numeric()->lg(a);
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::log(const _MyT& arg, const _MyT& base)
{
	const _Number& a = *arg.numeric();
	const _Number& b = *base.numeric();
	_numeric()->log(a, b);
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::sin(const _MyT& arg)
{
	const _Number& a = *arg.numeric();
	_numeric()->sin(a);
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::cos(const _MyT& arg)
{
	const _Number& a = *arg.numeric();
	_numeric()->cos(a);
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::tg(const _MyT& arg)
{
	const _Number& a = *arg.numeric();
	_numeric()->tg(a);
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::asin(const _MyT& arg)
{
	const _Number& a = *arg.numeric();
	_numeric()->asin(a);
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::acos(const _MyT& arg)
{
	const _Number& a = *arg.numeric();
	_numeric()->acos(a);
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::atg(const _MyT& arg)
{
	const _Number& a = *arg.numeric();
	_numeric()->atg(a);
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline void tvobject<_NUMBER, _COBJECT, _STRING>::atg2(const _MyT& opposite, const _MyT& adjacent)
{
	const _Number& o = *opposite.numeric();
	const _Number& a = *adjacent.numeric();
	_numeric()->atg2(o, a);
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline tvobject<_NUMBER, _COBJECT, _STRING>
tvobject<_NUMBER, _COBJECT, _STRING>::pow(_TypeInt p) const
{
	return tvobject(numeric()->pow(p));
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline tvobject<_NUMBER, _COBJECT, _STRING>
tvobject<_NUMBER, _COBJECT, _STRING>::operator/(const _MyT& right) const
{
	const _Number& r = *right.numeric();
	return tvobject(numeric()->operator/(r));
}



// Check:
template<class _NUMBER, class _COBJECT, class _STRING>
inline bool tvobject<_NUMBER, _COBJECT, _STRING>::zero() const
{
	return numeric()->zero();
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline bool tvobject<_NUMBER, _COBJECT, _STRING>::positive() const
{
	return numeric()->positive();
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline bool tvobject<_NUMBER, _COBJECT, _STRING>::negative() const
{
	return numeric()->negative();
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline bool tvobject<_NUMBER, _COBJECT, _STRING>::integer() const
{
	return numeric()->integer();
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline bool tvobject<_NUMBER, _COBJECT, _STRING>::dimensionless() const
{
	return numeric()->dimensionless();
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline bool tvobject<_NUMBER, _COBJECT, _STRING>::one_dimension() const
{
	return numeric()->one_dimension();
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline bool tvobject<_NUMBER, _COBJECT, _STRING>::scalar() const
{
	return numeric()->scalar();
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline bool tvobject<_NUMBER, _COBJECT, _STRING>::real() const
{
	return numeric()->real();
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline bool tvobject<_NUMBER, _COBJECT, _STRING>::imaginary() const
{
	return numeric()->imaginary();
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline bool tvobject<_NUMBER, _COBJECT, _STRING>::complex() const
{
	return numeric()->complex();
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline bool tvobject<_NUMBER, _COBJECT, _STRING>::vector() const
{
	return numeric()->vector();
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline bool tvobject<_NUMBER, _COBJECT, _STRING>::matrix() const
{
	return numeric()->matrix();
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline bool tvobject<_NUMBER, _COBJECT, _STRING>::equal_unit(const _MyT& right) const
{
	return numeric()->equal_unit(*right.numeric());
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline const typename tvobject<_NUMBER, _COBJECT, _STRING>::_TypeValue& tvobject<_NUMBER, _COBJECT, _STRING>::get_number() const
{
	return numeric()->get_number();
}



template<class _NUMBER, class _COBJECT, class _STRING>
inline const typename tvobject<_NUMBER, _COBJECT, _STRING>::unit& tvobject<_NUMBER, _COBJECT, _STRING>::get_unit() const
{
	return numeric()->get_unit();
}



}
}
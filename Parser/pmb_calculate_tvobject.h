#pragma once

namespace pmb
{
namespace calculate
{



template <class _NUMBER, class _COBJECT, class _STRING>
struct tvobject
{
	typename typedef _NUMBER _Number;
	typename typedef _Number::_TypeValue _TypeValue;
	typename typedef _Number::_2TypeValue _2TypeValue;
	typename typedef _Number::_TypeInt _TypeInt;
	typename typedef _Number::_TypeChar _TypeChar;
	typename typedef _Number::_TypeSzstr _TypeSzstr;
	typedef typename _Number::unit unit;
	typedef typename _Number::dimension dimension;
	typename typedef _COBJECT _CObject;
	typename typedef _STRING _String;
	typename typedef tvobject _MyT;


	enum tvtype : char {
		tvt_null,
		tvt_boolean,
		tvt_numeric,
		tvt_string,
		tvt_object
	};


	tvobject();
	tvobject(tvobject& src);
	explicit tvobject(const _Number& src);
	explicit tvobject(const _MyT* src);
	explicit tvobject(const _TypeChar* str, const _TypeSzstr& len);
	explicit tvobject(const dimension* dim);
	explicit tvobject(const _TypeValue& n, const unit& u);
	~tvobject();

private:
	void _destructor();
public:

	void clear();

	bool is_null() const;
	bool is_boolean() const;
	bool is_numeric() const;
	bool is_string() const;
	bool is_object() const;

	bool boolean() const;
	const _Number* numeric() const;
	_Number* numeric();
	const _String* string() const;
	_String* string();
	const _CObject* object() const;
	_CObject* object();

protected:
	_Number* _numeric();

public:

	// Operators:
	void positive(const _MyT& right);
	void negative(const _MyT& right);

	void factorial(const _MyT& right);
	void multiplication(const _MyT& left, const _MyT& right);
	void division(const _MyT& left, const _MyT& right);
	void modulo(const _MyT& left, const _MyT& right);

	void addition(const _MyT& left, const _MyT& right);
	void substraction(const _MyT& left, const _MyT& right);

	void pow(const _MyT& left, const _MyT& right);
	void sqrroot(const _MyT& right);
	void root(const _MyT& left, const _MyT& right);

	void equal(const _MyT& left, const _MyT& right);
	void not_equal(const _MyT& left, const _MyT& right);
	void less_equal(const _MyT& left, const _MyT& right);
	void less(const _MyT& left, const _MyT& right);
	void greater_equal(const _MyT& left, const _MyT& right);
	void greater(const _MyT& left, const _MyT& right);
	void not(const _MyT& right);
	bool and_check(const _MyT& left);
	void and(const _MyT& left, const _MyT& right);
	bool nand_check(const _MyT& left);
	void nand(const _MyT& left, const _MyT& right);
	void xor(const _MyT& left, const _MyT& right);
	bool or_check(const _MyT & left);
	void or(const _MyT& left, const _MyT& right);
	bool nor_check(const _MyT& left);
	void nor(const _MyT & left, const _MyT & right);
	bool implication_check(const _MyT& left);
	void implication(const _MyT& left, const _MyT& right);
	bool implication_reverse_check(const _MyT& left);
	void implication_reverse(const _MyT& left, const _MyT& right);
	void implication_double(const _MyT& left, const _MyT& right);

	void assignation(const _MyT& left, const _MyT& right);


	// functions:
	void abs(const _MyT& arg);
	void sgn(const _MyT& arg);

	// logarithmic functions:
	void ln(const _MyT& arg);
	void lg(const _MyT& arg);
	void log(const _MyT& arg, const _MyT& base);

	// trigonometric functions:
	void sin(const _MyT& arg);
	void cos(const _MyT& arg);
	void tg(const _MyT& arg);

	// inverse trigonometric functions:
	void asin(const _MyT& arg);
	void acos(const _MyT& arg);
	void atg(const _MyT& arg);
	void atg2(const _MyT& opposite, const _MyT& adjacent);

	void fnc_if(const _MyT& bValue, _MyT& trueReturn, _MyT& falseReturn);

	_MyT pow(_TypeInt p) const;
	_MyT operator/(const _MyT& right) const;

	// check:
	bool zero() const;
	bool positive() const;
	bool negative() const;
	bool integer() const;
	bool dimensionless() const;
	bool one_dimension() const;
	bool scalar() const;
	bool real() const;
	bool imaginary() const;
	bool complex() const;
	bool vector() const;
	bool matrix() const;

	void stringstream(std::stringstream& ssOut) const;
	bool equal_unit(const _MyT& right) const;

	const _TypeValue& get_number() const;
	const unit& get_unit() const;


protected:
	tvtype _type;
	union {
		bool _vbool;
		_Number* _vnumeric;
		_CObject* _vobject;
		_String* _vstring;
	};
};



}
}

#include "pmb_calculate_tvobject.hpp"

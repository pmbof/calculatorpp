#pragma once


namespace pmb
{
namespace calculate
{



template<class _TYPE>
struct number
{
public:
	typename typedef _TYPE _TypeValue;
	typedef number<_TYPE> _MyT;

public:
	// Constructors:
	number();
	explicit number(const _TYPE& src);
	explicit number(const _MyT* src);
	explicit number(const char* str, unsigned short len);

	// Operators:
	void positive(const _MyT& right);
	void negative(const _MyT& right);

	void factorial(const _MyT& right);
	void multiplication(const _MyT& left, const _MyT& right);
	void division(const _MyT& left, const _MyT& right);
	void modulo(const _MyT& left, const _MyT& right);

	void addition(const _MyT& left, const _MyT& right);
	void substraction(const _MyT& left, const _MyT& right);

	void exponentiation(const _MyT& left, const _MyT& right);
	void root(const _MyT& left, const _MyT& right);

	void assignation(const _MyT& left, const _MyT& right);


	// functions:
	void abs(const _MyT& arg);
	void sgn(const _MyT& arg);

	// trigonometric functions:
	void sin(const _MyT& arg);
	void tg(const _MyT& arg);

	// inverse trigonometric functions:
	void atg(const _MyT& arg);
	void atg2(const _MyT& opposite, const _MyT& adjacent);
	

	void result(const _MyT& right) {
	}

	// check:
	bool zero() const;
	bool positive() const;
	bool negative() const;
	bool integer() const;
	bool dimensionless() const;
	bool scalar() const;
	bool real() const;
	bool imaginary() const;
	bool complex() const;
	bool vector() const;
	bool matrix() const;

	_TYPE _number;
};







}
}

#include "pmb_calculate_number.hpp"
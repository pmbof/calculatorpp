#pragma once

#include <list>


namespace pmb
{
namespace calculate
{
namespace units
{



template<typename _CHAR>
class dimension
{
public:
	dimension();
//	dimension(const stringcmp& symbol, const char* name);
	~dimension();

	bool empty() const;
	bool operator==(const _CHAR* dimension) const;
	bool operator!=(const _CHAR* dimension) const;

	void set(const _CHAR* symbol, unsigned short len, const _CHAR* name = nullptr);

	const _CHAR* getSymbol() const;
	const _CHAR* getName() const;

private:
	void clear();

private:
	_CHAR* _symbol;
	_CHAR* _name;
};














template <typename _INT>
struct rational
{
	_INT numerator;
	_INT denominator;

	rational();
	rational(const _INT& _numerator, const _INT& _denominator);

	static void normalize(_INT& _numerator, _INT& _denominator);
	void normalize();

	operator bool() const;
	bool operator!() const;

	bool operator==(const rational<_INT>& right) const;

	rational& operator=(const rational<_INT>& right);
	rational& operator=(const _INT& right);
	rational& operator+=(const rational<_INT>& right);

	rational operator+(const rational<_INT>& right) const;
	rational operator-(const rational<_INT>& right) const;
	rational operator*(const rational<_INT>& right) const;
	rational operator/(const rational<_INT>& right) const;
	rational operator*(const _INT& right) const;
	rational operator/(const _INT& right) const;

	template<typename _INT>
	friend rational<_INT> operator+(const _INT& left, const rational<_INT>& right);
	template<typename _INT>
	friend rational<_INT> operator-(const _INT& left, const rational<_INT>& right);
	template<typename _INT>
	friend rational<_INT> operator*(const _INT& left, const rational<_INT>& right);
	template<typename _INT>
	friend rational<_INT> operator/(const _INT& left, const rational<_INT>& right);
};














template<typename _INT, typename _CHAR>
struct power_dimension: rational<_INT>
{
	power_dimension();
	power_dimension(const rational<_INT>& q, const dimension<_CHAR>* pDim);

	power_dimension<_INT, _CHAR>& operator =(const power_dimension<_INT, _CHAR>& right);
	power_dimension<_INT, _CHAR>& operator +=(const power_dimension<_INT, _CHAR>& right);

	power_dimension<_INT, _CHAR> operator*(const _INT& right) const;
	power_dimension<_INT, _CHAR> operator/(const _INT& right) const;

	template<typename _INT, typename _CHAR>
	friend power_dimension<_INT, _CHAR> operator*(const _INT& left, const power_dimension<_INT, _CHAR>& right);
	template<typename _INT, typename _CHAR>
	friend power_dimension<_INT, _CHAR> operator/(const _INT& left, const power_dimension<_INT, _CHAR>& right);


	const dimension<_CHAR>* dim;
};












template<typename _CHAR, typename _POWER>
class prefix
{
public:
	prefix(const _CHAR* symbol, const _CHAR* name, _POWER power);
	~prefix();

	short find(const _CHAR* u) const;
	short findName(const _CHAR* u) const;

	double getFactor(short base) const;

private:
	_CHAR* _symbol;
	_CHAR* _name;
	_POWER _power;
};














template<typename _CHAR, typename _POWER, typename _BASE>
struct prefix_base: std::list<prefix<_CHAR, _POWER>*>
{
	_BASE base;
};










































template<typename _INT, typename _CHAR>
struct unit
{
	typedef unsigned char ndim;

	explicit unit(power_dimension<_INT, _CHAR>* dim, const ndim& nDims);
	explicit unit(const dimension<_CHAR>* dim);
	unit(const unit& u);
	unit();
	~unit();

	ndim nDims() const;
	bool operator==(const unit& right) const;
	bool operator!=(const unit& right) const;

	unit& operator=(const unit& right);
	unit& operator=(const dimension<_CHAR>* dim);

	unit operator*(const unit& right) const;
	unit operator/(const unit& right) const;
	unit pow(const _INT& p) const;
	unit root(const _INT& p) const;

	void clean();

	void clear();

	std::string getDimension() const;

private:
	ndim calcNewDim(const unit& right) const;
	void populateNewDim(power_dimension<_INT, _CHAR>* result, ndim nNewDim, const unit& right, const _INT& mult) const;
	void calc(power_dimension<_INT, _CHAR>** result, ndim* nNewDim, const unit& right, const _INT& mult) const;
	static bool compressVector(power_dimension<_INT, _CHAR>* dim, ndim* nDim);

private:
	ndim _capacity;
	ndim _nDims;
	power_dimension<_INT, _CHAR>* _dim;
};























template<class _TYPE, typename _INT, typename _CHAR>
struct magnitude
{
public:
	typename typedef _TYPE _TypeValue;
	typename typedef _INT _TypeInt;
	typedef magnitude<_TYPE, _INT, _CHAR> _MyT;

public:
	// Constructors:
	magnitude();
	explicit magnitude(const _TYPE& src);
	explicit magnitude(const _MyT* src);
	explicit magnitude(const _CHAR* str, unsigned short len);

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
	unit<_INT, _CHAR> _unit;
};






}
}
}


#include "pmb_calculate_magnitude.hpp"

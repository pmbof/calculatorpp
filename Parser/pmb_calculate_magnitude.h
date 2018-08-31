#pragma once

#include <list>


namespace pmb
{
namespace calculate
{
namespace units
{



template<typename _CHAR, typename _SIZE>
class dimension
{
public:
	typedef typename _CHAR tpChar;
	typedef typename _SIZE tpSize;

public:
	dimension();
	dimension(const _CHAR* symbol, const _CHAR* name = nullptr);
	dimension(const _CHAR* symbol, const _SIZE& slen, const _CHAR* name = nullptr, const _SIZE& nlen = 0);
//	dimension(const stringcmp& symbol, const char* name);
	~dimension();

	bool empty() const;
	bool operator==(const _CHAR* dimension) const;
	bool operator!=(const _CHAR* dimension) const;

	bool operator<(const dimension& right) const;
	bool less(const _CHAR* right, const _SIZE& len);
	bool greater(const _CHAR* right, const _SIZE& len);

	bool less_name(const dimension& right) const;
	bool less_name(const _CHAR* right, const _SIZE& len);
	bool greater_name(const _CHAR* right, const _SIZE& len);

	void set(const _CHAR* symbol, const _SIZE& slen, const _CHAR* name = nullptr, const _SIZE& nlen = 0);

	const _CHAR* symbol() const;
	const _CHAR* name() const;
	const _SIZE& symbol_size() const;
	const _SIZE& name_size() const;

private:
	void clear();

private:
	_SIZE _slen;
	_CHAR* _symbol;
	_SIZE _nlen;
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














template<typename _INT, typename _CHAR, typename _SZSTR>
struct power_dimension: rational<_INT>
{
	typedef typename _SZSTR tpSzStr;
	typedef typename dimension<_CHAR, _SZSTR> dimension;

	power_dimension();
	power_dimension(const rational<_INT>& q, const dimension* pDim);

	power_dimension& operator =(const power_dimension& right);
	power_dimension& operator +=(const power_dimension& right);

	power_dimension operator*(const _INT& right) const;
	power_dimension operator/(const _INT& right) const;

	template<typename _INT, typename _CHAR, typename _SZSTR>
	friend power_dimension<_INT, _CHAR, _SZSTR> operator*(const _INT& left, const power_dimension<_INT, _CHAR, _SZSTR>& right);
	template<typename _INT, typename _CHAR, typename _SZSTR>
	friend power_dimension<_INT, _CHAR, _SZSTR> operator/(const _INT& left, const power_dimension<_INT, _CHAR, _SZSTR>& right);


	const dimension* dim;
};














































template<typename _INT, typename _CHAR, typename _SZSTR>
struct unit
{
	typedef typename _INT _tpInt;
	typedef typename _CHAR _tpChar;
	typedef power_dimension<_INT, _CHAR, _SZSTR> power_dimension;
	typedef typename power_dimension::dimension dimension;
	typedef unsigned char ndim;

	explicit unit(power_dimension* dim, const ndim& nDims);
	explicit unit(const dimension* dim);
	unit(const unit& u);
	unit();
	~unit();

	ndim nDims() const;
	bool operator==(const unit& right) const;
	bool operator!=(const unit& right) const;

	unit& operator=(const unit& right);
	unit& operator=(const dimension* dim);

	unit operator*(const unit& right) const;
	unit operator/(const unit& right) const;
	unit pow(const _INT& p) const;
	unit root(const _INT& p) const;

	bool one_dimension() const;
	const dimension* operator[](ndim nd) const;
	const dimension* get_dimension(ndim nd) const;

	void clean();

	void clear();

	std::string getDimension() const;

private:
	ndim calcNewDim(const unit& right) const;
	void populateNewDim(power_dimension* result, ndim nNewDim, const unit& right, const _INT& mult) const;
	void calc(power_dimension** result, ndim* nNewDim, const unit& right, const _INT& mult) const;
	static bool compressVector(power_dimension* dim, ndim* nDim);

private:
	ndim _capacity;
	ndim _nDims;
	power_dimension* _dim;
};




























template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
struct magnitude
{
public:
	typename typedef _TYPE _TypeValue;
	typename typedef _INT _TypeInt;
	typedef magnitude<_TYPE, _INT, _CHAR, _SZSTR> _MyT;
	typedef typename unit<_INT, _CHAR, _SZSTR> unit;
	typedef typename unit::dimension dimension;

public:
	// Constructors:
	magnitude();
	explicit magnitude(const _TYPE& src);
	explicit magnitude(const _MyT* src);
	explicit magnitude(const _CHAR* str, const _SZSTR& len);
	explicit magnitude(const dimension* dim);

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
	bool one_dimension() const;
	bool scalar() const;
	bool real() const;
	bool imaginary() const;
	bool complex() const;
	bool vector() const;
	bool matrix() const;

	_TYPE _number;
	unit _unit;
};







}
}
}


#include "pmb_calculate_magnitude.hpp"

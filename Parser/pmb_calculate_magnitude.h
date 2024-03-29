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
	void set(unsigned char index);

	unsigned char index() const;
	const _CHAR* symbol() const;
	const _CHAR* name() const;
	const _SIZE symbol_size() const;
	const _SIZE name_size() const;

private:
	void clear();

private:
	unsigned char _index;
	_SIZE _slen;
	_CHAR* _symbol;
	_SIZE _nlen;
	_CHAR* _name;
};














template <typename _INT>
struct rational
{
	typename typedef rational<_INT> _MyT;
	typename typedef _INT _TypeNum;


	_INT numerator;
	_INT denominator;

	rational();
	explicit rational(const _INT& _numerator);
	rational(const _INT& _numerator, const _INT& _denominator);
	template <typename _N>
	explicit rational(const _N& number);

	static void normalize(_INT& _numerator, _INT& _denominator);
	void normalize();


	std::string serialize() const;
	bool deserialize(const char* bytes, unsigned short size);

	operator bool() const;
	bool operator!() const;
	explicit operator long() const;

	bool operator==(const _MyT& right) const;

	rational& operator=(const _MyT& right);
	rational& operator=(const _INT& right);
	rational& operator+=(const _MyT& right);

	rational operator-() const;
	rational operator+() const;
	rational operator+(const _MyT& right) const;
	rational operator-(const _MyT& right) const;
	rational operator*(const _MyT& right) const;
	rational operator/(const _MyT& right) const;
	rational operator*(const _INT& right) const;
	rational operator/(const _INT& right) const;

	rational operator*=(const _INT& right);

	template <typename _N>
	rational operator*(const _N& right) const;

	template <typename _N>
	rational operator/(const _N& right) const;


	bool operator<(const rational<_INT>& right) const;
	bool operator<(const _INT& right) const;
	bool operator<=(const rational<_INT>& right) const;
	bool operator<=(const _INT& right) const;
	bool operator>(const rational<_INT>& right) const;
	bool operator>(const _INT& right) const;
	bool operator>=(const rational<_INT>& right) const;
	bool operator>=(const _INT& right) const;

	bool zero() const;
	bool unit() const;
	bool natural() const;
	bool integer() const;

	rational pow(const _INT& exp) const;
	rational pow(const _MyT& exp) const;

	_INT proportionality(const rational<_INT>& right) const;

	template<typename _INT>
	friend rational<_INT> operator+(const _INT& left, const rational<_INT>& right);
	template<typename _INT>
	friend rational<_INT> operator-(const _INT& left, const rational<_INT>& right);
	template<typename _INT>
	friend rational<_INT> operator*(const _INT& left, const rational<_INT>& right);
	template<typename _INT>
	friend rational<_INT> operator/(const _INT& left, const rational<_INT>& right);

	template<typename _INT>
	friend std::stringstream& operator<<(std::stringstream& left, const rational<_INT>& right);
};














template<typename _INT, typename _CHAR, typename _SZSTR>
struct power_dimension: rational<_INT>
{
	typedef typename _SZSTR tpSzStr;
	typedef typename dimension<_CHAR, _SZSTR> dimension;
	typedef typename rational<_INT> rational_base;

	power_dimension();
	power_dimension(const rational<_INT>& q, const dimension* pDim);

	std::string serialize() const;
	unsigned char deserialize(const char* bytes, unsigned short size);

	power_dimension& operator =(const power_dimension& right);
	power_dimension& operator +=(const power_dimension& right);

	template<typename _N>
	power_dimension operator*(const _N& right) const;
	template<typename _N>
	power_dimension operator/(const _N& right) const;

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

	std::string serialize() const;
	bool deserialize(dimension**const vdimensions, unsigned char vdimsz, const char* bytes, unsigned short size);

	ndim nDims() const;
	bool operator==(const unit& right) const;
	bool operator!=(const unit& right) const;

	unit& operator=(const unit& right);
	unit& operator=(const dimension* dim);

	unit operator*(const unit& right) const;
	unit operator/(const unit& right) const;
	template<typename _N>
	unit pow(const _N& p) const;
	template<typename _N>
	unit pow(const _N& n, const _N& d) const;
	template<typename _N>
	unit root(const _N& p) const;

	bool dimensionless() const;
	bool one_dimension() const;
	const dimension* operator[](ndim nd) const;
	const dimension* get_dimension(ndim nd) const;
	const power_dimension* get_powdimension(ndim nd) const;

	void clean();

	void release();
	void clear();

	_INT compare(const unit& right) const;

	std::string get_dimension() const;

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
	typename typedef _TypeValue::_TypeValue _2TypeValue;
	typename typedef _INT _TypeInt;
	typename typedef _CHAR _TypeChar;
	typename typedef _SZSTR _TypeSzstr;

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
	explicit magnitude(const _TYPE& n, const unit& u);


	bool is_numeric() const;

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


	magnitude pow(_TypeInt p) const;
	magnitude operator*(const magnitude& right) const;
	magnitude operator/(const magnitude& right) const;

	bool equal(const magnitude& right) const;
	bool less_equal(const magnitude& right) const;
	bool less(const magnitude& right) const;
	bool greater(const magnitude& right) const;
	bool greater_equal(const magnitude& right) const;

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
	bool equal_unit(const magnitude& right) const;

	const _TYPE& get_number() const;
	_TYPE& get_number();
	const unit& get_unit() const;
	unit& get_unit();
	void release_unit();

protected:
	_TYPE _number;
	unit _unit;
};







}
}
}


#include "pmb_calculate_magnitude.hpp"

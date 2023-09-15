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
	explicit number(const _TypeValue& src);
	explicit number(const _MyT* src);
	explicit number(const char* str, unsigned short len);

	std::string serialize() const;
	bool deserialize(const char* bytes, unsigned short size);

	// Assign operators:
	_MyT& operator=(const _TypeValue& rValue);
	_MyT& operator*=(const _TypeValue& rValue);

	// Operators comparation:
	bool operator ==(const _MyT& right) const;
	bool operator !=(const _MyT& right) const;
	bool operator<(const _MyT& right) const;
	bool operator<(const _TypeValue& right) const;
	bool operator>(const _MyT& right) const;
	bool operator<=(const _MyT& right) const;
	bool operator<=(const _TypeValue& right) const;
	bool operator>=(const _MyT& right) const;
	bool operator>=(const _TypeValue& right) const;

	template<class _TYPE>
	friend bool operator <(const _TypeValue& left, const number<_TYPE>& right);
	template<class _TYPE>
	friend bool operator <=(const _TypeValue& left, const number<_TYPE>& right);

	operator _TypeValue() const;

	operator bool() const;
	bool operator!() const;

	// Math operations:
	_MyT operator+() const;
	_MyT operator-() const;

	_MyT operator*(const _MyT& right) const;
	_MyT operator*(const _TypeValue& right) const;
	_MyT operator/(const _MyT& right) const;
	_MyT operator/(const _TypeValue& right) const;
	_MyT operator%(const _MyT& right) const;

	_MyT operator+(const _MyT& right) const;
	_MyT operator-(const _MyT& right) const;


	// Operations:
	void positive(const _MyT& right);
	void negative(const _MyT& right);

	// functions:
	_MyT abs() const;
	_MyT sgn() const;

	_MyT factorial() const;

	_MyT pow(const _MyT& right) const;
	_MyT pow(const _TypeValue& right) const;
	_MyT sqrt() const;
	_MyT root(const _MyT& right) const;
	_MyT ln() const;
	_MyT log() const;
	_MyT log(const _MyT& base) const;

	// trigonometric functions:
	_MyT sin() const;
	_MyT cos() const;
	_MyT tg() const;
	_MyT sec() const;
	_MyT cosec() const;
	_MyT cotg() const;

	// inverse trigonometric functions:
	_MyT asin() const;
	_MyT acos() const;
	_MyT atg() const;
	_MyT atg(const _MyT& adjacent) const;
	

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

	template<class _TYPE>
	friend std::stringstream& operator <<(std::stringstream& ss, const number<_TYPE>& right);
};






// Friend operators:
template<class _TYPE>
inline bool operator <(const _TYPE& left, const number<_TYPE>& right);

template<class _TYPE>
inline bool operator <=(const _TYPE& left, const number<_TYPE>& right);


template<class _TYPE>
inline std::stringstream& operator <<(std::stringstream& ss, const number<_TYPE>& right);


}
}



#include "pmb_calculate_number.hpp"
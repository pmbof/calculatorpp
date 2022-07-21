#include "pmb_calculate_number.h"
#pragma once


namespace pmb
{
namespace calculate
{


// Constructors:
template<class _TYPE>
number<_TYPE>::number()
{
}



template<class _TYPE>
inline number<_TYPE>::number(const _TypeValue& src)
	: _number(src)
{
}



template<class _TYPE>
inline number<_TYPE>::number(const _MyT* src)
	: _number(src->_number)
{
}


template<class _TYPE>
inline number<_TYPE>::number(const char* str, unsigned short len)
{
	_number = 0;
	unsigned short ini = 0;
	int base = 10;
	if (2 < len && str[0] == '0') {
		ini = 2;
		switch (str[1]) {
		case 'x':
		case 'X':
			base = 16;
			break;
		case 'o':
		case 'O':
			base = 8;
			break;
		case 'b':
		case 'B':
			base = 2;
			break;
		default:
			ini = 0;
			break;
		}
	}
	bool bDecimal = false;
	_TYPE decimal = 1;
	for (unsigned short i = ini; i < len; ++i)
	{
		if (str[i] == '.')
		{
			if (bDecimal)
				throw "numeric expression error";
			bDecimal = true;
		}
		else
			_number = _number * (bDecimal ? 1 : base) + (10 < base && 'A' <= str[i] ? str[i] < 'a' ? str[i] - 'A' : str[i] - 'a' : str[i] - '0') / decimal;
		if (bDecimal)
			decimal = decimal * base;
	}
}



// Assign operators:
template<class _TYPE>
inline number<_TYPE>& number<_TYPE>::operator=(const _TypeValue& rValue)
{
	_number = rValue;
	return *this;
}



template<class _TYPE>
inline number<_TYPE>& number<_TYPE>::operator*=(const _TypeValue& rValue)
{
	_number *= rValue;
	return *this;
}



// Operators comparation:
template<class _TYPE>
inline bool number<_TYPE>::operator==(const _MyT& right) const
{
	return _number == right._number;
}



template<class _TYPE>
inline bool number<_TYPE>::operator!=(const _MyT& right) const
{
	return _number != right._number;
}



template<class _TYPE>
inline bool number<_TYPE>::operator<(const _MyT& right) const
{
	return _number < right._number;
}



template<class _TYPE>
inline bool number<_TYPE>::operator<(const _TypeValue& right) const
{
	return _number < right;
}



template<class _TYPE>
inline bool number<_TYPE>::operator>(const _MyT& right) const
{
	return _number > right._number;
}



template<class _TYPE>
inline bool number<_TYPE>::operator<=(const _MyT& right) const
{
	return _number <= right._number;
}



template<class _TYPE>
inline bool number<_TYPE>::operator<=(const _TypeValue& right) const
{
	return _number <= right;
}



template<class _TYPE>
inline bool number<_TYPE>::operator>=(const _MyT& right) const
{
	return _number >= right._number;
}



template<class _TYPE>
inline bool number<_TYPE>::operator>=(const _TypeValue& right) const
{
	return _number >= right;
}



// Friend operators:
template<class _TYPE>
inline bool operator <(const _TYPE& left, const number<_TYPE>& right)
{
	return left < right._number;
}



template<class _TYPE>
inline bool operator <=(const _TYPE& left, const number<_TYPE>& right)
{
	return left <= right._number;
}



template<class _TYPE>
inline number<_TYPE>::operator _TypeValue() const
{
	return _number;
}




// Operators comparation:
template<class _TYPE>
inline number<_TYPE>::operator bool() const
{
	return !zero();
}



template<class _TYPE>
inline bool number<_TYPE>::operator!() const
{
	return zero();
}



// Math operations:
template<class _TYPE>
inline number<_TYPE> number<_TYPE>::operator+() const
{
	return _MyT(_number);
}



template<class _TYPE>
inline number<_TYPE> number<_TYPE>::operator-() const
{
	return _MyT(-_number);
}



template<class _TYPE>
inline number<_TYPE> number<_TYPE>::operator*(const _MyT& right) const
{
	return _MyT(_number * right._number);
}



template<class _TYPE>
inline number<_TYPE> number<_TYPE>::operator*(const _TypeValue& right) const
{
	return _MyT(_number * right);
}



template<class _TYPE>
inline number<_TYPE> number<_TYPE>::operator/(const _MyT& right) const
{
	return _MyT(_number / right._number);
}



template<class _TYPE>
inline number<_TYPE> number<_TYPE>::operator/(const _TypeValue& right) const
{
	return _MyT(_number / right);
}



template<class _TYPE>
inline number<_TYPE> number<_TYPE>::operator%(const _MyT& right) const
{
	return _MyT((long)_number % (long)right._number);
}



template<class _TYPE>
inline number<_TYPE> number<_TYPE>::operator+(const _MyT& right) const
{
	return _MyT(_number + right._number);
}



template<class _TYPE>
inline number<_TYPE> number<_TYPE>::operator-(const _MyT& right) const
{
	return _MyT(_number - right._number);
}



// Operations:
template<class _TYPE>
inline void number<_TYPE>::positive(const _MyT& right)
{
	_number = +right._number;
}


template<class _TYPE>
inline void number<_TYPE>::negative(const _MyT& right)
{
	_number = -right._number;
}



// Math functions:
template<class _TYPE>
inline number<_TYPE> number<_TYPE>::abs() const
{
	return _MyT(_number < 0 ? -_number : _number);
}



template<class _TYPE>
inline number<_TYPE> number<_TYPE>::sgn() const
{
	return _MyT(_number < 0 ? -1 : 1);
}


template<class _TYPE>
inline number<_TYPE> number<_TYPE>::factorial() const
{
	_MyT res;
	res._number = 1;
	for (int i = 2; i <= _number; ++i)
		res._number *= i;
	return res;
}



template<class _TYPE>
inline number<_TYPE> number<_TYPE>::pow(const _MyT& right) const
{
	return _MyT(::pow(_number, right._number));
}



template<class _TYPE>
inline number<_TYPE> number<_TYPE>::pow(const _TypeValue& right) const
{
	return _MyT(::pow(_number, right));
}



template<class _TYPE>
inline number<_TYPE> number<_TYPE>::sqrt() const
{
	return _MyT(::sqrt(_number));
}



template<class _TYPE>
inline number<_TYPE> number<_TYPE>::root(const _MyT& exp) const
{
	return _MyT(::pow(_number, 1 / exp._number));
}



template<class _TYPE>
inline number<_TYPE> number<_TYPE>::ln() const
{
	return _MyT(::log(_number));
}



template<class _TYPE>
inline number<_TYPE> number<_TYPE>::log() const
{
	return _MyT(::log10(_number));
}



template<class _TYPE>
inline number<_TYPE> number<_TYPE>::log(const _MyT& base) const
{
	return _MyT(::log(_number) / ::log(base._number));
}



// trigonometric functions:
template<class _TYPE>
inline number<_TYPE> number<_TYPE>::sin() const
{
	return _MyT(::sin(_number));
}



template<class _TYPE>
inline number<_TYPE> number<_TYPE>::cos() const
{
	return _MyT(::cos(_number));
}



template<class _TYPE>
inline number<_TYPE> number<_TYPE>::tg() const
{
	return _MyT(::tan(_number));
}



template<class _TYPE>
inline number<_TYPE> number<_TYPE>::sec() const
{
	return _MyT(::sec(_number));
}



template<class _TYPE>
inline number<_TYPE> number<_TYPE>::cosec() const
{
	return _MyT(::cosec(_number));
}



template<class _TYPE>
inline number<_TYPE> number<_TYPE>::cotg() const
{
	return _MyT(::cotan(_number));
}



template<class _TYPE>
inline number<_TYPE> number<_TYPE>::asin() const
{
	return _MyT(::asin(_number));
}



template<class _TYPE>
inline number<_TYPE> number<_TYPE>::acos() const
{
	return _MyT(::acos(_number));
}



template<class _TYPE>
inline number<_TYPE> number<_TYPE>::atg() const
{
	return _MyT(::atan(_number));
}


template<class _TYPE>
inline number<_TYPE> number<_TYPE>::atg(const _MyT& adjacent) const
{
	return _MyT(::atan2(_number, adjacent._number));
}



// Check:
template<class _TYPE>
inline bool number<_TYPE>::zero() const
{
	return _number == _TYPE(0);
}

template<class _TYPE>
inline bool number<_TYPE>::positive() const
{
	return 0 < _number;
}


template<class _TYPE>
inline bool number<_TYPE>::negative() const
{
	return _number < 0;
}


template<class _TYPE>
inline bool number<_TYPE>::integer() const
{
	return ceil(_number) == floor(_number);
}


template<class _TYPE>
inline bool number<_TYPE>::dimensionless() const
{
	return true;
}


template<class _TYPE>
inline bool number<_TYPE>::scalar() const
{
	return true;
}


template<class _TYPE>
inline bool number<_TYPE>::real() const
{
	return true;
}

template<class _TYPE>
inline bool number<_TYPE>::imaginary() const
{
	return false;
}


template<class _TYPE>
inline bool number<_TYPE>::complex() const
{
	return false;
}


template<class _TYPE>
inline bool number<_TYPE>::vector() const
{
	return false;
}


template<class _TYPE>
inline bool number<_TYPE>::matrix() const
{
	return false;
}




template<class _TYPE>
std::stringstream& operator <<(std::stringstream& ss, const number<_TYPE>& right)
{
	ss << right._number;
	return ss;
}


}
}
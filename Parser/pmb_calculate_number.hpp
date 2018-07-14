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
inline number<_TYPE>::number(const _TYPE& src)
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


template<class _TYPE>
inline void number<_TYPE>::factorial(const _MyT& right)
{
	long res = 1;
	for (int i = 2; i <= right._number; ++i)
		res *= i;
	_number = res;
}


template<class _TYPE>
inline void number<_TYPE>::multiplication(const _MyT& left, const _MyT& right)
{
	_number = left._number * right._number;
}


template<class _TYPE>
inline void number<_TYPE>::division(const _MyT& left, const _MyT& right)
{
	_number = left._number / right._number;
}


template<class _TYPE>
inline void number<_TYPE>::modulo(const _MyT& left, const _MyT& right)
{
	_number = (long int)left._number % (long int)right._number;
}


template<class _TYPE>
inline void number<_TYPE>::addition(const _MyT& left, const _MyT& right)
{
	_number = left._number + right._number;
}


template<class _TYPE>
inline void number<_TYPE>::substraction(const _MyT& left, const _MyT& right)
{
	_number = left._number - right._number;
}


template<class _TYPE>
inline void number<_TYPE>::exponentiation(const _MyT& left, const _MyT& right)
{
	_number = pow(left._number, right._number);
}


template<class _TYPE>
inline void number<_TYPE>::root(const _MyT& left, const _MyT& right)
{
	_number = pow(left._number, 1 / right._number);
}


template<class _TYPE>
inline void number<_TYPE>::assignation(const _MyT& left, const _MyT& right)
{
	_number = right._number;
}



// Functions:
template<class _TYPE>
inline void number<_TYPE>::abs(const _MyT& arg)
{
	_number = arg._number < 0 ? -arg._number : arg._number;
}


template<class _TYPE>
inline void number<_TYPE>::sgn(const _MyT& arg)
{
	_number = arg._number < 0 ? -1 : 1;
}


template<class _TYPE>
inline void number<_TYPE>::sin(const _MyT& arg)
{
	_number = ::sin(arg._number);
}


template<class _TYPE>
inline void number<_TYPE>::tg(const _MyT & arg)
{
	_number = ::tan(arg._number);
}


template<class _TYPE>
inline void number<_TYPE>::atg(const _MyT & arg)
{
	_number = ::atan(arg._number);
}


template<class _TYPE>
inline void number<_TYPE>::atg2(const _MyT& opposite, const _MyT& adjacent)
{
	_number = ::atan2(opposite._number, adjacent._number);
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




}
}
#include "pmb_calculate_magnitude.h"
#pragma once


namespace pmb
{
namespace calculate
{
namespace units
{




template<typename _CHAR>
inline dimension<_CHAR>::dimension()
	: _symbol(nullptr), _name(nullptr)
{
}


template<typename _CHAR>
inline dimension<_CHAR>::~dimension()
{
	clear();
}


template<typename _CHAR>
inline void dimension<_CHAR>::clear()
{
	if (_symbol)
		delete[] _symbol;
	if (_name)
		delete[] _name;
}


template<typename _CHAR>
inline bool dimension<_CHAR>::empty() const
{
	return !_symbol;
}


template<typename _CHAR>
inline bool dimension<_CHAR>::operator==(const _CHAR* dimension) const
{
	return dimension ? strcmp(_symbol, dimension) == 0 || (_name ? strcmp(_name, dimension) == 0 : false) : false;
}

template<typename _CHAR>
inline bool dimension<_CHAR>::operator!=(const _CHAR* dimension) const
{
	return !dimension || strcmp(_symbol, dimension) != 0 && (_name ? strcmp(_name, dimension) != 0 : true);
}


template<typename _CHAR>
inline void dimension<_CHAR>::set(const _CHAR* symbol, unsigned short len, const _CHAR* name)
{
	clear();
	_symbol = new _CHAR[len + 1];
	strncpy(_symbol, symbol, len);
	_symbol[(int)symbol] = '\0';
	if (name)
	{
		_name = new _CHAR[strlen(name) + 1];
		strcpy(_name, name);
	}
	else
		_name = nullptr;
}


template<typename _CHAR>
inline const _CHAR* dimension<_CHAR>::getSymbol() const
{
	return _symbol;
}


template<typename _CHAR>
inline const _CHAR* dimension<_CHAR>::getName() const
{
	return _name;
}















































template<typename _INT>
inline rational<_INT>::rational()
{
}


template<typename _INT>
inline rational<_INT>::rational(const _INT & _numerator, const _INT & _denominator)
	: numerator(_numerator), denominator(_denominator)
{
}


template<typename _INT>
inline void rational<_INT>::normalize(_INT& _numerator, _INT& _denominator)
{
	static const _INT prime[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997 };

	if (_denominator < 0)
	{
		_numerator = -_numerator;
		_denominator = -_denominator;
	}
	if (_denominator != 1 && _numerator != 1 && _numerator != -1 && _numerator != 0)
	{
		if (_numerator % _denominator != 0 && _denominator % _numerator != 0)
		{
			for (unsigned short p = 0; p < sizeof(prime) / sizeof(*prime); ++p)
			{
				while (_denominator % prime[p] == 0 && _numerator % prime[p] == 0)
				{
					_denominator /= prime[p];
					_numerator /= prime[p];
				}
				if (_denominator < prime[p] || _numerator < prime[p])
					break;
			}
			for (_INT np = prime[sizeof(prime) / sizeof(*prime) - 1] + 2; 2 * np < _denominator && 2 * np < _numerator; np += 2)
			{
				while (_denominator % np == 0 && _numerator % np == 0)
				{
					_denominator /= np;
					_numerator /= np;
				}
			}
		}
		else
		{
			if (_denominator < _numerator)
			{
				_numerator /= _denominator;
				_denominator = 1;
			}
			else
			{
				_denominator /= _numerator;
				_numerator = 1;
			}
		}
	}
}



template<typename _INT>
inline void rational<_INT>::normalize()
{
	normalize(numerator, denominator);
}



template<typename _INT>
inline rational<_INT>::operator bool() const
{
	return numerator;
}

template<typename _INT>
inline bool rational<_INT>::operator!() const
{
	return numerator == 0;
}


template<typename _INT>
inline bool rational<_INT>::operator==(const rational<_INT>& right) const
{
	return numerator == right.numerator && denominator == right.denominator;
}


template<typename _INT>
inline rational<_INT>& rational<_INT>::operator=(const rational<_INT>& right)
{
	numerator = right.numerator;
	denominator = right.denominator;
	return *this;
}


template<typename _INT>
inline rational<_INT>& rational<_INT>::operator=(const _INT& right)
{
	numerator = right;
	denominator = 1;
	return *this;
}

template<typename _INT>
inline rational<_INT>& rational<_INT>::operator+=(const rational<_INT>& right)
{
	numerator = numerator * right.denominator + right.numerator * denominator;
	denominator = denominator * right.denominator;
	normalize();
	return *this;
}


template<typename _INT>
inline rational<_INT> rational<_INT>::operator+(const rational<_INT>& right) const
{
	_INT n = numerator * right.denominator + right.numerator * denominator;
	_INT d = denominator * right.denominator;
	normalize(n, d);
	return rational<_INT>(n, d);
}


template<typename _INT>
inline rational<_INT> rational<_INT>::operator-(const rational<_INT>& right) const
{
	_INT n = numerator * right.denominator + right.numerator * denominator;
	_INT d = denominator * right.denominator;
	normalize(n, d);
	return rational<_INT>(n, d);
}


template<typename _INT>
inline rational<_INT> rational<_INT>::operator*(const rational<_INT>& right) const
{
	_INT n = numerator * right.denominator;
	_INT d = denominator * right.denominator;
	normalize(n, d);
	return rational<_INT>(n, d);
}


template<typename _INT>
inline rational<_INT> rational<_INT>::operator/(const rational<_INT>& right) const
{
	if (!right)
		throw "divide by zero";
	_INT n = numerator * right.denominator;
	_INT d = denominator * right.numerator;
	normalize(n, d);
	return rational<_INT>(n, d);
}


template<typename _INT>
inline rational<_INT> rational<_INT>::operator*(const _INT& right) const
{
	_INT n = numerator * right;
	_INT d = denominator;
	normalize(n, d);
	return rational<_INT>(n, d);
}



template<typename _INT>
inline rational<_INT> rational<_INT>::operator/(const _INT& right) const
{
	if (!right)
		throw "divide by zero";
	_INT n = numerator;
	_INT d = denominator * right;
	normalize(n, d);
	return rational<_INT>(n, d);
}








template<typename _INT>
inline rational<_INT> operator+(const _INT &left, const rational<_INT>& right)
{
	_INT numerator = left * right.denominator + right.numerator;
	_INT denominator = right.denominator;
	rational<_INT>::normalize(numerator, denominator);
	return rational<_INT>(numerator, denominator);
}



template<typename _INT>
inline rational<_INT> operator-(const _INT &left, const rational<_INT>& right)
{
	_INT numerator = left * right.denominator - right.numerator;
	_INT denominator = right.denominator;
	rational<_INT>::normalize(numerator, denominator);
	return rational<_INT>(numerator, denominator);
}



template<typename _INT>
inline rational<_INT> operator*(const _INT &left, const rational<_INT>& right)
{
	_INT numerator = left * right.numerator;
	_INT denominator = right.denominator;
	rational<_INT>::normalize(numerator, denominator);
	return rational<_INT>(numerator, denominator);
}



template<typename _INT>
inline rational<_INT> operator/(const _INT &left, const rational<_INT>& right)
{
	if (!right)
		throw "divide by zero";
	_INT numerator = left * right.denominator;
	_INT denominator = right.numerator;
	rational<_INT>::normalize(numerator, denominator);
	return rational<_INT>(numerator, denominator);
}








































template<typename _INT, typename _CHAR>
inline power_dimension<_INT, _CHAR>::power_dimension()
	: rational()
{
}


template<typename _INT, typename _CHAR>
inline power_dimension<_INT, _CHAR>::power_dimension(const rational<_INT>& q, const dimension<_CHAR>* pDim)
	: rational(q), dim(pDim)
{
}



template<typename _INT, typename _CHAR>
inline power_dimension<_INT, _CHAR>& power_dimension<_INT, _CHAR>::operator=(const power_dimension<_INT, _CHAR>& right)
{
	*static_cast<rational<_INT>*>(this) = *static_cast<const rational<_INT>*>(&right);
	dim = right.dim;
	return *this;
}

template<typename _INT, typename _CHAR>
inline power_dimension<_INT, _CHAR>& power_dimension<_INT, _CHAR>::operator+=(const power_dimension<_INT, _CHAR>& right)
{
	*static_cast<rational<_INT>*>(this) += *static_cast<const rational<_INT>*>(&right);
	dim = right.dim;
	return *this;
}


template<typename _INT, typename _CHAR>
inline power_dimension<_INT, _CHAR> power_dimension<_INT, _CHAR>::operator*(const _INT& right) const
{
	rational<_INT> result = *static_cast<const rational<_INT>*>(this) * right;
	return power_dimension<_INT, _CHAR>(result, dim);
}


template<typename _INT, typename _CHAR>
inline power_dimension<_INT, _CHAR> power_dimension<_INT, _CHAR>::operator/(const _INT& right) const
{
	rational<_INT> result = *static_cast<rational<_INT>*>(this) / right;
	return power_dimension<_INT, _CHAR>(result, dim);
}




template<typename _INT, typename _CHAR>
inline power_dimension<_INT, _CHAR> operator*(const _INT &left, const power_dimension<_INT, _CHAR>& right)
{
	rational<_INT> result = left * *static_cast<const rational<_INT>*>(&right);
	return power_dimension<_INT, _CHAR>(result, right.dim);
}


template<typename _INT, typename _CHAR>
inline power_dimension<_INT, _CHAR> operator/(const _INT &left, const power_dimension<_INT, _CHAR>& right)
{
	rational<_INT> result = left / *static_cast<rational<_INT>*>(&right);
	return power_dimension<_INT, _CHAR>(result, right.dim);
}















































template<typename _CHAR, typename _POWER>
inline prefix<_CHAR, _POWER>::prefix(const _CHAR* symbol, const _CHAR* name, _POWER power)
	: _power(power)
{
	_symbol = new _CHAR[strlen(symbol) + 1];
	strcpy(_symbol, symbol);
	if (name)
	{
		_name = new _CHAR[strlen(name) + 1];
		strcpy(_name, name);
	}
	else
		_name = nullptr;
}


template<typename _CHAR, typename _POWER>
inline prefix<_CHAR, _POWER>::~prefix()
{
	delete[]_symbol;
	if (_name)
		delete[]_name;
}


template<typename _CHAR, typename _POWER>
inline short prefix<_CHAR, _POWER>::find(const _CHAR* str) const
{
	short i;
	for (i = 0; _symbol[i] && str[i] && str[i] == _symbol[i]; ++i)
		;
	return !_symbol[i] && str[i] ? i : -1;
}


template<typename _CHAR, typename _POWER>
inline short prefix<_CHAR, _POWER>::findName(const _CHAR* str) const
{
	short i;
	for (i = 0; _name[i] && str[i] && str[i] == _name[i]; ++i)
		;
	return !_name[i] && str[i] ? i : -1;
}


template<typename _CHAR, typename _POWER>
inline double prefix<_CHAR, _POWER>::getFactor(short base) const
{
	return ::pow((double)base, (double)_power);
}
















































template<typename _INT, typename _CHAR>
inline unit<_INT, _CHAR>::unit(power_dimension<_INT, _CHAR>* dim, const ndim& nDims)
	: _dim(dim), _nDims(nDims), _capacity(nDims)
{
}


template<typename _INT, typename _CHAR>
inline unit<_INT, _CHAR>::unit(const dimension<_CHAR>* dim)
	: _nDims(1)
{
	_dim = new power_dimension<_INT, _CHAR>[_capacity = _nDims];
	_dim[0].pow = _INT(1);
	_dim[0].dim = dim;
}


template<typename _INT, typename _CHAR>
inline unit<_INT, _CHAR>::unit(const unit& u)
{
	if (u._dim)
	{
		_dim = new power_dimension<_INT, _CHAR>[_capacity = _nDims = u._nDims];
		for (ndim d = 0; d < _nDims; ++d)
			_dim[d] = u._dim[d];
	}
	else
	{
		_dim = nullptr;
		_capacity = _nDims = 0;
	}
}


template<typename _INT, typename _CHAR>
inline unit<_INT, _CHAR>::unit()
	: _dim(nullptr), _nDims(0), _capacity(0)
{
}


template<typename _INT, typename _CHAR>
inline unit<_INT, _CHAR>::~unit()
{
	if (_dim)
		delete[] _dim;
}



template<typename _INT, typename _CHAR>
inline typename unit<_INT, _CHAR>::ndim unit<_INT, _CHAR>::nDims() const
{
	return _nDims;
}


template<typename _INT, typename _CHAR>
inline bool unit<_INT, _CHAR>::operator==(const unit& right) const
{
	bool bRet = _nDims == right._nDims;
	for (ndim i = 0; i < _nDims && bRet; ++i)
	{
		ndim j;
		for (j = 0; j < right._nDims; ++j)
		{
			if (_dim[i].dim == right._dim[j].dim)
			{
				bRet = _dim[i] == right._dim[j];
				break;
			}
		}
		if (bRet)
			bRet = j < right._nDims;
	}
	return bRet;
}


template<typename _INT, typename _CHAR>
inline bool unit<_INT, _CHAR>::operator!=(const unit& right) const
{
	return !operator==(right);
}



template<typename _INT, typename _CHAR>
inline typename unit<_INT, _CHAR>::ndim unit<_INT, _CHAR>::calcNewDim(const unit& right) const
{
	ndim notFoundInRight = 0;
	for (ndim i = 0; i < _nDims; ++i)
	{
		ndim j;
		for (j = 0; j < right._nDims && _dim[i].dim != right._dim[j].dim; ++j)
			;
		if (j == right._nDims && j)
			++notFoundInRight;
	}
	ndim newDim = max(_nDims, right._nDims);
	if (newDim < notFoundInRight + right._nDims)
		newDim = notFoundInRight + right._nDims;
	return newDim;
}



template<typename _INT, typename _CHAR>
inline void unit<_INT, _CHAR>::populateNewDim(power_dimension<_INT, _CHAR>* result, ndim nNewDim, const unit& right, const _INT& mult) const
{
	for (ndim i = 0; i < _nDims; ++i)
	{
		result[i] = _dim[i];
		int j;
		for (j = 0; j < right._nDims && _dim[i].dim != right._dim[j].dim; ++j)
			;
		if (j < right._nDims)
			result[i] += mult * right._dim[j];
	}
	if (_nDims < nNewDim)
	{
		for (ndim i = 0, k = 0; i < right._nDims; ++i)
		{
			ndim j;
			for (j = 0; j < _nDims && right._dim[i].dim != _dim[j].dim; ++j)
				;
			if (j == _nDims)
			{
				result[_nDims + k].dim = right._dim[i].dim;
				result[_nDims + k] = mult * right._dim[i];
				++k;
			}
		}
	}
}


template<typename _INT, typename _CHAR>
inline void unit<_INT, _CHAR>::clean()
{
	compressVector(this, &_nDims);
}


template<typename _INT, typename _CHAR>
inline bool unit<_INT, _CHAR>::compressVector(power_dimension<_INT, _CHAR>* dim, ndim* nDim)
{
	for (ndim i = 0; i < *nDim; ++i)
	{
		if ((!dim[i] || dim[i].dim == nullptr) && i + 1 < *nDim)
		{
			for (int j = i + 1; j < *nDim; ++j)
				dim[j - 1] = dim[j];
			--i;
			--*nDim;
		}
	}
	return !*nDim;
}



template<typename _INT, typename _CHAR>
inline void unit<_INT, _CHAR>::calc(power_dimension<_INT, _CHAR>** result, ndim* nNewDim, const unit& right, const _INT& mult) const
{
	*nNewDim = calcNewDim(right);
	*result = new power_dimension<_INT, _CHAR>[*nNewDim];
	populateNewDim(*result, *nNewDim, right, mult);
	if (compressVector(*result, nNewDim))
	{
		delete[]* result;
		*result = nullptr;
	}
}


template<typename _INT, typename _CHAR>
inline unit<_INT, _CHAR>& unit<_INT, _CHAR>::operator=(const unit& right)
{
	if (this != &right)
	{
		if (_nDims != right._nDims)
		{
			if (_capacity < right._nDims)
			{
				if (_dim)
					delete[]_dim;
				_dim = new power_dimension<_INT, _CHAR>[_capacity = _nDims = right._nDims];
			}
			else
				_nDims = right._nDims;
		}
		for (ndim i = 0; i < _nDims; ++i)
			_dim[i] = right._dim[i];
	}
	return *this;
}


template<typename _INT, typename _CHAR>
inline unit<_INT, _CHAR>& unit<_INT, _CHAR>::operator=(const dimension<_CHAR>* right)
{
	if (_nDims != 1)
	{
		if (_capacity < 1)
		{
			if (_dim)
				delete[]_dim;
			_dim = new power_dimension<_INT, _CHAR>[_capacity = _nDims = right._nDims];
		}
		else
			_nDims = right._nDims;
	}
	_dim[0].dim = right;
	_dim[0] = _INT(1);
	return *this;
}


template<typename _INT, typename _CHAR>
inline void unit<_INT, _CHAR>::clear()
{
	if (_dim)
	{
		delete[]_dim;
		_dim = nullptr;
	}
	_capacity = _nDims = ndim(0);
}


template<typename _INT, typename _CHAR>
inline unit<_INT, _CHAR> unit<_INT, _CHAR>::operator*(const unit& right) const
{
	ndim newDim;
	power_dimension<_INT, _CHAR>* dim;
	calc(&dim, &newDim, right, _INT(1));
	return unit(dim, newDim);
}


template<typename _INT, typename _CHAR>
inline unit<_INT, _CHAR> unit<_INT, _CHAR>::operator/(const unit& right) const
{
	ndim newDim;
	power_dimension<_INT, _CHAR>* dim;
	calc(&dim, &newDim, right, _INT(-1));
	return unit(dim, newDim);
}


template<typename _INT, typename _CHAR>
inline unit<_INT, _CHAR> unit<_INT, _CHAR>::pow(const _INT& p) const
{
	power_dimension<_INT, _CHAR>* dim = p ? new power_dimension<_INT, _CHAR>[_nDims] : nullptr;
	for (ndim i = 0; p && i < _nDims; ++i)
	{
		dim[i].dim = _dim[i].dim;
		dim[i] = _dim[i] * p;
	}
	return unit(dim, p ? _nDims : 0);
}



template<typename _INT, typename _CHAR>
inline unit<_INT, _CHAR> unit<_INT, _CHAR>::root(const _INT& p) const
{
	power_dimension<_INT, _CHAR>* dim = p ? new power_dimension<_INT, _CHAR>[_nDims] : nullptr;
	for (ndim i = 0; p && i < _nDims; ++i)
	{
		dim[i].dim = _dim[i].dim;
		dim[i] = _dim[i] / p;
	}
	return unit(dim, p ? _nDims : 0);
}




















































template<class _TYPE, typename _INT, typename _CHAR>
inline magnitude<_TYPE, _INT, _CHAR>::magnitude()
{
}


template<class _TYPE, typename _INT, typename _CHAR>
inline magnitude<_TYPE, _INT, _CHAR>::magnitude(const _TYPE& src)
	: _number(src)
{
}


template<class _TYPE, typename _INT, typename _CHAR>
inline magnitude<_TYPE, _INT, _CHAR>::magnitude(const _MyT* src)
	: _number(src->_number), _unit(src->_unit)
{
}


template<class _TYPE, typename _INT, typename _CHAR>
inline magnitude<_TYPE, _INT, _CHAR>::magnitude(const _CHAR* str, unsigned short len)
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
	_TYPE decimal = _TYPE(1);
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
template<class _TYPE, typename _INT, typename _CHAR>
inline void magnitude<_TYPE, _INT, _CHAR>::positive(const _MyT& right)
{
	_number = +right._number;
}


template<class _TYPE, typename _INT, typename _CHAR>
inline void magnitude<_TYPE, _INT, _CHAR>::negative(const _MyT& right)
{
	_number = -right._number;
}


template<class _TYPE, typename _INT, typename _CHAR>
inline void magnitude<_TYPE, _INT, _CHAR>::factorial(const _MyT& right)
{
	long res = 1;
	for (int i = 2; i <= right._number; ++i)
		res *= i;
	_number = res;
}


template<class _TYPE, typename _INT, typename _CHAR>
inline void magnitude<_TYPE, _INT, _CHAR>::multiplication(const _MyT& left, const _MyT& right)
{
	_number = left._number * right._number;
	_unit = left._unit * right._unit;
}


template<class _TYPE, typename _INT, typename _CHAR>
inline void magnitude<_TYPE, _INT, _CHAR>::division(const _MyT& left, const _MyT& right)
{
	_number = left._number / right._number;
	_unit = left._unit / right._unit;
}


template<class _TYPE, typename _INT, typename _CHAR>
inline void magnitude<_TYPE, _INT, _CHAR>::modulo(const _MyT& left, const _MyT& right)
{
	_number = (long int)left._number % (long int)right._number;
	_unit = left._unit;
}


template<class _TYPE, typename _INT, typename _CHAR>
inline void magnitude<_TYPE, _INT, _CHAR>::addition(const _MyT& left, const _MyT& right)
{
	if (left._unit != right._unit)
		throw "incompatible units";
	_number = left._number + right._number;
}


template<class _TYPE, typename _INT, typename _CHAR>
inline void magnitude<_TYPE, _INT, _CHAR>::substraction(const _MyT& left, const _MyT& right)
{
	if (left._unit != right._unit)
		throw "incompatible units";
	_number = left._number - right._number;
}


template<class _TYPE, typename _INT, typename _CHAR>
inline void magnitude<_TYPE, _INT, _CHAR>::exponentiation(const _MyT& left, const _MyT& right)
{
	_number = pow(left._number, right._number);
	_unit = left._unit.pow(right._number);
}


template<class _TYPE, typename _INT, typename _CHAR>
inline void magnitude<_TYPE, _INT, _CHAR>::root(const _MyT& left, const _MyT& right)
{
	_number = pow(left._number, 1 / right._number);
	_unit = left.root(right._number);
}


template<class _TYPE, typename _INT, typename _CHAR>
inline void magnitude<_TYPE, _INT, _CHAR>::assignation(const _MyT& left, const _MyT& right)
{
	_number = right._number;
	_unit = right._unit;
}



// Functions:
template<class _TYPE, typename _INT, typename _CHAR>
inline void magnitude<_TYPE, _INT, _CHAR>::abs(const _MyT& arg)
{
	_number = arg._number < 0 ? -arg._number : arg._number;
	_unit = arg._unit;
}


template<class _TYPE, typename _INT, typename _CHAR>
inline void magnitude<_TYPE, _INT, _CHAR>::sgn(const _MyT& arg)
{
	_number = arg._number < 0 ? -1 : 1;
	_unit = arg._unit;
}


template<class _TYPE, typename _INT, typename _CHAR>
inline void magnitude<_TYPE, _INT, _CHAR>::sin(const _MyT& arg)
{
	_number = ::sin(arg._number);
}


template<class _TYPE, typename _INT, typename _CHAR>
inline void magnitude<_TYPE, _INT, _CHAR>::tg(const _MyT & arg)
{
	_number = ::tan(arg._number);
}


template<class _TYPE, typename _INT, typename _CHAR>
inline void magnitude<_TYPE, _INT, _CHAR>::atg(const _MyT & arg)
{
	_number = ::atan(arg._number);
}


template<class _TYPE, typename _INT, typename _CHAR>
inline void magnitude<_TYPE, _INT, _CHAR>::atg2(const _MyT& opposite, const _MyT& adjacent)
{
	_number = ::atan2(opposite._number, adjacent._number);
}



// Check:
template<class _TYPE, typename _INT, typename _CHAR>
inline bool magnitude<_TYPE, _INT, _CHAR>::zero() const
{
	return _number == _TYPE(0);
}

template<class _TYPE, typename _INT, typename _CHAR>
inline bool magnitude<_TYPE, _INT, _CHAR>::positive() const
{
	return 0 < _number;
}


template<class _TYPE, typename _INT, typename _CHAR>
inline bool magnitude<_TYPE, _INT, _CHAR>::negative() const
{
	return _number < 0;
}


template<class _TYPE, typename _INT, typename _CHAR>
inline bool magnitude<_TYPE, _INT, _CHAR>::integer() const
{
	return ceil(_number) == floor(_number);
}



template<class _TYPE, typename _INT, typename _CHAR>
inline bool magnitude<_TYPE, _INT, _CHAR>::dimensionless() const
{
	return !_unit.nDims();
}



template<class _TYPE, typename _INT, typename _CHAR>
inline bool magnitude<_TYPE, _INT, _CHAR>::scalar() const
{
	return true;
}



template<class _TYPE, typename _INT, typename _CHAR>
inline bool magnitude<_TYPE, _INT, _CHAR>::real() const
{
	return true;
}



template<class _TYPE, typename _INT, typename _CHAR>
inline bool magnitude<_TYPE, _INT, _CHAR>::imaginary() const
{
	return false;
}



template<class _TYPE, typename _INT, typename _CHAR>
inline bool magnitude<_TYPE, _INT, _CHAR>::complex() const
{
	return false;
}



template<class _TYPE, typename _INT, typename _CHAR>
inline bool magnitude<_TYPE, _INT, _CHAR>::vector() const
{
	return false;
}



template<class _TYPE, typename _INT, typename _CHAR>
inline bool magnitude<_TYPE, _INT, _CHAR>::matrix() const
{
	return false;
}





}
}
}

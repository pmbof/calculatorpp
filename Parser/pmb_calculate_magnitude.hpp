#include "pmb_calculate_magnitude.h"
#pragma once


namespace pmb
{
namespace calculate
{
namespace units
{




#ifdef _WINDOWS
	template<typename _CHAR, typename _SIZE>
	void strncpy(_CHAR* target, const _CHAR* source, _SIZE size)
	{
		for (_SIZE i = 0; i < size; ++i)
			target[i] = source[i];
	}
#endif




template<typename _CHAR, typename _SIZE>
inline dimension<_CHAR, _SIZE>::dimension()
	: _symbol(nullptr), _name(nullptr)
{
}

template<typename _CHAR, typename _SIZE>
inline dimension<_CHAR, _SIZE>::dimension(const _CHAR* symbol, const _CHAR* name)
	: _symbol(nullptr), _name(nullptr)
{
	_SIZE slen = strlen(symbol),
		nlen = name ? strlen(name) : 0;
	set(symbol, slen, name, nlen);
}

template<typename _CHAR, typename _SIZE>
inline dimension<_CHAR, _SIZE>::dimension(const _CHAR* symbol, const _SIZE& slen, const _CHAR* name, const _SIZE& nlen)
	: _symbol(nullptr), _name(nullptr)
{
	set(symbol, slen, name, nlen);
}


template<typename _CHAR, typename _SIZE>
inline dimension<_CHAR, _SIZE>::~dimension()
{
	clear();
}


template<typename _CHAR, typename _SIZE>
inline void dimension<_CHAR, _SIZE>::clear()
{
	if (_symbol)
		delete[] _symbol;
	if (_name)
		delete[] _name;
}


template<typename _CHAR, typename _SIZE>
inline bool dimension<_CHAR, _SIZE>::empty() const
{
	return !_symbol;
}


template<typename _CHAR, typename _SIZE>
inline bool dimension<_CHAR, _SIZE>::operator==(const _CHAR* dimension) const
{
	return dimension ? strcmp(_symbol, dimension) == 0 || (_name ? strcmp(_name, dimension) == 0 : false) : false;
}

template<typename _CHAR, typename _SIZE>
inline bool dimension<_CHAR, _SIZE>::operator!=(const _CHAR* dimension) const
{
	return !dimension || strcmp(_symbol, dimension) != 0 && (_name ? strcmp(_name, dimension) != 0 : true);
}

template<typename _CHAR, typename _SIZE>
inline bool dimension<_CHAR, _SIZE>::operator<(const dimension& right) const
{
	const _CHAR* l, *r;
	for (l = _symbol, r = right._symbol; l - _symbol < _slen && r - right._symbol < right._slen && *l == *r; ++l, ++r)
		;
	return l - _symbol == _slen && r - right._symbol < right._slen || *l < *r;
}

template<typename _CHAR, typename _SIZE>
inline bool dimension<_CHAR, _SIZE>::less(const _CHAR* right, const _SIZE& len)
{
	const _CHAR* l, *r;
	for (l = _symbol, r = right; l - _symbol < _slen && r - right < len && *l == *r; ++l, ++r)
		;
	return r - right < len && (_slen <= l - _symbol || *l < *r);
}

template<typename _CHAR, typename _SIZE>
inline bool dimension<_CHAR, _SIZE>::greater(const _CHAR* right, const _SIZE& len)
{
	const _CHAR* l, *r;
	for (l = _symbol, r = right; l - _symbol < _slen && r - right < len && *l == *r; ++l, ++r)
		;
	return l - _symbol < _slen && (len <= r - right || *r < *l);
}

template<typename _CHAR, typename _SIZE>
inline bool dimension<_CHAR, _SIZE>::less_name(const dimension& right) const
{
	const _CHAR* l, *r;
	for (l = _name, r = right._name; l && r && l - _name < _nlen && r - right._name < right._nlen && *l == *r; ++l, ++r)
		;
	return !l && r || l && r && r - right._name < right._nlen && (_nlen < l - _name || *l < *r);
}

template<typename _CHAR, typename _SIZE>
inline bool dimension<_CHAR, _SIZE>::less_name(const _CHAR* right, const _SIZE& len)
{
	const _CHAR* l, *r;
	for (l = _name, r = right; l && l - _name < _nlen && r - right < len && *l == *r; ++l, ++r)
		;
	return !l || r - right < len && (_nlen <= l - _name || *l < *r);
}

template<typename _CHAR, typename _SIZE>
inline bool dimension<_CHAR, _SIZE>::greater_name(const _CHAR* right, const _SIZE& len)
{
	const _CHAR* l, *r;
	for (l = _name, r = right; l && l - _name < _nlen && r - right < len && *l == *r; ++l, ++r)
		;
	return l && len && l - _name < _nlen && (len <= r - right || *r < *l);
}




template<typename _CHAR, typename _SIZE>
inline void dimension<_CHAR, _SIZE>::set(const _CHAR* symbol, const _SIZE& slen, const _CHAR* name, const _SIZE& nlen)
{
	clear();

	_slen = slen;
	_symbol = new _CHAR[_slen];
	strncpy(_symbol, symbol, _slen);
	if (name && nlen)
	{
		_nlen = nlen;
		_name = new _CHAR[_nlen];
		strncpy(_name, name, _nlen);
	}
	else
		_name = nullptr;
}



template<typename _CHAR, typename _SIZE>
inline void dimension<_CHAR, _SIZE>::set(unsigned char index)
{
	_index = index;
}



template<typename _CHAR, typename _SIZE>
inline unsigned char dimension<_CHAR, _SIZE>::index() const
{
	return _index;
}



template<typename _CHAR, typename _SIZE>
inline const _CHAR* dimension<_CHAR, _SIZE>::symbol() const
{
	return _symbol;
}


template<typename _CHAR, typename _SIZE>
inline const _CHAR* dimension<_CHAR, _SIZE>::name() const
{
	return _name;
}


template<typename _CHAR, typename _SIZE>
inline const _SIZE dimension<_CHAR, _SIZE>::symbol_size() const
{
	return _symbol ? _slen: 0;
}


template<typename _CHAR, typename _SIZE>
inline const _SIZE dimension<_CHAR, _SIZE>::name_size() const
{
	return _name ? _nlen : 0;
}
















































template<typename _INT>
inline rational<_INT>::rational()
{
}


template<typename _INT>
inline rational<_INT>::rational(const _INT& _numerator)
	: numerator(_numerator), denominator(1)
{
}


template<typename _INT>
inline rational<_INT>::rational(const _INT & _numerator, const _INT & _denominator)
	: numerator(_numerator), denominator(_denominator)
{
}


template<typename _INT>
template<typename _N>
inline rational<_INT>::rational(const _N& number)
{
	if (!number)
	{
		numerator = 0;
		denominator = 1;
	}
	else
	{
		int expmx = ::log10(::pow(2, 8 * sizeof(_INT)));
		double exp = ::log10((double)number);
		if (exp < 0)
		{
			if (-expmx  < exp)
			{
				for (numerator = 1; numerator / number != (denominator = numerator / (_N)(number)); numerator *= 10)
					;
			}
			else
			{
				numerator = 0;
				denominator = 1;
			}
		}
		if (exp < expmx)
		{
			for (denominator = 1; number * denominator != (numerator = (_N)(number) * denominator); denominator *= 10)
				;
		}
		else
		{
			numerator = 0;
			denominator = 0;
		}
		normalize();
	}
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
	if (_denominator < 0 && 0 < _numerator)
	{
		_numerator = -_numerator;
		_denominator = -_denominator;
	}
}



template<typename _INT>
inline void rational<_INT>::normalize()
{
	normalize(numerator, denominator);
}



template<typename _INT>
inline std::string rational<_INT>::serialize() const
{
	return std::string(reinterpret_cast<const char*>(&numerator), sizeof(_INT)) + std::string(reinterpret_cast<const char*>(&denominator), sizeof(_INT));
}



template<typename _INT>
inline bool rational<_INT>::deserialize(const char* bytes, unsigned short size)
{
	if (size != 2 * sizeof(_INT))
		return false;
	numerator = reinterpret_cast<const _INT*>(bytes)[0];
	denominator = reinterpret_cast<const _INT*>(bytes)[1];
	return true;
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
inline rational<_INT>::operator long() const
{
	if (!denominator)
		throw "divide by zero";
	return numerator / denominator;
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
inline rational<_INT> rational<_INT>::operator-() const
{
	if (denominator < 0)
		return rational<_INT>(numerator, -denominator);
	return rational<_INT>(-numerator, denominator);
}


template<typename _INT>
inline rational<_INT> rational<_INT>::operator+() const
{
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
	_INT n = numerator * right.numerator;
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
inline rational<_INT> rational<_INT>::operator*=(const _INT& right)
{
	numerator *= right;
	normalize(numerator, denominator);
	return *this;
}



template<typename _INT>
template<typename _N>
inline rational<_INT> rational<_INT>::operator*(const _N& right) const
{
	rational<_INT> nright(right);
	return *this * nright;
}



template<typename _INT>
template<typename _N>
inline rational<_INT> rational<_INT>::operator/(const _N& right) const
{
	if (!right)
		throw "divide by zero";
	rational<_INT> nright(right);
	return *this / nright;
}





template<typename _INT>
inline bool rational<_INT>::operator<(const rational<_INT>& right) const
{
	return 0 < denominator && 0 < right.denominator || denominator < 0 && right.denominator < 0 
			? numerator * right.denominator < right.numerator * denominator
			: right.numerator * denominator < numerator * right.denominator;
}


template<typename _INT>
inline bool rational<_INT>::operator<(const _INT& right) const
{
	return 0 < denominator ? numerator < right * denominator : right * denominator < numerator;
}



template<typename _INT>
inline bool rational<_INT>::operator<=(const rational<_INT>& right) const
{
	return 0 < denominator && 0 < right.denominator || denominator < 0 && right.denominator < 0
		? numerator * right.denominator <= right.numerator* denominator
		: right.numerator* denominator <= numerator* right.denominator;
}



template<typename _INT>
inline bool rational<_INT>::operator<=(const _INT& right) const
{
	return 0 < denominator ? numerator <= right* denominator : right * denominator <= numerator;
}


template<typename _INT>
inline bool rational<_INT>::operator>(const rational<_INT>& right) const
{
	return 0 < denominator && 0 < right.denominator || denominator < 0 && right.denominator < 0
		? right.numerator * denominator < numerator * right.denominator
		: numerator * right.denominator < right.numerator * denominator;
}


template<typename _INT>
inline bool rational<_INT>::operator>(const _INT& right) const
{
	return 0 < denominator ? right * denominator < numerator : numerator < right * denominator;
}



template<typename _INT>
inline bool rational<_INT>::operator>=(const rational<_INT>& right) const
{
	return 0 < denominator && 0 < right.denominator || denominator < 0 && right.denominator < 0
		? right.numerator * denominator <= numerator* right.denominator
		: numerator * right.denominator <= right.numerator* denominator;
}



template<typename _INT>
inline bool rational<_INT>::operator>=(const _INT& right) const
{
	return 0 < denominator ? right * denominator <= numerator : numerator <= right * denominator;
}



template<typename _INT>
inline bool rational<_INT>::zero() const
{
	return !numerator;
}

template<typename _INT>
inline bool rational<_INT>::unit() const
{
	return numerator == denominator;
}

template<typename _INT>
inline bool rational<_INT>::natural() const
{
	return denominator == 1 && 0 <= numerator || denominator == numerator || denominator == -1 && numerator < 0;
}

template<typename _INT>
inline bool rational<_INT>::integer() const
{
	return denominator == 1 || denominator == -1;
}



template<typename _INT>
inline rational<_INT> rational<_INT>::pow(const _INT& exp) const
{
	_MyT result;
	if (!numerator)
		result = _MyT(0);
	else if (!exp)
		result = _MyT(1);
	else
	{
		if (exp == 1)
			result = *this;
		else if (exp == -1)
		{
			result.numerator = denominator;
			result.denominator = numerator;
		}
		else
		{
			if (exp < 0)
			{
				result.numerator = ::pow(denominator, -exp);
				if (numerator == 1)
					result.denominator = 1;
				else
				{
					result.denominator = ::pow(numerator, -exp);
					result.normalize();
				}
			}
			else
			{
				result.numerator = ::pow(numerator, exp);
				if (denominator == 1)
					result.denominator = 1;
				else
				{
					result.denominator = ::pow(denominator, exp);
					result.normalize();
				}
			}
		}
	}
	return result;
}



template<typename _INT>
inline rational<_INT> rational<_INT>::pow(const _MyT& exp) const
{
	_MyT result;
	if (!numerator)
		result = _MyT(0);
	else if (!exp.numerator)
		result = _MyT(1);
	else if (exp.denominator == 1)
	{
		if (exp.numerator == 1)
			result = *this;
		else
		{
			result.numerator = ::pow(numerator, exp.numerator);
			if (denominator == 1)
				result.denominator = 1;
			else
			{
				result.denominator = ::pow(denominator, exp.numerator);
				result.normalize();
			}
		}
	}
	else
	{
		result.numerator = ::pow(numerator, (double)exp.numerator / exp.denominator);
		if (denominator == 1)
			result.denominator = 1;
		else
		{
			result.denominator = ::pow(denominator, (double)exp.numerator / exp.denominator);
			result.normalize();
		}
	}
	return result;
}



template<typename _INT>
inline _INT rational<_INT>::proportionality(const rational<_INT>& right) const
{
	_INT vret;
	if (right.numerator && denominator && numerator % right.numerator == 0 && right.denominator % denominator == 0)
		vret = numerator / right.numerator * right.denominator / denominator;
	else
		vret = 0;
	return vret;
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



template<typename _INT>
std::stringstream& operator<<(std::stringstream& left, const rational<_INT>& right)
{
	if (!right.denominator)
		throw "divide by zero";

	left << right.numerator;
	if (right.denominator != 1)
		left << "/" << right.denominator;

	return left;
}









































template<typename _INT, typename _CHAR, typename _SZSTR>
inline power_dimension<_INT, _CHAR, _SZSTR>::power_dimension()
	: rational()
{
}


template<typename _INT, typename _CHAR, typename _SZSTR>
inline power_dimension<_INT, _CHAR, _SZSTR>::power_dimension(const rational<_INT>& q, const dimension* pDim)
	: rational(q), dim(pDim)
{
}



template<typename _INT, typename _CHAR, typename _SZSTR>
inline std::string power_dimension<_INT, _CHAR, _SZSTR>::serialize() const
{
	std::string bytes;
	bytes = rational<_INT>::serialize();
	bytes = (char)dim->index() + bytes;
	return bytes;
}



template<typename _INT, typename _CHAR, typename _SZSTR>
inline unsigned char power_dimension<_INT, _CHAR, _SZSTR>::deserialize(const char* bytes, unsigned short size)
{
	rational_base::deserialize(bytes + 1, size - 1);
	return static_cast<const unsigned char>(bytes[0]);
}



template<typename _INT, typename _CHAR, typename _SZSTR>
inline power_dimension<_INT, _CHAR, _SZSTR>& power_dimension<_INT, _CHAR, _SZSTR>::operator=(const power_dimension& right)
{
	*static_cast<rational<_INT>*>(this) = *static_cast<const rational<_INT>*>(&right);
	dim = right.dim;
	return *this;
}

template<typename _INT, typename _CHAR, typename _SZSTR>
inline power_dimension<_INT, _CHAR, _SZSTR>& power_dimension<_INT, _CHAR, _SZSTR>::operator+=(const power_dimension& right)
{
	*static_cast<rational<_INT>*>(this) += *static_cast<const rational<_INT>*>(&right);
	dim = right.dim;
	return *this;
}


template<typename _INT, typename _CHAR, typename _SZSTR>
template<typename _N>
inline power_dimension<_INT, _CHAR, _SZSTR> power_dimension<_INT, _CHAR, _SZSTR>::operator*(const _N& right) const
{
	rational<_INT> result = *static_cast<const rational<_INT>*>(this) * right;
	return power_dimension(result, dim);
}


template<typename _INT, typename _CHAR, typename _SZSTR>
template<typename _N>
inline power_dimension<_INT, _CHAR, _SZSTR> power_dimension<_INT, _CHAR, _SZSTR>::operator/(const _N& right) const
{
	rational<_INT> result = *static_cast<const rational<_INT>*>(this) / right;
	return power_dimension(result, dim);
}




template<typename _INT, typename _CHAR, typename _SZSTR>
inline power_dimension<_INT, _CHAR, _SZSTR> operator*(const _INT &left, const power_dimension<_INT, _CHAR, _SZSTR>& right)
{
	rational<_INT> result = left * *static_cast<const rational<_INT>*>(&right);
	return power_dimension<_INT, _CHAR, _SZSTR>(result, right.dim);
}


template<typename _INT, typename _CHAR, typename _SZSTR>
inline power_dimension<_INT, _CHAR, _SZSTR> operator/(const _INT &left, const power_dimension<_INT, _CHAR, _SZSTR>& right)
{
	rational<_INT> result = left / *static_cast<rational<_INT>*>(&right);
	return power_dimension<_INT, _CHAR, _SZSTR>(result, right.dim);
}














































template<typename _INT, typename _CHAR, typename _SZSTR>
inline unit<_INT, _CHAR, _SZSTR>::unit(power_dimension* dim, const ndim& nDims)
	: _dim(dim), _nDims(nDims), _capacity(nDims)
{
}


template<typename _INT, typename _CHAR, typename _SZSTR>
inline unit<_INT, _CHAR, _SZSTR>::unit(const dimension* dim)
	: _nDims(1)
{
	_dim = new power_dimension[_capacity = _nDims];
	_dim[0].dim = dim;
}


template<typename _INT, typename _CHAR, typename _SZSTR>
inline unit<_INT, _CHAR, _SZSTR>::unit(const unit& u)
{
	if (u._dim)
	{
		_dim = new power_dimension[_capacity = _nDims = u._nDims];
		for (ndim d = 0; d < _nDims; ++d)
			_dim[d] = u._dim[d];
	}
	else
	{
		_dim = nullptr;
		_capacity = _nDims = 0;
	}
}


template<typename _INT, typename _CHAR, typename _SZSTR>
inline unit<_INT, _CHAR, _SZSTR>::unit()
	: _dim(nullptr), _nDims(0), _capacity(0)
{
}


template<typename _INT, typename _CHAR, typename _SZSTR>
inline unit<_INT, _CHAR, _SZSTR>::~unit()
{
	if (_dim)
		delete[] _dim;
}



template<typename _INT, typename _CHAR, typename _SZSTR>
inline std::string unit<_INT, _CHAR, _SZSTR>::serialize() const
{
	std::string bytes;
	if (_dim)
	{
		for (ndim i = 0; i < _nDims; ++i)
			bytes += _dim[i].serialize();
	}
	return bytes;
}



template<typename _INT, typename _CHAR, typename _SZSTR>
inline bool unit<_INT, _CHAR, _SZSTR>::deserialize(dimension** const vdimension, unsigned char vdimsz, const char* bytes, unsigned short size)
{
	if (_dim)
	{
		delete[] _dim;
		_dim = nullptr;
		_capacity = _nDims = 0;
	}

	constexpr long sz = sizeof(power_dimension::rational_base) + 1;
	if (size % sz != 0)
		return false;

	_capacity = _nDims = size / sz;
	if (!_nDims)
		return true;

	_dim = new power_dimension[_nDims];
	for (int i = 0; i < _nDims; ++i)
	{
		unsigned char index = _dim[i].deserialize(bytes + sz * i, sz);
		if (index < vdimsz)
			_dim[i].dim = vdimension[index];
		else
		{
			delete[] _dim;
			_dim = nullptr;
			return false;
		}
	}
	return true;
}



template<typename _INT, typename _CHAR, typename _SZSTR>
inline typename unit<_INT, _CHAR, _SZSTR>::ndim unit<_INT, _CHAR, _SZSTR>::nDims() const
{
	return _nDims;
}


template<typename _INT, typename _CHAR, typename _SZSTR>
inline bool unit<_INT, _CHAR, _SZSTR>::operator==(const unit& right) const
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


template<typename _INT, typename _CHAR, typename _SZSTR>
inline bool unit<_INT, _CHAR, _SZSTR>::operator!=(const unit& right) const
{
	return !operator==(right);
}



template<typename _INT, typename _CHAR, typename _SZSTR>
inline typename unit<_INT, _CHAR, _SZSTR>::ndim unit<_INT, _CHAR, _SZSTR>::calcNewDim(const unit& right) const
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



template<typename _INT, typename _CHAR, typename _SZSTR>
inline void unit<_INT, _CHAR, _SZSTR>::populateNewDim(power_dimension* result, ndim nNewDim, const unit& right, const _INT& mult) const
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


template<typename _INT, typename _CHAR, typename _SZSTR>
inline void unit<_INT, _CHAR, _SZSTR>::clean()
{
	compressVector(this, &_nDims);
}


template<typename _INT, typename _CHAR, typename _SZSTR>
inline bool unit<_INT, _CHAR, _SZSTR>::compressVector(power_dimension* dim, ndim* nDim)
{
	for (ndim i = 0; i < *nDim; ++i)
	{
		if ((!dim[i] || dim[i].dim == nullptr))
		{
			for (int j = i + 1; j < *nDim; ++j)
				dim[j - 1] = dim[j];
			if (i + 1 < *nDim)
				--i;
			--*nDim;
		}
	}
	return !*nDim;
}



template<typename _INT, typename _CHAR, typename _SZSTR>
inline void unit<_INT, _CHAR, _SZSTR>::calc(power_dimension** result, ndim* nNewDim, const unit& right, const _INT& mult) const
{
	*nNewDim = calcNewDim(right);
	*result = new power_dimension[*nNewDim];
	populateNewDim(*result, *nNewDim, right, mult);
	if (compressVector(*result, nNewDim))
	{
		delete[]* result;
		*result = nullptr;
	}
}


template<typename _INT, typename _CHAR, typename _SZSTR>
inline unit<_INT, _CHAR, _SZSTR>& unit<_INT, _CHAR, _SZSTR>::operator=(const unit& right)
{
	if (this != &right)
	{
		if (_nDims != right._nDims)
		{
			if (_capacity < right._nDims)
			{
				if (_dim)
					delete[]_dim;
				_dim = new power_dimension[_capacity = _nDims = right._nDims];
			}
			else
				_nDims = right._nDims;
		}
		for (ndim i = 0; i < _nDims; ++i)
			_dim[i] = right._dim[i];
	}
	return *this;
}


template<typename _INT, typename _CHAR, typename _SZSTR>
inline unit<_INT, _CHAR, _SZSTR>& unit<_INT, _CHAR, _SZSTR>::operator=(const dimension* right)
{
	if (_nDims != 1)
	{
		if (_capacity < 1)
		{
			if (_dim)
				delete[]_dim;
			_dim = new power_dimension[_capacity = _nDims = right._nDims];
		}
		else
			_nDims = right._nDims;
	}
	_dim[0].dim = right;
	_dim[0] = _INT(1);
	return *this;
}


template<typename _INT, typename _CHAR, typename _SZSTR>
inline void unit<_INT, _CHAR, _SZSTR>::release()
{
	if (_dim)
		_dim = nullptr;
	_capacity = _nDims = ndim(0);
}


template<typename _INT, typename _CHAR, typename _SZSTR>
inline void unit<_INT, _CHAR, _SZSTR>::clear()
{
	if (_dim)
	{
		delete[]_dim;
		_dim = nullptr;
	}
	_capacity = _nDims = ndim(0);
}


template<typename _INT, typename _CHAR, typename _SZSTR>
inline _INT unit<_INT, _CHAR, _SZSTR>::compare(const unit& right) const
{
	if (_nDims < right._nDims)
		return 0;
	bool found = false;
	_INT proportionality = 0;
	for (ndim r = 0; r < right._nDims; ++r)
	{
		_INT prop = 0;
		for (ndim l = 0; l < _nDims; ++l)
		{
			if (_dim[l].dim == right._dim[r].dim)
			{
				prop = _dim[l].proportionality(right._dim[r]);
				break;
			}
		}
		if (prop)
		{
			if (!found)
				proportionality = prop;
			else if (prop < proportionality)
				proportionality = prop;
			found = true;
		}
		else
			return 0;
	}
	return proportionality;
}


template<typename _INT, typename _CHAR, typename _SZSTR>
inline std::string unit<_INT, _CHAR, _SZSTR>::get_dimension() const
{
	std::string str;
	for (ndim d = 0; d < _nDims; ++d)
	{
		if (!_dim[d].zero())
		{
			std::stringstream ss;
			std::string sdim(_dim[d].dim->symbol(), _dim[d].dim->symbol_size());
			if (!_dim[d].unit())
			{
				sdim += "^";
				ss << _dim[d].numerator;
				if (!_dim[d].natural())
				{
					sdim += "(";
					if (!_dim[d].integer())
					{
						sdim += ss.str();
						ss.str("");
						sdim += "/";
						ss << _dim[d].denominator;
					}
					sdim += ss.str();
					sdim += ")";
				}
				else
					sdim += ss.str();
			}
			sdim = "[" + sdim + "]";
			str += sdim;
		}
	}
	return str;
}


template<typename _INT, typename _CHAR, typename _SZSTR>
inline unit<_INT, _CHAR, _SZSTR> unit<_INT, _CHAR, _SZSTR>::operator*(const unit& right) const
{
	ndim newDim;
	power_dimension* dim;
	calc(&dim, &newDim, right, _INT(1));
	return unit(dim, newDim);
}


template<typename _INT, typename _CHAR, typename _SZSTR>
inline unit<_INT, _CHAR, _SZSTR> unit<_INT, _CHAR, _SZSTR>::operator/(const unit& right) const
{
	ndim newDim;
	power_dimension* dim;
	calc(&dim, &newDim, right, _INT(-1));
	return unit(dim, newDim);
}


template<typename _INT, typename _CHAR, typename _SZSTR>
template<typename _N>
inline unit<_INT, _CHAR, _SZSTR> unit<_INT, _CHAR, _SZSTR>::pow(const _N& p) const
{
	power_dimension* dim = p && _dim ? new power_dimension[_nDims] : nullptr;
	if (dim)
	{
		for (ndim i = 0; i < _nDims; ++i)
		{
			dim[i].dim = _dim[i].dim;
			dim[i] = _dim[i] * p;
		}
	}
	return unit(dim, p ? _nDims : 0);
}



template<typename _INT, typename _CHAR, typename _SZSTR>
template<typename _N>
inline unit<_INT, _CHAR, _SZSTR> unit<_INT, _CHAR, _SZSTR>::pow(const _N& n, const _N& d) const
{
	if (!d)
		throw "Divide by zero";
	power_dimension* dim = n && _dim ? new power_dimension[_nDims] : nullptr;
	if (dim)
	{
		for (ndim i = 0; i < _nDims; ++i)
		{
			dim[i].dim = _dim[i].dim;
			dim[i] = _dim[i] * n / d;
		}
	}
	return unit(dim, n ? _nDims : 0);
}



template<typename _INT, typename _CHAR, typename _SZSTR>
template<typename _N>
inline unit<_INT, _CHAR, _SZSTR> unit<_INT, _CHAR, _SZSTR>::root(const _N& p) const
{
	if (!p)
		throw "Divide by zero";
	power_dimension* dim = _dim ? new power_dimension[_nDims] : nullptr;
	if (dim)
	{
		for (ndim i = 0; i < _nDims; ++i)
		{
			dim[i].dim = _dim[i].dim;
			dim[i] = _dim[i] / p;
		}
	}
	return unit(dim, dim ? _nDims : 0);
}

template<typename _INT, typename _CHAR, typename _SZSTR>
inline bool unit<_INT, _CHAR, _SZSTR>::dimensionless() const
{
	return !_nDims;
}

template<typename _INT, typename _CHAR, typename _SZSTR>
inline bool unit<_INT, _CHAR, _SZSTR>::one_dimension() const
{
	return _nDims == 1 && _dim[0].numerator == _dim[0].denominator;
}

template<typename _INT, typename _CHAR, typename _SZSTR>
inline const typename unit<_INT, _CHAR, _SZSTR>::dimension*
	unit<_INT, _CHAR, _SZSTR>::operator[](ndim nd) const
{
	return _dim[nd].dim;
}

template<typename _INT, typename _CHAR, typename _SZSTR>
inline const typename unit<_INT, _CHAR, _SZSTR>::dimension*
	unit<_INT, _CHAR, _SZSTR>::get_dimension(ndim nd) const
{
	return _dim[nd].dim;
}

template<typename _INT, typename _CHAR, typename _SZSTR>
inline const typename unit<_INT, _CHAR, _SZSTR>::power_dimension*
	unit<_INT, _CHAR, _SZSTR>::get_powdimension(ndim nd) const
{
	return _dim + nd;
}




















































template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline magnitude<_TYPE, _INT, _CHAR, _SZSTR>::magnitude()
{
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline magnitude<_TYPE, _INT, _CHAR, _SZSTR>::magnitude(const _TYPE& src)
	: _number(src)
{
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline magnitude<_TYPE, _INT, _CHAR, _SZSTR>::magnitude(const _MyT* src)
	: _number(src->_number), _unit(src->_unit)
{
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline magnitude<_TYPE, _INT, _CHAR, _SZSTR>::magnitude(const _CHAR* str, const _SZSTR& len)
	: _number(str, len)
{
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline magnitude<_TYPE, _INT, _CHAR, _SZSTR>::magnitude(const dimension* dim)
	: _number(_2TypeValue(1)), _unit(dim)
{
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline magnitude<_TYPE, _INT, _CHAR, _SZSTR>::magnitude(const _TYPE& n, const unit& u)
	: _number(n), _unit(u)
{
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool magnitude<_TYPE, _INT, _CHAR, _SZSTR>::is_numeric() const
{
	return true;
}




// Operations:
template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::positive(const _MyT& right)
{
	_number = +right._number;
	_unit = right._unit;
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::negative(const _MyT& right)
{
	_number = -right._number;
	_unit = right._unit;
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::factorial(const _MyT& right)
{
	if (!right._unit.dimensionless())
		throw "factorial operator must be dimensionless";
	_number = right._number.factorial();
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::multiplication(const _MyT& left, const _MyT& right)
{
	_number = left._number * right._number;
	_unit = left._unit * right._unit;
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::division(const _MyT& left, const _MyT& right)
{
	_number = left._number / right._number;
	_unit = left._unit / right._unit;
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, typename _SZSTR>::modulo(const _MyT& left, const _MyT& right)
{
	_number = left._number % right._number;
	_unit = left._unit;
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::addition(const _MyT& left, const _MyT& right)
{
	if (left._unit != right._unit)
		throw "incompatible units";
	_number = left._number + right._number;
	_unit = left._unit;
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::substraction(const _MyT& left, const _MyT& right)
{
	if (left._unit != right._unit)
		throw "incompatible units";
	_number = left._number - right._number;
	_unit = left._unit;
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::pow(const _MyT& left, const _MyT& right)
{
	if (!right._unit.dimensionless())
		throw "exponent must be dimensionless";
	_number = left._number.pow(right._number);
	_unit = left._unit.pow((_2TypeValue)right._number);
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::sqrroot(const _MyT& right)
{
	_number = right._number.sqrt();
	_unit = right._unit.pow(1, 2);
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::root(const _MyT& left, const _MyT& right)
{
	if (!left._unit.dimensionless())
		throw "exponent must be dimensionless";
	if (left.zero())
		throw "illegal root, divide by zero";
	_number = right._number.root(left._number);
	_unit = right._unit.root((_2TypeValue)left._number);
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::assignation(const _MyT& left, const _MyT& right)
{
	_number = right._number;
	_unit = right._unit;
}



// Functions:
template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::abs(const _MyT& arg)
{
	_number = arg._number.abs();
	_unit = arg._unit;
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::sgn(const _MyT& arg)
{
	_number = arg._number.sgn();
	_unit = arg._unit;
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::ln(const _MyT& arg)
{
	if (!arg._unit.dimensionless())
		throw "argument for function natural logarithm must be dimensionless";
	if (!arg.scalar())
		throw "must be scaler";
	_number = arg._number.ln();
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::lg(const _MyT& arg)
{
	if (!arg._unit.dimensionless())
		throw "argument for function logarithm must be dimensionless";
	if (!arg.scalar())
		throw "must be scaler";
	_number = arg._number.log();
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::log(const _MyT& arg, const _MyT& base)
{
	if (!arg._unit.dimensionless())
		throw "argument for function logarithm must be dimensionless";
	if (!arg.scalar())
		throw "must be scaler";
	if (!base._unit.dimensionless())
		throw "base for function logarithm must be dimensionless";
	if (!base.scalar())
		throw "must be scaler";
	_number = arg._number.log(base._number);
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::sin(const _MyT& arg)
{
	if (!arg._unit.dimensionless())
		throw "argument for function sine must be dimensionless";
	if (!arg.scalar())
		throw "must be scaler";
	_number = arg._number.sin();
}

template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::cos(const _MyT& arg)
{
	if (!arg._unit.dimensionless())
		throw "argument for function cosine must be dimensionless";
	if (!arg.scalar())
		throw "must be scaler";
	_number = arg._number.cos();
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::tg(const _MyT & arg)
{
	if (!arg._unit.dimensionless())
		throw "argument for function tangent must be dimensionless";
	if (!arg.scalar())
		throw "must be scaler";
	_number = arg._number.tg();
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::asin(const _MyT& arg)
{
	if (!arg._unit.dimensionless())
		throw "argument for function arc sine must be dimensionless";
	if (!arg.scalar())
		throw "must be scaler";
	_number = arg._number.asin();
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::acos(const _MyT& arg)
{
	if (!arg._unit.dimensionless())
		throw "argument for function arc cosine must be dimensionless";
	if (!arg.scalar())
		throw "must be scaler";
	_number = arg._number.acos();
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::atg(const _MyT & arg)
{
	if (!arg._unit.dimensionless())
		throw "argument for function arc tangent must be dimensionless";
	if (!arg.scalar())
		throw "must be scaler";
	_number = arg._number.atg();
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::atg2(const _MyT& opposite, const _MyT& adjacent)
{
	if (opposite._unit != adjacent._unit)
		throw "arguments for function arc tanget2 must be equal units";
	if (!opposite.scalar() || !adjacent.scalar())
		throw "must be scaler";
	_number = opposite._number.atg(adjacent._number);
}

template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline magnitude<_TYPE, _INT, _CHAR, _SZSTR>
	magnitude<_TYPE, _INT, _CHAR, _SZSTR>::pow(_TypeInt p) const
{
	return magnitude(_number.pow(p), _unit.pow(p));
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline magnitude<_TYPE, _INT, _CHAR, _SZSTR>
magnitude<_TYPE, _INT, _CHAR, _SZSTR>::operator*(const magnitude<_TYPE, _INT, _CHAR, _SZSTR>& right) const
{
	return magnitude(_number * right._number, _unit * right._unit);
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline magnitude<_TYPE, _INT, _CHAR, _SZSTR>
	magnitude<_TYPE, _INT, _CHAR, _SZSTR>::operator/(const magnitude<_TYPE, _INT, _CHAR, _SZSTR>& right) const
{
	if (!right._number)
		throw "Divide by zero";
	return magnitude(_number / right._number, _unit / right._unit);
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool magnitude<_TYPE, _INT, _CHAR, _SZSTR>::equal(const magnitude& right) const
{
	return _number == right._number && _unit == right._unit;
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool magnitude<_TYPE, _INT, _CHAR, _SZSTR>::less_equal(const magnitude& right) const
{
	if (_unit != right._unit)
		throw "Must be equal dimensions";
	return _number <= right._number;
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool magnitude<_TYPE, _INT, _CHAR, _SZSTR>::less(const magnitude& right) const
{
	if (_unit != right._unit)
		throw "Must be equal dimensions";
	return _number < right._number;
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool magnitude<_TYPE, _INT, _CHAR, _SZSTR>::greater_equal(const magnitude& right) const
{
	if (_unit != right._unit)
		throw "Must be equal dimensions";
	return _number >= right._number;
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool magnitude<_TYPE, _INT, _CHAR, _SZSTR>::greater(const magnitude& right) const
{
	if (_unit != right._unit)
		throw "Must be equal dimensions";
	return _number > right._number;
}



// Check:
template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool magnitude<_TYPE, _INT, _CHAR, _SZSTR>::zero() const
{
	return _number.zero();
}

template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool magnitude<_TYPE, _INT, _CHAR, _SZSTR>::positive() const
{
	return _number.positive();
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool magnitude<_TYPE, _INT, _CHAR, _SZSTR>::negative() const
{
	return _number.negative();
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool magnitude<_TYPE, _INT, _CHAR, _SZSTR>::integer() const
{
	return _number.integer();
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool magnitude<_TYPE, _INT, _CHAR, _SZSTR>::dimensionless() const
{
	return _unit.dimensionless();
}

template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool magnitude<_TYPE, _INT, _CHAR, _SZSTR>::one_dimension() const
{
	return _unit.one_dimension();
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool magnitude<_TYPE, _INT, _CHAR, _SZSTR>::scalar() const
{
	return _number.scalar();
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool magnitude<_TYPE, _INT, _CHAR, _SZSTR>::real() const
{
	return _number.real();
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool magnitude<_TYPE, _INT, _CHAR, _SZSTR>::imaginary() const
{
	return _number.imaginary();
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool magnitude<_TYPE, _INT, _CHAR, _SZSTR>::complex() const
{
	return _number.complex();
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool magnitude<_TYPE, _INT, _CHAR, _SZSTR>::vector() const
{
	return _number.vector();
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool magnitude<_TYPE, _INT, _CHAR, _SZSTR>::matrix() const
{
	return _number.matrix();
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::stringstream(std::stringstream& ssOut) const
{
	ssOut << _number;
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool magnitude<_TYPE, _INT, _CHAR, _SZSTR>::equal_unit(const magnitude& right) const
{
	return _unit == right._unit;
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline const _TYPE& magnitude<_TYPE, _INT, _CHAR, _SZSTR>::get_number() const
{
	return _number;
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline _TYPE& magnitude<_TYPE, _INT, _CHAR, _SZSTR>::get_number()
{
	return _number;
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline const typename magnitude<_TYPE, _INT, _CHAR, _SZSTR>::unit& magnitude<_TYPE, _INT, _CHAR, _SZSTR>::get_unit() const
{
	return _unit;
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline typename magnitude<_TYPE, _INT, _CHAR, _SZSTR>::unit& magnitude<_TYPE, _INT, _CHAR, _SZSTR>::get_unit()
{
	return _unit;
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::release_unit()
{
	_unit.release();
}



}
}
}

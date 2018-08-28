#include "pmb_calculate_magnitude.h"
#pragma once


namespace pmb
{
namespace calculate
{
namespace units
{




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
inline const _SIZE& dimension<_CHAR, _SIZE>::symbol_size() const
{
	return _symbol ? _slen: 0;
}


template<typename _CHAR, typename _SIZE>
inline const _SIZE& dimension<_CHAR, _SIZE>::name_size() const
{
	return _name ? _nlen : 0;
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
inline power_dimension<_INT, _CHAR, _SZSTR> power_dimension<_INT, _CHAR, _SZSTR>::operator*(const _INT& right) const
{
	rational<_INT> result = *static_cast<const rational<_INT>*>(this) * right;
	return power_dimension(result, dim);
}


template<typename _INT, typename _CHAR, typename _SZSTR>
inline power_dimension<_INT, _CHAR, _SZSTR> power_dimension<_INT, _CHAR, _SZSTR>::operator/(const _INT& right) const
{
	rational<_INT> result = *static_cast<rational<_INT>*>(this) / right;
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
inline unit<_INT, _CHAR, _SZSTR> unit<_INT, _CHAR, _SZSTR>::pow(const _INT& p) const
{
	power_dimension* dim = p ? new power_dimension[_nDims] : nullptr;
	for (ndim i = 0; p && i < _nDims; ++i)
	{
		dim[i].dim = _dim[i].dim;
		dim[i] = _dim[i] * p;
	}
	return unit(dim, p ? _nDims : 0);
}



template<typename _INT, typename _CHAR, typename _SZSTR>
inline unit<_INT, _CHAR, _SZSTR> unit<_INT, _CHAR, _SZSTR>::root(const _INT& p) const
{
	power_dimension* dim = p ? new power_dimension[_nDims] : nullptr;
	for (ndim i = 0; p && i < _nDims; ++i)
	{
		dim[i].dim = _dim[i].dim;
		dim[i] = _dim[i] / p;
	}
	return unit(dim, p ? _nDims : 0);
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

template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline magnitude<_TYPE, _INT, _CHAR, _SZSTR>::magnitude(const dimension * dim)
	: _number(1), _unit(dim)
{
}




// Operations:
template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::positive(const _MyT& right)
{
	_number = +right._number;
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::negative(const _MyT& right)
{
	_number = -right._number;
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::factorial(const _MyT& right)
{
	long res = 1;
	for (int i = 2; i <= right._number; ++i)
		res *= i;
	_number = res;
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
	_number = (long int)left._number % (long int)right._number;
	_unit = left._unit;
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::addition(const _MyT& left, const _MyT& right)
{
	if (left._unit != right._unit)
		throw "incompatible units";
	_number = left._number + right._number;
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::substraction(const _MyT& left, const _MyT& right)
{
	if (left._unit != right._unit)
		throw "incompatible units";
	_number = left._number - right._number;
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::exponentiation(const _MyT& left, const _MyT& right)
{
	_number = pow(left._number, right._number);
	_unit = left._unit.pow(right._number);
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::root(const _MyT& left, const _MyT& right)
{
	_number = pow(left._number, 1 / right._number);
	_unit = left.root(right._number);
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
	_number = arg._number < 0 ? -arg._number : arg._number;
	_unit = arg._unit;
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::sgn(const _MyT& arg)
{
	_number = arg._number < 0 ? -1 : 1;
	_unit = arg._unit;
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::sin(const _MyT& arg)
{
	_number = ::sin(arg._number);
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::tg(const _MyT & arg)
{
	_number = ::tan(arg._number);
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::atg(const _MyT & arg)
{
	_number = ::atan(arg._number);
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void magnitude<_TYPE, _INT, _CHAR, _SZSTR>::atg2(const _MyT& opposite, const _MyT& adjacent)
{
	_number = ::atan2(opposite._number, adjacent._number);
}



// Check:
template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool magnitude<_TYPE, _INT, _CHAR, _SZSTR>::zero() const
{
	return _number == _TYPE(0);
}

template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool magnitude<_TYPE, _INT, _CHAR, _SZSTR>::positive() const
{
	return 0 < _number;
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool magnitude<_TYPE, _INT, _CHAR, _SZSTR>::negative() const
{
	return _number < 0;
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool magnitude<_TYPE, _INT, _CHAR, _SZSTR>::integer() const
{
	return ceil(_number) == floor(_number);
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool magnitude<_TYPE, _INT, _CHAR, _SZSTR>::dimensionless() const
{
	return !_unit.nDims();
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool magnitude<_TYPE, _INT, _CHAR, _SZSTR>::scalar() const
{
	return true;
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool magnitude<_TYPE, _INT, _CHAR, _SZSTR>::real() const
{
	return true;
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool magnitude<_TYPE, _INT, _CHAR, _SZSTR>::imaginary() const
{
	return false;
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool magnitude<_TYPE, _INT, _CHAR, _SZSTR>::complex() const
{
	return false;
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool magnitude<_TYPE, _INT, _CHAR, _SZSTR>::vector() const
{
	return false;
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool magnitude<_TYPE, _INT, _CHAR, _SZSTR>::matrix() const
{
	return false;
}





}
}
}

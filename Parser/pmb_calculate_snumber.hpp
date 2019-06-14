#include "pmb_calculate_snumber.h"
#pragma once


namespace pmb
{
namespace calculate
{


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
snumber<_TYPE, _INT, _CHAR, _SZSTR>::snumber()
	: _type(tpNoSet), _data(nullptr)
{
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline snumber<_TYPE, _INT, _CHAR, _SZSTR>::snumber(const _TYPE& src, bool imaginary)
	: _type(imaginary ? tpImaginary : tpReal), magnitude(src), _data(nullptr)
{
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline snumber<_TYPE, _INT, _CHAR, _SZSTR>::snumber(_TYPE& real, _TYPE& imag)
	: _type(imag && real ? tpComplex : real ? tpReal : tpImaginary), magnitude(real ? real : imag)
{
	if (_type == tpComplex)
	{
		_data = new _TYPE;
		*_data = imag;
	}
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline snumber<_TYPE, _INT, _CHAR, _SZSTR>::snumber(_TYPE* vector, vidx size, bool imaginary)
	: _type(imaginary ? tpVectorImaginary : tpVectorReal)
{
	*reinterpret_cast<vidx*>(&_number) = size;
	_data = size ? vect : nullptr;
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline snumber<_TYPE, _INT, _CHAR, _SZSTR>::snumber(_TYPE* vector, vidx size)
	: _type(tpVectorComplex)
{
	*reinterpret_cast<vidx*>(&_number) = size;
	_data = size ? vect : nullptr;
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline snumber<_TYPE, _INT, _CHAR, _SZSTR>::snumber(_TYPE * array, aidx rows, aidx cols, bool imaginary)
	: _type(imaginary ? tpArrayImaginary : tpArrayReal)
{
	reinterpret_cast<const aidx*>(&_number)[0] = rows;
	reinterpret_cast<const aidx*>(&_number)[1] = cols;
	_data = rows * cols ? array : nullptr;
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline snumber<_TYPE, _INT, _CHAR, _SZSTR>::snumber(_TYPE* array, aidx rows, aidx cols)
	: _type(tpArrayComplex)
{
	reinterpret_cast<const aidx*>(&_number)[0] = rows;
	reinterpret_cast<const aidx*>(&_number)[1] = cols;
	_data = rows * cols ? array : nullptr;
}




template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline snumber<_TYPE, _INT, _CHAR, _SZSTR>::~snumber()
{
	if (_data)
	{
		if (tpComplex)
			delete _data;
		else
			delete[] _data;
	}
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void snumber<_TYPE, _INT, _CHAR, _SZSTR>::clear()
{
	if (_data)
	{
		if (tpComplex)
			delete _data;
		else
			delete[] _data;
	}
	_type = tpNoSet;
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool snumber<_TYPE, _INT, _CHAR, _SZSTR>::copy(const snumber& right, const _TYPE& factor)
{
	bool bRet = true;
	clear();
	_type = right._type;
	switch (_type)
	{
	case tpReal:
	case tpImaginary:
		_number = factor * right._number;
		break;
	case tpComplex:
		_number = factor * right._number;
		if (!_number)
		{
			if (right._data)
			{
				_type = tpImaginary;
				_number = factor * *right._data;
			}
			else
				_type = tpReal;
		}
		else if (right._data)
		{
			_data = new _TYPE;
			*data = factor * *right._data;
		}
		else
			_type = tpReal;
		break;
	case tpVectorReal:
	case tpVectorImaginary:
		{
			_number = right._number;
			vidx size = *reinterpret_cast<const vidx*>(&_number);
			if (size)
			{
				_data = new _TYPE[size];
				for (vidx i = 0; i < size; ++i)
					_data[i] = factor * right._data[i];
			}
		}
		break;
	case tpVectorComplex:
		{
			_number = right._number;
			vidx size = *reinterpret_cast<const vidx*>(&_number);
			if (size)
			{
				_data = new _TYPE[size];
				for (vidx i = 0; i < 2 * size; ++i)
					_data[i] = factor * right._data[i];
			}
		}
		break;
	case tpArrayReal:
	case tpArrayImaginary:
		{
			_number = right._number;
			reinterpret_cast<const aidx*>(&_number)[0] = rows;
			reinterpret_cast<const aidx*>(&_number)[1] = cols;
			if (cols && rows)
			{
				_data = new _TYPE[cols * rows];
				for (vidx i = 0; i < cols * rows; ++i)
					_data[i] = factor * right._data[i];
			}
		}
		break;
	case tpArrayComplex:
		{
			_number = right._number;
			reinterpret_cast<const aidx*>(&_number)[0] = rows;
			reinterpret_cast<const aidx*>(&_number)[1] = cols;
			if (rows && cols)
			{
				_data = new _TYPE[2 * rows * rows];
				for (vidx i = 0; i < 2 * rows * cols; ++i)
					_data[i] = factor * right._data[i];
			}
		}
		break;
	default:
		bRet = false;
		break;
	}
	return bRet;
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void snumber<_TYPE, _INT, _CHAR, _SZSTR>::positive(const snumber& right)
{
	copy(right, 1);
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void snumber<_TYPE, _INT, _CHAR, _SZSTR>::negative(const snumber& right)
{
	copy(right, -1);
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void snumber<_TYPE, _INT, _CHAR, _SZSTR>::multiplication(const snumber& left, const snumber& right)
{
	clear();
	switch (left._type)
	{
	case tpBoolean:
		_product_lboolean(left, right);
		break;
	case tpVectorBoolean:
		_product_lvectorboolean(left, right);
		break;
	case tpArrayBoolean:
		_product_larrayboolean(left, right);
		break;
	case tpReal:
		_product_lreal(left, right);
		break;
	case tpImaginary:
		_product_limaginary(left, right);
		break;
	case tpComplex:
		_product_lcomplex(left, right);
		break;
	case tpVectorReal:
		_product_lvectorreal(left, right);
		break;
	case tpVectorImaginary:
		_product_lvectorimaginary(left, right);
		break;
	case tpVectorComplex:
		_product_lvectorcomplex(left, right);
		break;
	case tpArrayReal:
		_product_larrayreal(left, right);
		break;
	case tpArrayImaginary:
		_product_larrayimaginary(left, right);
		break;
	case tpArrayComplex:
		_product_larraycomplex(left, right);
		break;
	default:
		break;
	}
	_unit = left._unit * right._unit;
}

template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void snumber<_TYPE, _INT, _CHAR, _SZSTR>::addition(const snumber& left, const snumber& right)
{
	if (left._type != right._type 
		&& !((left._type == tpReal || left._type == tpImaginary || left._type == tpComplex) && (right._type == tpReal || right._type == tpImaginary || right._type == tpComplex)
			|| (left._type == tpVectorReal || left._type == tpVectorImaginary || left._type == tpVectorComplex) && (right._type == tpVectorReal || right._type == tpVectorImaginary || right._type == tpVectorComplex)
			|| (left._type == tpArrayReal || left._type == tpArrayImaginary || left._type == tpArrayComplex) && (right._type == tpArrayReal || right._type == tpArrayImaginary || right._type == tpArrayComplex))
	{
		throw "Invalid addition over incompatible types";
	}
	if ((left._type == tpVectorBoolean || left._type == tpVectorReal || left._type == tpVectorImaginary || left._type == tpVectorComplex)
		&& *reinterpret_cast<const vidx*>(&left._number) != *reinterpret_cast<const vidx*>(&right._number))
	{
		throw "Incompatibles dimension";
	}
	if (left._type == tpArrayBoolean || left._type == tpArryReal || left._type == tpArrayImaginary || left._type == tpArrayComplex)
	{
		if (reinterpret_cast<const aidx*>(&left._number)[0] != reinterpret_cast<const aidx*>(&right._number)[0])
			throw "Incompatible rows";
		if ((reinterpret_cast<const aidx*>(&left._number)[1] != reinterpret_cast<const aidx*>(&right._number)[1]))
			throw "Incompatible columns";
	}

	clear();
	_unit = left._unit + right._unit;

	switch (left._type)
	{
	case tpBoolean:
		*reinterpret_cast<bool*>(&_number) = *reinterpret_cast<const bool*>(&left._number) || *reinterpret_cast<const bool*>(&right._number);
		break;
	case tpVectorBoolean:
		*reinterpret_cast<vidx*>(&_number) = *reinterpret_cast<const vidx*>(&left._number);
		if (left._data && right._data)
		{
			vidx size = *reinterpret_cast<const vidx*>(&_number);
			_data = reinterpret_cast<_TYPE*>(new bool[size]);
			for (vidx i = 0; i < size; ++i)
				reinterpret_cast<bool*>(_data)[i] = reinterpret_cast<const bool*>(left._data)[i] || reinterpret_cast<const bool*>(right._data)[i];
		}
		break;
	case tpArrayBoolean:
		reinterpret_cast<aidx*>(&_number)[0] = reinterpret_cast<const vidx*>(&left._number)[0];
		reinterpret_cast<aidx*>(&_number)[1] = reinterpret_cast<const vidx*>(&left._number)[1];
		if (left._data && right._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&_number)[0];
			aidx cols = reinterpret_cast<const aidx*>(&_number)[1];
			_data = reinterpret_cast<_TYPE*>(new bool[rows * cols]);
			for (aidx i = 0; i < rows; ++i)
				for (aidx j = 0; j < cols; ++j)
				reinterpret_cast<bool*>(_data)[i + j * rows] = reinterpret_cast<const bool*>(left._data)[i + j * rows] || reinterpret_cast<const bool*>(right._data)[i + j * rows];
		}
		break;
	case tpReal:
		_product_lreal(left, right);
		break;
	case tpImaginary:
		_product_limaginary(left, right);
		break;
	case tpComplex:
		_product_lcomplex(left, right);
		break;
	case tpVectorReal:
		_product_lvectorreal(left, right);
		break;
	case tpVectorImaginary:
		_product_lvectorimaginary(left, right);
		break;
	case tpVectorComplex:
		_product_lvectorcomplex(left, right);
		break;
	case tpArrayReal:
		_product_larrayreal(left, right);
		break;
	case tpArrayImaginary:
		_product_larrayimaginary(left, right);
		break;
	case tpArrayComplex:
		_product_larraycomplex(left, right);
		break;
	default:
		break;
	}
}

template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void snumber<_TYPE, _INT, _CHAR, _SZSTR>::substraction(const snumber & left, const snumber & right)
{
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool snumber<_TYPE, _INT, _CHAR, _SZSTR>::zero() const
{
	bool bRet;
	switch (_type)
	{
	case tpBoolean:
	case tpVectorBoolean:
	case tpArrayBoolean:
		bRet = false;
		break;
	case tpReal:
	case tpImaginary:
		bRet = magnitude::zero();
		break;
	case tpComplex:
		bRet = !_number && !*_data;
		break;
	case tpVectorReal:
	case tpVectorImaginary:
		bRet = true;
		for (vidx i = 0; i < *reinterpret_cast<const vidx*>(&_number); ++i)
		{
			if (_data[i])
			{
				bRet = false;
				break;
			}
		}
		break;
	case tpVectorComplex:
		bRet = true;
		for (vidx i = 0; i < *reinterpret_cast<const vidx*>(&_number); ++i)
		{
			if (_data[2 * i] || _data[2 * i + 1])
			{
				bRet = false;
				break;
			}
		}
		break;
	case tpArrayReal:
	case tpArrayImaginary:
		{
			bRet = true;
			aidx cols = reinterpret_cast<const aidx*>(&_number)[0];
			aidx rows = reinterpret_cast<const aidx*>(&_number)[1];
			for (aidx i = 0; i < cols * rows; ++i)
			{
				if (_data[i])
				{
					bRet = false;
					break;
				}
			}
		}
		break;
	case tpArrayImaginary:
		{
			bRet = true;
			aidx cols = reinterpret_cast<const aidx*>(&_number)[0];
			aidx rows = reinterpret_cast<const aidx*>(&_number)[1];
			for (aidx i = 0; i < cols * rows; ++i)
			{
				if (_data[2 * i] || _data[2 * i + 1])
				{
					bRet = false;
					break;
				}
			}
		}
		break;
	default:
		bRet = false;
	}
	return bRet;
}





template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool snumber<_TYPE, _INT, _CHAR, _SZSTR>::integer() const
{
	return tpReal && ceil(_number) == floor(_number);
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool snumber<_TYPE, _INT, _CHAR, _SZSTR>::dimensionless() const
{
	return magnitude::dimensionless();
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool snumber<_TYPE, _INT, _CHAR, _SZSTR>::one_dimension() const
{
	return magnitude::one_dimension();
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool snumber<_TYPE, _INT, _CHAR, _SZSTR>::scalar() const
{
	return tpBoolean || tpReal || tpImaginary || tpComplex;
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool snumber<_TYPE, _INT, _CHAR, _SZSTR>::real() const
{
	return tpReal || tpVectorReal || tpArrayReal;
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool snumber<_TYPE, _INT, _CHAR, _SZSTR>::imaginary() const
{
	return tpImaginary || tpVectorImaginary || tpArrayImaginary;
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool snumber<_TYPE, _INT, _CHAR, _SZSTR>::complex() const
{
	return tpComplex || tpVectorComplex || tpArrayComplex;
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool snumber<_TYPE, _INT, _CHAR, _SZSTR>::vector() const
{
	return tpVectorBoolean || tpVectorReal || tpVectorImaginary || tpVectorComplex;
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline bool snumber<_TYPE, _INT, _CHAR, _SZSTR>::matrix() const
{
	return tpMatrixBoolean || tpMatrixReal || tpMatrixImaginary || tpMatrixComplex;
}

template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline snumber<_TYPE, _INT, _CHAR, _SZSTR>::vidx
	snumber<_TYPE, _INT, _CHAR, _SZSTR>::dimension() const
{
	return _type == tpVectorBoolean || _type == tpVectorReal || _type == tpVectorImaginary || _type == tpVectorComplex ?
			*reinterpret_cast<const vidx*>(&_number) : 0;
}

template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline snumber<_TYPE, _INT, _CHAR, _SZSTR>::aidx
	snumber<_TYPE, _INT, _CHAR, _SZSTR>::rows() const
{
	return _type == _tpArrayBoolean || _type == tpArrayReal || _type == tpArrayImaginary || _type == tpArrayComplex ?
			_reinterpret_cast<const aidx*>(&_number)[0] : 0;
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline snumber<_TYPE, _INT, _CHAR, _SZSTR>::aidx
	snumber<_TYPE, _INT, _CHAR, _SZSTR>::cols() const
{
	return _type == _tpArrayBoolean || _type == tpArrayReal || _type == tpArrayImaginary || _type == tpArrayComplex ?
		_reinterpret_cast<const aidx*>(&_number)[1] : 0;
}






template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void snumber<_TYPE, _INT, _CHAR, _SZSTR>::_product_lboolean(const snumber& left, const snumber& right)
{
	switch (right._type)
	{
	case tpBoolean:
		_type = tpBoolean;
		*reinterpret_cast<bool*>(&_number) = *reinterpret_cast<const bool*>(&left._number) && *reinterpret_cast<const bool*>(&right._number);
		_data = nullptr;
		break;
	case tpVectorBoolean:
		_type = tpVectorBoolean;
		_number = right._number;
		if (right._data)
		{
			vidx size = *reinterpret_cast<vidx*>(&_number);
			reinterpret_cast<bool*>(_data) = new bool[size];
			for (vidx i = 0; i < size; ++i)
				reinterpret_cast<bool*>(_data)[i] = *reinterpret_cast<const bool*>(&left._number) && reinterpret_cast<const bool*>(right._data)[i];
		}
		else
			_data = nullptr;
		break;
	case tpArrayBoolean:
		_type = tpArrayBoolean;
		_number = right._number;
		if (right._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&_number)[0];
			aidx cols = reinterpret_cast<const aidx*>(&_number)[1];
			reinterpret_cast<bool*>(_data) = new bool[rows * cols];
			for (vidx i = 0; i < rows * cols; ++i)
				reinterpret_cast<bool*>(_data)[i] = *reinterpret_cast<const bool*>(&left._number) && reinterpret_cast<const bool*>(right._data)[i];
		}
		else
			_data = nullptr;
		break;
	default:
		throw "Incompatible types";
		break;
	}
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void snumber<_TYPE, _INT, _CHAR, _SZSTR>::_product_lvectorboolean(const snumber& left, const snumber& right)
{
	switch (right._type)
	{
	case tpBoolean:
		_type = tpVectorBoolean;
		_number = left._number;
		if (left._data)
		{
			vidx size = *reinterpret_cast<const vidx*>(&_number);
			reinterpret_cast<bool*>(_data) = new bool[size];
			for (vidx i = 0; i < size; ++i)
				reinterpret_cast<bool*>(_data)[i] = *reinterpret_cast<const bool*>(left._data)[i] && *reinterpret_cast<const bool*>(&right._number);
		}
		else
			_data = nullptr;
		break;
	case tpVectorBoolean:
		if (*reinterpret_cast<const vidx*>(&left._number) != *reinterpret_cast<const vidx*>(&right._number))
			throw "The vectors must have equal dimension";
		_type = tpBoolean;
		_data = nullptr;
		if (left._data && right._data)
		{
			vidx size = *reinterpret_cast<const vidx*>(&_number);
			if (0 < size)
				*reinterpret_cast<bool*>(_number) = reinterpret_cast<const bool*>(left._data)[0] && reinterpret_cast<const bool*>(right._data)[0];
			for (vidx i = 1; i < size; ++i)
				*reinterpret_cast<bool*>(_number) = *reinterpret_cast<bool*>(_number)
						|| reinterpret_cast<const bool*>(left._data)[i] && reinterpret_cast<const bool*>(right._data)[i];
		}
		else
			_data = nullptr;
		break;
	case tpArrayBoolean:
		if (reinterpret_cast<const aidx*>(&right._number)[0] != 1)
			throw "The right matrix must be a column vector";
		reinterpret_cast<aidx*>(&_number)[0] = *reinterpret_cast<const vidx*>(&left._number);
		reinterpret_cast<aidx*>(&_number)[1] = reinterpret_cast<const aidx*>(&right._number)[1];
		if (right._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&_number)[0];
			aidx cols = reinterpret_cast<const aidx*>(&_number)[1];
			reinterpret_cast<bool*>(_data) = new bool[rows * cols];
			for (aidx i = 0; i < rows; ++i)
			{
				for (aidx j = 0; j < cols; ++j)
					reinterpret_cast<bool*>(_data)[i + j * rows] = reinterpret_cast<const bool*>(left._data)[i] && reinterpret_cast<const bool*>(right._data)[0 + j * 1];
			}
		}
		else
			_data = nullptr;
		break;
	default:
		throw "Incompatible types";
		break;
	}
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void snumber<_TYPE, _INT, _CHAR, _SZSTR>::_product_larrayboolean(const snumber& left, const snumber& right)
{
	switch (right._type)
	{
	case tpBoolean:
		_type = tpArrayBoolean;
		_number = left._number;
		if (left._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&_number)[0];
			aidx cols = reinterpret_cast<const aidx*>(&_number)[1];
			reinterpret_cast<bool*>(_data) = new bool[rows * cols];
			for (vidx i = 0; i < rows * cols; ++i)
				reinterpret_cast<bool*>(_data)[i] = *reinterpret_cast<const bool*>(left._data)[i] && *reinterpret_cast<const bool*>(&right._number);
		}
		else
			_data = nullptr;
		break;
	case tpVectorBoolean:
		if (reinterpret_cast<const aidx*>(&left._number)[1] != *reinterpret_cast<const vidx*>(&right._number))
			throw "The vector dimension must be equal to array columns";
		_type = tpVectorBoolean;
		*reinterpret_cast<vidx*>(&_number) = reinterpret_cast<const aidx*>(&left._number)[0];
		_data = nullptr;
		if (left._data && right._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&left._number)[0];
			aidx cols = reinterpret_cast<const aidx*>(&left._number)[1];
			reinterpret_cast<bool*>(_data) = new bool[rows];

			*reinterpret_cast<bool*>(_number) = reinterpret_cast<const bool*>(left._data)[0] && reinterpret_cast<const bool*>(right._data)[0];
			for (aidx i = 0; i < rows; ++i)
			{
				for (aidx j = 0; j < 1; ++j)
				{
					if (0 < cols)
						reinterpret_cast<bool*>(_number)[i] = reinterpret_cast<const bool*>(left._data)[i + 0 * rows] && reinterpret_cast<const bool*>(right._data)[k];
					for (aidx k = 0; k < cols; ++k)
						reinterpret_cast<bool*>(_number)[i] = reinterpret_cast<bool*>(_number)[i]
							|| reinterpret_cast<const bool*>(left._data)[i + k * rows] && reinterpret_cast<const bool*>(right._data)[k];
				}
			}
		}
		else
			_data = nullptr;
		break;
	case tpArrayBoolean:
		if (reinterpret_cast<const aidx*>(&left._number)[1] != reinterpret_cast<const aidx*>(&right._number)[0])
			throw "The invalid operation [m*n][n*p] = [m*p]";
		reinterpret_cast<aidx*>(&_number)[0] = reinterpret_cast<const vidx*>(&left._number)[0];
		reinterpret_cast<aidx*>(&_number)[1] = reinterpret_cast<const aidx*>(&right._number)[1];
		if (left._data && right._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&_number)[0];
			aidx cols = reinterpret_cast<const aidx*>(&_number)[1];
			aidx m = reinterpret_cast<const vidx*>(&left._number)[0];
			reinterpret_cast<bool*>(_data) = new bool[rows * cols];
			for (aidx i = 0; i < rows; ++i)
			{
				for (aidx j = 0; j < cols; ++j)
				{
					if (0 < m)
						reinterpret_cast<bool*>(_data)[i + j * rows] = reinterpret_cast<const bool*>(left._data)[i + 0 * rows] 
																	&& reinterpret_cast<const bool*>(right._data)[0 + j * m];
					for (aidx k = 1; k < m; ++k)
						reinterpret_cast<bool*>(_data)[i + j * rows] = reinterpret_cast<bool*>(_data)[i + j * rows]
																	|| reinterpret_cast<const bool*>(left._data)[i + k * rows]
																	&& reinterpret_cast<const bool*>(right._data)[k + j * m];
				}
			}
		}
		else
			_data = nullptr;
		break;
	default:
		throw "Incompatible types";
		break;
	}
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void snumber<_TYPE, _INT, _CHAR, _SZSTR>::_product_lreal(const snumber& left, const snumber& right)
{
	switch (right._type)
	{
	case tpBoolean:
	case tpVectorBoolean:
	case tpArrayBoolean:
		throw "Incompatible types";
		break;
	case tpReal:
		_type = tpReal;
		_number = left._number * right._number;
		break;
	case tpImaginary:
		_type = left._number && right._number ? tpImaginary : tpReal;
		_number = left._number * right._number;
		break;
	case tpComplex:
		_type = left._number ? tpComplex : right._data ? tpImaginary : tpReal;
		_number = left._number * (_type == tpComplex || _type == tpReal ? right._number : right._data ? *right._data : 0);
		if (right._data && _type != tpImaginary)
		{
			_data = new _TYPE;
			*_data = left._number * *right._data;
		}
		else
			_data = nullptr;
		break;
	case tpVectorReal:
	case tpVectorImaginary:
		_type = right._type;
		_number = right._number;
		if (right._data)
		{
			_data = new _TYPE[*reinterpret_cast<const vidx*>(&_number)];
			for (vidx i = 0; i < *reinterpret_cast<const vidx*>(&_number); ++i)
				_data[i] = left._number * right._data[i];
		}
		else
			_data = nullptr;
		break;
	case tpVectorComplex:
		_type = right._type;
		_number = right._number;
		if (right._data)
		{
			_data = new _TYPE[2 * *reinterpret_cast<const vidx*>(&_number)];
			for (vidx i = 0; i < 2 * *reinterpret_cast<const vidx*>(&_number); ++i)
				_data[i] = left._number * right._data[i];
		}
		else
			_data = nullptr;
		break;
	case tpArrayReal:
	case tpArrayImaginary:
		_type = right._type;
		_number = right._number;
		if (right._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&_number)[0];
			aidx cols = reinterpret_cast<const aidx*>(&_number)[1];
			_data = new _TYPE[rows * cols];
			for (vidx i = 0; i < rows * cols; ++i)
				_data[i] = left._number * right._data[i];
		}
		else
			_data = nullptr;
		break;
	case tpArrayComplex:
		_type = right._type;
		_number = right._number;
		if (right._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&_number)[0];
			aidx cols = reinterpret_cast<const aidx*>(&_number)[1];
			_data = new _TYPE[2 * rows * cols];
			for (vidx i = 0; i < 2 * rows * cols; ++i)
				_data[i] = left._number * right._data[i];
		}
		else
			_data = nullptr;
	default:
		throw "Incompatible types";
		break;
	}
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void snumber<_TYPE, _INT, _CHAR, _SZSTR>::_product_limaginary(const snumber& left, const snumber& right)
{
	switch (right._type)
	{
	case tpReal:
		_type = tpImaginary;
		_number = left._number * right._number;
		break;
	case tpImaginary:
		_type = tpReal;
		_number = -(left._number * right._number);
		break;
	case tpComplex:
		_type = left._number && right._data ? tpComplex : left._number ? tpImaginary : tpReal;
		_number = left._number * (_type == tpComplex || _type == tpImaginary ? right._number : right._data ? -*right._data);
		if (_type == tpComplex)
		{
			_data = new _TYPE;
			*_data = left._number * right._number;
		}
		else
			_data = nullptr;
		break;
	case tpVectorReal:
	case tpVectorImaginary:
		_type = right._type == tpVectorReal ? tpVectorImaginary : tpVectorReal;
		_number = right._number;
		if (right._data)
		{
			_data = new _TYPE[*reinterpret_cast<const vidx*>(&_number)];
			if (_type == tpVectorImaginary)
				for (vidx i = 0; i < *reinterpret_cast<const vidx*>(&_number); ++i)
					_data[i] = left._number * right._data[i];
			else
				for (vidx i = 0; i < *reinterpret_cast<const vidx*>(&_number); ++i)
					_data[i] = -(left._number * right._data[i]);
		}
		else
			_data = nullptr;
		break;
	case tpVectorComplex:
		_type = right._type;
		_number = right._number;
		if (right._data)
		{
			_data = new _TYPE[2 * *reinterpret_cast<const vidx*>(&_number)];
			for (vidx i = 0; i < *reinterpret_cast<const vidx*>(&_number); ++i)
			{
				_data[2 * i] = -(left._number * right._data[2 * i + 1]);
				_data[2 * i + 1] = left._number * right._data[2 * i];
			}
		}
		else
			_data = nullptr;
		break;
	case tpArrayReal:
	case tpArrayImaginary:
		_type = right._type == tpArrayReal ? tpArrayImaginary : tpArrayReal;
		_number = right._number;
		if (right._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&_number)[0];
			aidx cols = reinterpret_cast<const aidx*>(&_number)[1];
			_data = new _TYPE[rows * cols];
			if (_type == tpArrayImaginary)
				for (vidx i = 0; i < rows * cols; ++i)
					_data[i] = left._number * right._data[i];
			else
				for (vidx i = 0; i < rows * cols; ++i)
					_data[i] = -(left._number * right._data[i]);
		}
		else
			_data = nullptr;
		break;
	case tpArrayComplex:
		_type = right._type;
		_number = right._number;
		if (right._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&_number)[0];
			aidx cols = reinterpret_cast<const aidx*>(&_number)[1];
			_data = new _TYPE[2 * rows * cols];
			for (vidx i = 0; i < rows * cols; ++i)
			{
				_data[2 * i] = -(left._number * right._data[2 * i + 1]);
				_data[2 * i + 1] = left._number * right._data[2 * i];
			}
		}
		else
			_data = nullptr;
	default:
		throw "Incompatible types";
		break;
	}
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void snumber<_TYPE, _INT, _CHAR, _SZSTR>::_product_lcomplex(const snumber& left, const snumber& right)
{
	switch (right._type)
	{
	case tpReal:
		_type = left._number && left._data ? tpComplex : left._data ? tpImaginary : tpReal;
		_number = (_type == tpImaginary ? *_left.data : left._number) * right._number;
		if (left._data)
		{
			_data = new _TYPE;
			*_data = *left._data * right._number;
		}
		else
			_data = nullptr;
		break;
	case tpImaginary:
		if (!left._data && right._number)
		{
			_type = tpImaginary;
			_number = left._number * right._number;
			_data = nullptr;
		}
		else if (left._data)
		{
			_type == right._number ? tpComplex : tpReal;
			_number = -(*left._data * right._number);
			if (_type == tpComplex)
			{
				_data = new _TYPE;
				*_data = left._number * right._number;
			}
			else
				_data = nullptr;
		}
		else
		{
			_number = 0;
			_data = nullptr;
		}
		break;
	case tpComplex:
		_type = tpComplex;
		_number = left._number * right._number;
		if (right._data || left._data)
		{
			_data = new _TYPE;
			*_data = right._data ? left._number * *right._data : 0;
			if (left._data)
			{
				*_data += *left._data * right._number;
				if (right._data)
					_number -= *left._data * *right._data;
			}
		}
		else
			_data = nullptr;
		break;
	case tpVectorReal:
	case tpVectorImaginary:
		_type = tpVectorComplex;
		_number = right._number;
		if (right._data)
		{
			_data = new _TYPE[2 * *reinterpret_cast<const vidx*>(&_number)];
			if (right._type == tpVectorImaginary)
				for (vidx i = 0; i < *reinterpret_cast<const vidx*>(&_number); ++i)
				{
					_data[2 * i] = left._data ? -(*left._data * right._data[i]) : 0;
					_data[2 * i + 1] = left._number * right._data[i];
				}
			else
				for (vidx i = 0; i < *reinterpret_cast<const vidx*>(&_number); ++i)
				{
					_data[2 * i] = left._number * right._data[i];
					_data[2 * i + 1] = left._data ? *left._data * right._data[i] : 0;
				}
		}
		else
			_data = nullptr;
		break;
	case tpVectorComplex:
		_type = right._type;
		_number = right._number;
		if (right._data)
		{
			_data = new _TYPE[2 * *reinterpret_cast<const vidx*>(&_number)];
			for (vidx i = 0; i < *reinterpret_cast<const vidx*>(&_number); ++i)
			{
				_data[2 * i] = left._number * right._data[2 * i] - (left._data ? *left._data * right._data[2 * i + 1] : 0);
				_data[2 * i + 1] = left._number * right._data[2 * i + 1] + (left._data ? *left._data * right._data[2 * i] : 0);
			}
		}
		else
			_data = nullptr;
		break;
	case tpArrayReal:
	case tpArrayImaginary:
		_type = tpArrayComplex;
		_number = right._number;
		if (right._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&_number)[0];
			aidx cols = reinterpret_cast<const aidx*>(&_number)[1];
			_data = new _TYPE[2 * rows * cols];
			if (right._type == tpArrayImaginary)
				for (vidx i = 0; i < rows * cols; ++i)
				{
					_data[2 * i] = left._data ? -(*left._data * right._data[i]) : 0;
					_data[2 * i + 1] = left._number * right._data[i];
				}
			else
				for (vidx i = 0; i < rows * cols; ++i)
				{
					_data[2 * i] = left._number * right._data[i];
					_data[2 * i + 1] = left._data ? *left._data * right._data[i] : 0;
				}
		}
		else
			_data = nullptr;
		break;
	case tpArrayComplex:
		_type = right._type;
		_number = right._number;
		if (right._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&_number)[0];
			aidx cols = reinterpret_cast<const aidx*>(&_number)[1];
			_data = new _TYPE[2 * rows * cols];
			for (vidx i = 0; i < rows * cols; ++i)
			{
				_data[2 * i] = left._number * right._data[2 * i] - (left._data ? *left._data * right._data[2 * i + 1] : 0);
				_data[2 * i + 1] = left._number * right._data[2 * i + 1] + (left._data ? *left._data * right._data[2 * i]);
			}
		}
		else
			_data = nullptr;
	default:
		throw "Incompatible types";
		break;
	}
}




template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void snumber<_TYPE, _INT, _CHAR, _SZSTR>::_product_lvectorreal(const snumber& left, const snumber& right)
{
	switch (right._type)
	{
	case tpReal:
	case tpImaginary:
		_type = right._type == tpReal ? tpVectorReal : tpVectorImaginary;
		_number = left._number;
		if (left._data)
		{
			_data = new _TYPE[*reinterpret_cast<const vidx*>(&_number)];
			for (vidx i = 0; i < *reinterpret_cast<const vidx*>(&_number); ++i)
				_data[i] = left._data[i] * right._number;
		}
		else
			_data = nullptr;
		break;
	case tpComplex:
		_type = right._data ? right._number ? tpVectorComplex : tpVectorImaginary : tpVectorReal;
		_number = left._number;
		if (left._data)
		{
			_data = new _TYPE[*reinterpret_cast<const vidx*>(&_number) * (_type == tpVectorComplex ? 2 : 1)];
			if (right._data)
			{
				if (_type == tpVectorImaginary)
					for (vidx i = 0; i < *reinterpret_cast<const vidx*>(&_number); ++i)
						_data[i] = left._data[i] * *right._data;
				else
					for (vidx i = 0; i < *reinterpret_cast<const vidx*>(&_number); ++i)
					{
						_data[2 * i] = left._data[i] * right._number;
						_data[2 * i + 1] = left._data[i] * *right._data;
					}
			}
			else
				for (vidx i = 0; i < *reinterpret_cast<const vidx*>(&_number); ++i)
					_data[i] = left._data[i] * right._number;
		}
		else
			_data = nullptr;
		break;
	case tpVectorReal:
	case tpVectorImaginary:
		if (*reinterpret_cast<const vidx*>(&left._number) != *reinterpret_cast<const vidx*>(&right._number))
			throw "The vectors must have equal dimension";
		_type = right._type == tpVectorReal ? tpReal : tpImaginary;
		_number = 0;
		if (left._data && right._data)
		{
			if (right._type == tpVectorReal)
				for (vidx i = 0; i < *reinterpret_cast<const vidx*>(&left._number); ++i)
					_number += left._data[i] * right._data[i];
			else
				for (vidx i = 0; i < *reinterpret_cast<const vidx*>(&left._number); ++i)
					_number -= left._data[i] * right._data[i];
		}
		_data = nullptr;
		break;
	case tpVectorComplex:
		if (*reinterpret_cast<const vidx*>(&left._number) != *reinterpret_cast<const vidx*>(&right._number))
			throw "The vectors must have equal dimension";
		_type = tpComplex;
		_number = 0;
		if (left._data && right._data)
		{
			_data = new _TYPE;
			*_data = 0;
			for (vidx i = 0; i < *reinterpret_cast<const vidx*>(&left._number); ++i)
			{
				_number += left._data[i] * right._data[2 * i];
				*_data -= left._data[i] * right._data[2 * i + 1];
			}
		}
		else
			_data = nullptr;
		break;
	case tpArrayReal:
	case tpArrayImaginary:
		if (reinterpret_cast<const aidx*>(&right._number)[0] == 1)
			throw "Invalid operation over matrix (must be a vector column)";
		_type = right._type;
		reinterpret_cast<aidx*>(&_number)[0] = *reinterpret_cast<const vidx*>(&left._number);
		reinterpret_cast<aidx*>(&_number)[1] = reinterpret_cast<const aidx*>(&right._number)[1];
		if (left._data && right._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&_number)[0];
			aidx cols = reinterpret_cast<const aidx*>(&_number)[1];
			_data = new _TYPE[rows * cols];
			for (aidx i = 0; i < rows; ++i)
			{
				for (aidx j = 0; j < cols; ++j)
					_data[i + j * rows] = left._data[i] * right._data[0 + j * 1];
			}
		}
		else
			_data = nullptr;
		break;
	case tpArrayComplex:
		if (reinterpret_cast<const aidx*>(&right._number)[0] == 1)
			throw "Invalid operation over matrix (must be a vector column)";
		_type = right._type;
		reinterpret_cast<aidx*>(&_number)[0] = *reinterpret_cast<const vidx*>(&left._number);
		reinterpret_cast<aidx*>(&_number)[1] = reinterpret_cast<const aidx*>(&right._number)[1];
		if (left._data && right._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&_number)[0];
			aidx cols = reinterpret_cast<const aidx*>(&_number)[1];
			_data = new _TYPE[2 * rows * cols];
			for (aidx i = 0; i < rows; ++i)
			{
				for (aidx j = 0; j < cols; ++j)
				{
					_data[2 * (i + j * rows)] = left._data[i] * right._data[2 * (0 + j * 1)];
					_data[2 * (i + j * rows) + 1] = left._data[i] * right._data[2 * (0 + j * 1) + 1];
				}
			}
		}
		else
			_data = nullptr;
	default:
		throw "Incompatible types";
		break;
	}
}


template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void snumber<_TYPE, _INT, _CHAR, _SZSTR>::_product_lvectorimaginary(const snumber& left, const snumber& right)
{
	switch (right._type)
	{
	case tpReal:
	case tpImaginary:
		_type = right._type == tpReal ? tpVectorImaginary : tpVectorReal;
		_number = left._number;
		if (left._data)
		{
			_data = new _TYPE[*reinterpret_cast<const vidx*>(&_number)];
			if (right._type == tpReal)
				for (vidx i = 0; i < *reinterpret_cast<const vidx*>(&_number); ++i)
					_data[i] = left._data[i] * right._number;
			else
				for (vidx i = 0; i < *reinterpret_cast<const vidx*>(&_number); ++i)
					_data[i] = -(left._data[i] * right._number);
		}
		else
			_data = nullptr;
		break;
	case tpComplex:
		_type = right._data ? right._number ? tpVectorComplex : tpVectorReal : tpVectorImaginary;
		_number = left._number;
		if (left._data)
		{
			_data = new _TYPE[*reinterpret_cast<const vidx*>(&_number) * (_type == tpVectorComplex ? 2 : 1)];
			if (right._data)
			{
				if (_type == tpVectorReal)
					for (vidx i = 0; i < rows * cols; ++i)
						_data[i] = -(left._data[i] * *right._data);
				else
					for (vidx i = 0; i < rows * cols; ++i)
					{
						_data[2 * i] = -left._data[i] * *right._data;
						_data[2 * i + 1] = left._data[i] * right._number;
					}
			}
			else
				for (vidx i = 0; i < rows * cols; ++i)
					_data[i] = left._data[i] * right._number;
		}
		else
			_data = nullptr;
		break;
	case tpVectorReal:
	case tpVectorImaginary:
		if (*reinterpret_cast<const vidx*>(&left._number) != *reinterpret_cast<const vidx*>(&right._number))
			throw "The vectors must have equal dimension";
		_type = right._type == tpVectorReal ? tpImaginary : tpReal;
		_number = 0;
		if (left._data && right._data)
		{
			if (right._type == tpVectorReal)
				for (vidx i = 0; i < *reinterpret_cast<const vidx*>(&left._number); ++i)
					_number += left._data[i] * right._data[i];
			else
				for (vidx i = 0; i < *reinterpret_cast<const vidx*>(&left._number); ++i)
					_number -= left._data[i] * right._data[i];
		}
		_data = nullptr;
		break;
	case tpVectorComplex:
		if (*reinterpret_cast<const vidx*>(&left._number) != *reinterpret_cast<const vidx*>(&right._number))
			throw "The vectors must have equal dimension";
		_type = tpComplex;
		_number = 0;
		if (left._data && right._data)
		{
			_data = new _TYPE;
			*_data = 0;
			for (vidx i = 0; i < *reinterpret_cast<const vidx*>(&left._number); ++i)
			{
				_number += left._data[i] * right._data[2 * i + 1];
				*_data += left._data[i] * right._data[2 * i];
			}
		}
		else
			_data = nullptr;
		break;
	case tpArrayReal:
	case tpArrayImaginary:
		if (reinterpret_cast<const aidx*>(&right._number)[0] == 1)
			throw "Invalid operation over matrix (must be a vector column)";
		_type = right._type == tpArrayReal ? tpArrayImaginary : tpArrayReal;
		reinterpret_cast<aidx*>(&_number)[0] = *reinterpret_cast<const vidx*>(&left._number);
		reinterpret_cast<aidx*>(&_number)[1] = reinterpret_cast<const aidx*>(&right._number)[1];
		if (left._data && right._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&_number)[0];
			aidx cols = reinterpret_cast<const aidx*>(&_number)[1];
			_data = new _TYPE[rows * cols];
			if (right._type == tpArrayReal)
				for (aidx i = 0; i < rows; ++i)
				{
					for (aidx j = 0; j < cols; ++j)
						_data[i + j * rows] = left._data[i] * right._data[0 + j * 1];
				}
			else
				for (aidx i = 0; i < rows; ++i)
				{
					for (aidx j = 0; j < cols; ++j)
						_data[i + j * rows] = -left._data[i] * right._data[0 + j * 1];
				}
		}
		else
			_data = nullptr;
		break;
	case tpArrayComplex:
		if (reinterpret_cast<const aidx*>(&right._number)[0] == 1)
			throw "Invalid operation over matrix (must be a vector column)";
		_type = right._type;
		reinterpret_cast<aidx*>(&_number)[0] = *reinterpret_cast<const vidx*>(&left._number);
		reinterpret_cast<aidx*>(&_number)[1] = reinterpret_cast<const aidx*>(&right._number)[1];
		if (left._data && right._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&_number)[0];
			aidx cols = reinterpret_cast<const aidx*>(&_number)[1];
			_data = new _TYPE[2 * rows * cols];
			for (aidx i = 0; i < rows; ++i)
			{
				for (aidx j = 0; j < cols; ++j)
				{
					_data[2 * (i + j * rows)] = -left._data[i] * right._data[2 * (0 + j * 1) + 1];
					_data[2 * (i + j * rows) + 1] = left._data[i] * right._data[2 * (0 + j * 1)];
				}
			}
		}
		else
			_data = nullptr;
	default:
		throw "Incompatible types";
		break;
	}
}



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void snumber<_TYPE, _INT, _CHAR, _SZSTR>::_product_lvectorcomplex(const snumber& left, const snumber& right)
{
	switch (right._type)
	{
	case tpReal:
	case tpImaginary:
		_type = tpVectorComplex;
		_number = left._number;
		if (left._data)
		{
			_data = new _TYPE[2 * *reinterpret_cast<const vidx*>(&_number)];
			if (right._type == tpReal)
				for (vidx i = 0; i < 2 * *reinterpret_cast<const vidx*>(&_number); ++i)
					_data[i] = left._data[i] * right._number;
			else
				for (vidx i = 0; i < *reinterpret_cast<const vidx*>(&_number); ++i)
				{
					_data[2 * i] = -(left._data[2 * i + 1] * right._number);
					_data[2 * i + 1] = left._data[2 * i] * right._number;
				}
		}
		else
			_data = nullptr;
		break;
	case tpComplex:
		_type = tpVectorComplex;
		_number = left._number;
		if (left._data)
		{
			_data = new _TYPE[2 * *reinterpret_cast<const vidx*>(&_number)];
			if (right._data)
				for (vidx i = 0; i < rows * cols; ++i)
				{
					_data[2 * i] = left._data[2 * i] * right._number - left._data[2 * i + 1] * *right._data;
					_data[2 * i + 1] = left._data[2 * i] * *right._data + left._data[2 * i + 1] * right._number;
				}
			else
				for (vidx i = 0; i < rows * cols; ++i)
				{
					_data[2 * i] = left._data[2 * i] * right._number;
					_data[2 * i + 1] = left._data[2 * i + 1] * right._number;
				}
		}
		else
			_data = nullptr;
		break;
	case tpVectorReal:
	case tpVectorImaginary:
		if (*reinterpret_cast<const vidx*>(&left._number) != *reinterpret_cast<const vidx*>(&right._number))
			throw "The vectors must have equal dimension";
		_type = left._data && right._data ? tpComplex : tpReal;
		_number = 0;
		if (left._data && right._data)
		{
			_data = new _TYPE;
			*_data = 0;
			if (right._type == tpVectorReal)
				for (vidx i = 0; i < *reinterpret_cast<const vidx*>(&left._number); ++i)
				{
					_number += left._data[2 * i] * right._data[i];
					*_data += left._data[2 * i + 1] * right._data[i];
				}
			else
				for (vidx i = 0; i < *reinterpret_cast<const vidx*>(&left._number); ++i)
				{
					_number += left._data[2 * i + 1] * right._data[i];
					*_data += left._data[2 * i] * (-right._data[i]);
				}
		}
		else
			_data = nullptr;
		break;
	case tpVectorComplex:
		if (*reinterpret_cast<const vidx*>(&left._number) != *reinterpret_cast<const vidx*>(&right._number))
			throw "The vectors must have equal dimension";
		_type = left._data && right._data ? tpComplex : tpReal;
		_number = 0;
		if (left._data && right._data)
		{
			_data = new _TYPE;
			*_data = 0;
			for (vidx i = 0; i < *reinterpret_cast<const vidx*>(&left._number); ++i)
			{
				_number += left._data[2 * i] * right._data[2 * i] + left._data[2 * i + 1] * right._data[2 * i + 1];
				*_data += left._data[2 * i + 1] * right._data[2 * i] - left._data[2 * i] * right._data[2 * i + 1];
			}
		}
		else
			_data = nullptr;
		break;
	case tpArrayReal:
	case tpArrayImaginary:
		if (reinterpret_cast<const aidx*>(&right._number)[0] == 1)
			throw "Invalid operation over matrix (must be a vector column)";
		_type = tpArrayComplex;
		reinterpret_cast<aidx*>(&_number)[0] = *reinterpret_cast<const vidx*>(&left._number);
		reinterpret_cast<aidx*>(&_number)[1] = reinterpret_cast<const aidx*>(&right._number)[1];
		if (left._data && right._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&_number)[0];
			aidx cols = reinterpret_cast<const aidx*>(&_number)[1];
			_data = new _TYPE[2 * rows * cols];
			if (right._type == tpArrayReal)
				for (aidx i = 0; i < rows; ++i)
				{
					for (aidx j = 0; j < cols; ++j)
					{
						_data[2 * (i + j * rows)] = left._data[2 * i] * right._data[0 + j * 1];
						_data[2 * (i + j * rows) + 1] = left._data[2 * i + 1] * right._data[0 + j * 1];
					}
				}
			else
				for (aidx i = 0; i < rows; ++i)
				{
					for (aidx j = 0; j < cols; ++j)
					{
						_data[2 * (i + j * rows)] = -(left._data[2 * i + 1] * right._data[0 + j * 1]);
						_data[2 * (i + j * rows) + 1] = left._data[2 * i] * right._data[0 + j * 1];
					}
				}
		}
		else
			_data = nullptr;
		break;
	case tpArrayComplex:
		if (reinterpret_cast<const aidx*>(&right._number)[0] == 1)
			throw "Invalid operation over matrix (must be a vector column)";
		_type = tpArrayComplex;
		reinterpret_cast<aidx*>(&_number)[0] = *reinterpret_cast<const vidx*>(&left._number);
		reinterpret_cast<aidx*>(&_number)[1] = *reinterpret_cast<const aidx*>(&right._number)[1];
		if (left._data && right._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&_number)[0];
			aidx cols = reinterpret_cast<const aidx*>(&_number)[1];
			_data = new _TYPE[2 * rows * cols];
			for (aidx i = 0; i < rows; ++i)
			{
				for (aidx j = 0; j < cols; ++j)
				{
					_data[2 * (i + j * rows)] = left._data[2 * i] * right._data[2 * (0 + j * 1)] - left._data[2 * i + 1] * right._data[2 * (0 + j * 1) + 1];
					_data[2 * (i + j * rows) + 1] = left._data[2 * i + 1] * right._data[2 * (0 + j * 1)] + left[2 * i] * right._data[2 * (0 + j * 1) + 1];
				}
			}
		}
		else
			_data = nullptr;
	default:
		throw "Incompatible types";
		break;
	}
}




template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void snumber<_TYPE, _INT, _CHAR, _SZSTR>::_product_larrayreal(const snumber& left, const snumber& right)
{
	switch (right._type)
	{
	case tpReal:
	case tpImaginary:
		_type = right._type == tpReal ? tpArrayReal : tpArrayImaginary;
		_number = left._number;
		if (left._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&_number)[0];
			aidx cols = reinterpret_cast<const aidx*>(&_number)[1];
			_data = new _TYPE[rows * cols];
			for (vidx i = 0; i < rows * cols; ++i)
				_data[i] = left._data[i] * right._number;
		}
		else
			_data = nullptr;
		break;
	case tpComplex:
		_type = right._number && right._data ? tpArrayComplex : right._data ? tpArrayImaginary : tpArrayReal;
		_number = left._number;
		if (left._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&_number)[0];
			aidx cols = reinterpret_cast<const aidx*>(&_number)[1];
			_data = new _TYPE[*reinterpret_cast<const vidx*>(&_number) * (_type == tpArrayComplex ? 2 : 1)];
			if (_type == tpArrayComplex)
				for (vidx i = 0; i < rows * cols; ++i)
				{
					_data[2 * i] = left._data[i] * right._number;
					_data[2 * i + 1] = left._data[i] * *right._data;
				}
			else if (right._data)
				for (vidx i = 0; i < rows * cols; ++i)
					_data[i] = left._data[i] * *right._data;
			else
				for (vidx i = 0; i < rows * cols; ++i)
					_data[i] = left._data[i] * right._number;
		}
		else
			_data = nullptr;
		break;
	case tpVectorReal:
	case tpVectorImaginary:
		if (reinterpret_cast<const aidx*>(&left._number)[1] != *reinterpret_cast<const vidx*>(&right._number))
			throw "The vector dimension must be equal to matrix columns";
		_type = right._type;
		*reinterpret_cast<vidx*>(&_number) = reinterpret_cast<const aidx*>(&left._number)[0];
		if (left._data && right._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&left._number)[0];
			aidx m = reinterpret_cast<const aidx*>(&left._number)[1];
			_data = new _TYPE[rows];
			for (aidx i = 0; i < rows; ++i)
				for (aidx j = 0; j < 1; ++j)
				{
					if (0 < cols)
						_data[i + j * rows] = left._data[i + 0 * rows] * right._data[0 + j * 1];
					for (aidx k = 1; k < cols; ++k)
						_data[i + j * rows] += left._data[i + k * rows] * right._data[k + j * 1];
				}
		}
		else
			_data = nullptr;
		break;
	case tpVectorComplex:
		if (reinterpret_cast<const aidx*>(&left._number)[1] != *reinterpret_cast<const vidx*>(&right._number))
			throw "The vector dimension must be equal to matrix columns";
		_type = tpVectorComplex;
		*reinterpret_cast<vidx*>(&_number) = reinterpret_cast<const aidx*>(&left._number)[0];
		if (left._data && right._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&left._number)[0];
			aidx m = reinterpret_cast<const aidx*>(&left._number)[1];
			_data = new _TYPE[2 * rows];
			for (aidx i = 0; i < rows; ++i)
				for (aidx j = 0; j < 1; ++j)
				{
					if (0 < cols)
					{
						_data[2 * (i + j * rows)] = left._data[i + 0 * rows] * right._data[2 * (0 + j * 1)];
						_data[2 * (i + j * rows) + 1] = left._data[i + 0 * rows] * right._data[2 * (0 + j * 1) + 1];
					}
					for (aidx k = 1; k < cols; ++k)
					{
						_data[2 * (i + j * rows)] += left._data[i + k * rows] * right._data[2 * (k + j * 1)];
						_data[2 * (i + j * rows) + 1] += left._data[i + k * rows] * right._data[2 (k + j * 1) + 1];
					}
				}
		}
		else
			_data = nullptr;
		break;
	case tpArrayReal:
	case tpArrayImaginary:
		if (reinterpret_cast<const aidx*>(&left._number)[1] == reinterpret_cast<const aidx*>(&right._number)[0])
			throw "Invalid operation over matrix ([mxn] [nxp] = [mxp])";
		_type = right._type;
		reinterpret_cast<aidx*>(&_number)[0] = reinterpret_cast<const aidx*>(&left._number)[0];
		reinterpret_cast<aidx*>(&_number)[1] = reinterpret_cast<const aidx*>(&right._number)[1];
		if (left._data && right._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&_number)[0];
			aidx cols = reinterpret_cast<const aidx*>(&_number)[1];
			aidx m = reinterpret_cast<const aidx*>(&right._number)[0];
			_data = new _TYPE[rows * cols];
			for (aidx i = 0; i < rows; ++i)
			{
				for (aidx j = 0; j < cols; ++j)
				{
					if (0 < m)
						_data[i + j * rows] = left._data[i + 0 * rows] * right._data[0 + j * m];
					for (aidx k = 1; k < m; ++k)
						_data[i + j * rows] += left._data[i + k * rows] * right._data[k + j * m];
				}
			}
		}
		else
			_data = nullptr;
		break;
	case tpArrayComplex:
		if (reinterpret_cast<const aidx*>(&left._number)[1] == reinterpret_cast<const aidx*>(&right._number)[0])
			throw "Invalid operation over matrix ([mxn] [nxp] = [mxp])";
		_type = tpArrayComplex;
		reinterpret_cast<aidx*>(&_number)[0] = reinterpret_cast<const aidx*>(&left._number)[0];
		reinterpret_cast<aidx*>(&_number)[1] = reinterpret_cast<const aidx*>(&right._number)[1];
		if (left._data && right._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&_number)[0];
			aidx cols = reinterpret_cast<const aidx*>(&_number)[1];
			aidx m = reinterpret_cast<const aidx*>(&right._number)[0];
			_data = new _TYPE[2 * rows * cols];
			for (aidx i = 0; i < rows; ++i)
			{
				for (aidx j = 0; j < cols; ++j)
				{
					if (0 < m)
					{
						_data[2 * (i + j * rows)] = left._data[i + 0 * rows] * right._data[2 * (0 + j * m)];
						_data[2 * (i + j * rows) + 1] += left._data[i + 0 * rows] * right._data[2 * (0 + j * m) + 1];
					}
					for (aidx k = 1; k < m; ++k)
					{
						_data[2 * (i + j * rows)] += left._data[i + k * rows] * right._data[2 * (k + j * m)];
						_data[2 * (i + j * rows) + 1] += left._data[i + k * rows] * right._data[2 * (k + j * m) + 1];
					}
				}
			}
		}
		else
			_data = nullptr;
		break;
	default:
		throw "Incompatible types";
		break;
	}
}




template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void snumber<_TYPE, _INT, _CHAR, _SZSTR>::_product_larrayimaginary(const snumber& left, const snumber& right)
{
	switch (right._type)
	{
	case tpReal:
	case tpImaginary:
		_type = right._type == tpReal ? tpArrayImaginary : tpArrayReal;
		_number = left._number;
		if (left._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&_number)[0];
			aidx cols = reinterpret_cast<const aidx*>(&_number)[1];
			_data = new _TYPE[rows * cols];
			if (right._type == tpReal)
				for (vidx i = 0; i < rows * cols; ++i)
					_data[i] = left._data[i] * right._number;
			else
				for (vidx i = 0; i < rows * cols; ++i)
					_data[i] = -(left._data[i] * right._number);
		}
		else
			_data = nullptr;
		break;
	case tpComplex:
		_type = right._number && right._data ? tpArrayComplex : !right._data ? tpArrayImaginary : tpArrayReal;
		_number = left._number;
		if (left._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&_number)[0];
			aidx cols = reinterpret_cast<const aidx*>(&_number)[1];
			_data = new _TYPE[*reinterpret_cast<const vidx*>(&_number) * (_type == tpArrayComplex ? 2 : 1)];
			if (_type == tpArrayComplex)
				for (vidx i = 0; i < rows * cols; ++i)
				{
					_data[2 * i] = -(left._data[i] * *right._data);
					_data[2 * i + 1] = left._data[i] * right._number;
				}
			else if (right._data)
				for (vidx i = 0; i < rows * cols; ++i)
					_data[i] = -(left._data[i] * *right._data);
			else
				for (vidx i = 0; i < rows * cols; ++i)
					_data[i] = left._data[i] * right._number;
		}
		else
			_data = nullptr;
		break;
	case tpVectorReal:
	case tpVectorImaginary:
		if (reinterpret_cast<const aidx*>(&left._number)[1] != *reinterpret_cast<const vidx*>(&right._number))
			throw "The vector dimension must be equal to matrix columns";
		_type = right._type == tpVectorReal ? tpVectorImaginary : tpVectorReal;
		*reinterpret_cast<vidx*>(&_number) = reinterpret_cast<const aidx*>(&left._number)[0];
		if (left._data && right._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&left._number)[0];
			aidx m = reinterpret_cast<const aidx*>(&left._number)[1];
			_data = new _TYPE[rows];
			int fi = right._type == tpVectorReal ? -1 : 1;
			for (aidx i = 0; i < rows; ++i)
				for (aidx j = 0; j < 1; ++j)
				{
					if (0 < cols)
						_data[i + j * rows] = fi * left._data[i + 0 * rows] * right._data[0 + j * 1];
					for (aidx k = 1; k < cols; ++k)
						_data[i + j * rows] += fi * left._data[i + k * rows] * right._data[k + j * 1];
				}
		}
		else
			_data = nullptr;
		break;
	case tpVectorComplex:
		if (reinterpret_cast<const aidx*>(&left._number)[1] != *reinterpret_cast<const vidx*>(&right._number))
			throw "The vector dimension must be equal to matrix columns";
		_type = tpVectorComplex;
		*reinterpret_cast<vidx*>(&_number) = reinterpret_cast<const aidx*>(&left._number)[0];
		if (left._data && right._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&left._number)[0];
			aidx m = reinterpret_cast<const aidx*>(&left._number)[1];
			_data = new _TYPE[2 * rows];
			for (aidx i = 0; i < rows; ++i)
				for (aidx j = 0; j < 1; ++j)
				{
					if (0 < cols)
					{
						_data[2 * (i + j * rows)] = -(left._data[i + 0 * rows] * right._data[2 * (0 + j * 1) + 1]);
						_data[2 * (i + j * rows) + 1] = left._data[i + 0 * rows] * right._data[2 * (0 + j * 1)];
					}
					for (aidx k = 1; k < cols; ++k)
					{
						_data[2 * (i + j * rows)] -= left._data[i + k * rows] * right._data[2 * (k + j * 1) + 1];
						_data[2 * (i + j * rows) + 1] += left._data[i + k * rows] * right._data[2 (k + j * 1)];
					}
				}
		}
		else
			_data = nullptr;
		break;
	case tpArrayReal:
	case tpArrayImaginary:
		if (reinterpret_cast<const aidx*>(&left._number)[1] == reinterpret_cast<const aidx*>(&right._number)[0])
			throw "Invalid operation over matrix ([mxn] [nxp] = [mxp])";
		_type = right._type == tpArrayReal ? tpArrayImaginary : tpArrayReal;
		reinterpret_cast<aidx*>(&_number)[0] = reinterpret_cast<const aidx*>(&left._number)[0];
		reinterpret_cast<aidx*>(&_number)[1] = reinterpret_cast<const aidx*>(&right._number)[1];
		if (left._data && right._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&_number)[0];
			aidx cols = reinterpret_cast<const aidx*>(&_number)[1];
			aidx m = reinterpret_cast<const aidx*>(&right._number)[0];
			int fi = _type == tpArrayReal ? -1 : 1;
			_data = new _TYPE[rows * cols];
			for (aidx i = 0; i < rows; ++i)
			{
				for (aidx j = 0; j < cols; ++j)
				{
					if (0 < m)
						_data[i + j * rows] = fi * left._data[i + 0 * rows] * right._data[0 + j * m];
					for (aidx k = 1; k < m; ++k)
						_data[i + j * rows] += fi * left._data[i + k * rows] * right._data[k + j * m];
				}
			}
		}
		else
			_data = nullptr;
		break;
	case tpArrayComplex:
		if (reinterpret_cast<const aidx*>(&left._number)[1] == reinterpret_cast<const aidx*>(&right._number)[0])
			throw "Invalid operation over matrix ([mxn] [nxp] = [mxp])";
		_type = tpArrayComplex;
		reinterpret_cast<aidx*>(&_number)[0] = reinterpret_cast<const aidx*>(&left._number)[0];
		reinterpret_cast<aidx*>(&_number)[1] = reinterpret_cast<const aidx*>(&right._number)[1];
		if (left._data && right._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&_number)[0];
			aidx cols = reinterpret_cast<const aidx*>(&_number)[1];
			aidx m = reinterpret_cast<const aidx*>(&right._number)[0];
			_data = new _TYPE[2 * rows * cols];
			for (aidx i = 0; i < rows; ++i)
			{
				for (aidx j = 0; j < cols; ++j)
				{
					if (0 < m)
					{
						_data[2 * (i + j * rows)] = -(left._data[i + 0 * rows] * right._data[2 * (0 + j * m) + 1]);
						_data[2 * (i + j * rows) + 1] += left._data[i + 0 * rows] * right._data[2 * (0 + j * m)];
					}
					for (aidx k = 1; k < m; ++k)
					{
						_data[2 * (i + j * rows)] -= left._data[i + k * rows] * right._data[2 * (k + j * m) + 1];
						_data[2 * (i + j * rows) + 1] += left._data[i + k * rows] * right._data[2 * (k + j * m)];
					}
				}
			}
		}
		else
			_data = nullptr;
		break;
	default:
		throw "Incompatible types";
		break;
	}
}




template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
inline void snumber<_TYPE, _INT, _CHAR, _SZSTR>::_product_larraycomplex(const snumber& left, const snumber& right)
{
	switch (right._type)
	{
	case tpReal:
	case tpImaginary:
		_type = tpArrayComplex;
		_number = left._number;
		if (left._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&_number)[0];
			aidx cols = reinterpret_cast<const aidx*>(&_number)[1];
			_data = new _TYPE[2 * rows * cols];
			if (right._type == tpReal)
				for (vidx i = 0; i < 2 * rows * cols; ++i)
					_data[i] = left._data[i] * right._number;
			else
				for (vidx i = 0; i < rows * cols; ++i)
				{
					_data[2 * i] = -(left._data[2 * i + 1] * right._number);
					_data[2 * i + 1] = left._data[2 * i] * right._number;
				}
		}
		else
			_data = nullptr;
		break;
	case tpComplex:
		_type = tpArrayComplex;
		_number = left._number;
		if (left._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&_number)[0];
			aidx cols = reinterpret_cast<const aidx*>(&_number)[1];
			_data = new _TYPE[2 * rows * cols];
			for (vidx i = 0; i < rows * cols; ++i)
			{
				_data[2 * i] = left._data[2 * i] * right._number - (right._data ? left._data[2 * i + 1] * *right._data : 0);
				_data[2 * i + 1] = left._data[2 * i + 1] * right._number + (right._data ? left._data[2 * i] * *right._data: 0);
			}
		}
		else
			_data = nullptr;
		break;
	case tpVectorReal:
	case tpVectorImaginary:
		if (reinterpret_cast<const aidx*>(&left._number)[1] != *reinterpret_cast<const vidx*>(&right._number))
			throw "The vector dimension must be equal to matrix columns";
		_type = tpVectorComplex;
		*reinterpret_cast<vidx*>(&_number) = reinterpret_cast<const aidx*>(&left._number)[0];
		if (left._data && right._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&left._number)[0];
			aidx m = reinterpret_cast<const aidx*>(&left._number)[1];
			_data = new _TYPE[rows];
			if (right._type == tpVectorReal)
				for (aidx i = 0; i < rows; ++i)
					for (aidx j = 0; j < 1; ++j)
					{
						if (0 < cols)
						{
							_data[2 * (i + j * rows)] = left._data[2 * (i + 0 * rows)] * right._data[0 + j * 1];
							_data[2 * (i + j * rows) + 1] = left._data[2 * (i + 0 * rows) + 1] * right._data[0 + j * 1];
						}
						for (aidx k = 1; k < cols; ++k)
						{
							_data[2 * (i + j * rows)] += left._data[2 * (i + k * rows)] * right._data[k + j * 1];
							_data[2 * (i + j * rows) + 1] += left._data[2 * (i + k * rows) + 1] * right._data[k + j * 1];
						}
					}
			else
				for (aidx i = 0; i < rows; ++i)
					for (aidx j = 0; j < 1; ++j)
					{
						if (0 < cols)
						{
							_data[2 * (i + j * rows)] = -(left._data[2 * (i + 0 * rows) + 1] * right._data[0 + j * 1]);
							_data[2 * (i + j * rows) + 1] = left._data[2 * (i + 0 * rows)] * right._data[0 + j * 1];
						}
						for (aidx k = 1; k < cols; ++k)
						{
							_data[2 * (i + j * rows)] -= left._data[2 * (i + k * rows) + 1] * right._data[k + j * 1];
							_data[2 * (i + j * rows) + 1] += left._data[2 * (i + k * rows)] * right._data[k + j * 1];
						}
					}
		}
		else
			_data = nullptr;
		break;
	case tpVectorComplex:
		if (reinterpret_cast<const aidx*>(&left._number)[1] != *reinterpret_cast<const vidx*>(&right._number))
			throw "The vector dimension must be equal to matrix columns";
		_type = tpVectorComplex;
		*reinterpret_cast<vidx*>(&_number) = reinterpret_cast<const aidx*>(&left._number)[0];
		if (left._data && right._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&left._number)[0];
			aidx m = reinterpret_cast<const aidx*>(&left._number)[1];
			_data = new _TYPE[2 * rows];
			for (aidx i = 0; i < rows; ++i)
				for (aidx j = 0; j < 1; ++j)
				{
					if (0 < cols)
					{
						_data[2 * (i + j * rows)] = left._data[2 * (i + 0 * rows)] * right._data[2 * (0 + j * 1)] - (left._data[2 * (i + 0 * rows) + 1] * right._data[2 * (0 + j * 1) + 1]);
						_data[2 * (i + j * rows) + 1] = left._data[2 * (i + 0 * rows) + 1] * right._data[2 * (0 + j * 1)] + left._data[2 * (i + 0 * rows)] * right._data[2 * (0 + j * 1) + 1];
					}
					for (aidx k = 1; k < cols; ++k)
					{
						_data[2 * (i + j * rows)] = left._data[2 * (i + k * rows)] * right._data[2 * (k + j * 1)] - (left._data[2 * (i + k * rows) + 1] * right._data[2 * (k + j * 1) + 1]);
						_data[2 * (i + j * rows) + 1] = left._data[2 * (i + k * rows) + 1] * right._data[2 * (k + j * 1)] + left._data[2 * (i + k * rows)] * right._data[2 * (k + j * 1) + 1];
					}
				}
		}
		else
			_data = nullptr;
		break;
	case tpArrayReal:
	case tpArrayImaginary:
		if (reinterpret_cast<const aidx*>(&left._number)[1] == reinterpret_cast<const aidx*>(&right._number)[0])
			throw "Invalid operation over matrix ([mxn] [nxp] = [mxp])";
		_type = tpArrayComplex;
		reinterpret_cast<aidx*>(&_number)[0] = reinterpret_cast<const aidx*>(&left._number)[0];
		reinterpret_cast<aidx*>(&_number)[1] = reinterpret_cast<const aidx*>(&right._number)[1];
		if (left._data && right._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&_number)[0];
			aidx cols = reinterpret_cast<const aidx*>(&_number)[1];
			aidx m = reinterpret_cast<const aidx*>(&right._number)[0];
			_data = new _TYPE[2 * rows * cols];
			if (right._type == tpArrayReal)
				for (aidx i = 0; i < rows; ++i)
				{
					for (aidx j = 0; j < cols; ++j)
					{
						if (0 < m)
						{
							_data[2 * (i + j * rows)] = left._data[2 * (i + 0 * rows)] * right._data[0 + j * m];
							_data[2 * (i + j * rows) + 1] = left._data[2 * (i + 0 * rows) + 1] * right._data[0 + j * m];
						}
						for (aidx k = 1; k < m; ++k)
						{
							_data[2 * (i + j * rows)] += left._data[2 * (i + k * rows)] * right._data[k + j * m];
							_data[2 * (i + j * rows) + 1] += left._data[2 * (i + k * rows) + 1] * right._data[k + j * m];
						}
					}
				}
			else
				for (aidx i = 0; i < rows; ++i)
				{
					for (aidx j = 0; j < cols; ++j)
					{
						if (0 < m)
						{
							_data[2 * (i + j * rows)] = -(left._data[2 * (i + 0 * rows) + 1] * right._data[0 + j * m]);
							_data[2 * (i + j * rows) + 1] = left._data[2 * (i + 0 * rows)] * right._data[0 + j * m];
						}
						for (aidx k = 1; k < m; ++k)
						{
							_data[2 * (i + j * rows)] -= left._data[2 * (i + k * rows) + 1] * right._data[k + j * m];
							_data[2 * (i + j * rows) + 1] += left._data[2 * (i + k * rows)] * right._data[k + j * m];
						}
					}
				}
		}
		else
			_data = nullptr;
		break;
	case tpArrayComplex:
		if (reinterpret_cast<const aidx*>(&left._number)[1] == reinterpret_cast<const aidx*>(&right._number)[0])
			throw "Invalid operation over matrix ([mxn] [nxp] = [mxp])";
		_type = tpArrayComplex;
		reinterpret_cast<aidx*>(&_number)[0] = reinterpret_cast<const aidx*>(&left._number)[0];
		reinterpret_cast<aidx*>(&_number)[1] = reinterpret_cast<const aidx*>(&right._number)[1];
		if (left._data && right._data)
		{
			aidx rows = reinterpret_cast<const aidx*>(&_number)[0];
			aidx cols = reinterpret_cast<const aidx*>(&_number)[1];
			aidx m = reinterpret_cast<const aidx*>(&right._number)[0];
			_data = new _TYPE[2 * rows * cols];
			for (aidx i = 0; i < rows; ++i)
			{
				for (aidx j = 0; j < cols; ++j)
				{
					if (0 < m)
					{
						_data[2 * (i + j * rows)] = left._data[2 * (i + 0 * rows)] * right._data[2 * (0 + j * m)] - left._data[2 * (i + 0 * rows) + 1] * right._data[2 * (0 + j * m) + 1];
						_data[2 * (i + j * rows) + 1] = left._data[2 * (i + 0 * rows) + 1] * right._data[2 * (0 + j * m)] + left._data[2 * (i + 0 * rows)] * right._data[2 * (0 + j * m) + 1];
					}
					for (aidx k = 1; k < m; ++k)
					{
						_data[2 * (i + j * rows)] += left._data[2 * (i + k * rows)] * right._data[2 * (k + j * m)] - left._data[2 * (i + k * rows) + 1] * right._data[2 * (k + j * m) + 1];
						_data[2 * (i + j * rows) + 1] += left._data[2 * (i + k * rows) + 1] * right._data[2 * (k + j * m)] + left._data[2 * (i + k * rows)] * right._data[2 * (k + j * m) + 1];
					}
				}
			}
		}
		else
			_data = nullptr;
		break;
	default:
		throw "Incompatible types";
		break;
	}
}







}
}
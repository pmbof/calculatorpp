#include "StdAfx.h"

#pragma once
#include "pmb_parser_item.h"


namespace pmb
{
namespace parser
{



template <typename _CHAR, typename _SIZE>
inline item<_CHAR, _SIZE>::item(_SIZE ini, _SIZE end)
	: _ini(ini), _end(end)
{
}


template <typename _CHAR, typename _SIZE>
inline _SIZE item<_CHAR, _SIZE>::len() const
{
	return _end - _ini;
}


template <typename _CHAR, typename _SIZE>
inline typename item<_CHAR, _SIZE>::string item<_CHAR, _SIZE>::getString(_CPTRCHAR expr) const
{
	return string(expr + _ini, len());
}


template <typename _CHAR, typename _SIZE>
inline typename item<_CHAR, _SIZE>::_CPTRCHAR item<_CHAR, _SIZE>::getCharPtr(_CPTRCHAR expr) const
{
	return expr + _ini;
}

template <typename _CHAR, typename _SIZE>
inline _SIZE item<_CHAR, _SIZE>::getIni() const
{
	return _ini;
}


template <typename _CHAR, typename _SIZE>
inline _SIZE item<_CHAR, _SIZE>::getEnd() const
{
	return _end;
}




template<typename _CHAR, typename _SIZE>
item<_CHAR, _SIZE>::string::string()
	: _str(nullptr)
{
}

template<typename _CHAR, typename _SIZE>
item<_CHAR, _SIZE>::string::string(_CPTRCHAR str, SIZETP size)
	: _str(str), _size(size)
{
}

template<typename _CHAR, typename _SIZE>
_CHAR item<_CHAR, _SIZE>::string::operator[](SIZETP i) const
{
	return i < _size ? _str[i] : '\0';
}

template<typename _CHAR, typename _SIZE>
item<_CHAR, _SIZE>::string::operator const _CHAR*() const
{
	return _str;
}

template<typename _CHAR, typename _SIZE>
item<_CHAR, _SIZE>::string::operator _SIZE() const
{
	return _size;
}

template<typename _CHAR, typename _SIZE>
typename item<_CHAR, _SIZE>::string item<_CHAR, _SIZE>::string::operator+(SIZETP right) const
{
	return string(_str + right, _size - right);
}

template<typename _CHAR, typename _SIZE>
std::string item<_CHAR, _SIZE>::string::getString() const
{
	return std::string(_str, _size);
}

template <typename _CHAR, typename _SIZE>
inline bool item<_CHAR, _SIZE>::string::operator <(const std::string& right) const
{
	std::string::size_type i;
	for(i = 0; i < right.length() && i < _size && _str[i] == right[i]; ++i)
		;
	bool bRet = i < _size && i < right.length() && _str[i] < right[i] || i == _size && i < right.length();

//	std::string l(left._str, left._size);
//	log::getInstance()->trace("1: \"%s\" < \"%s\"\t==\t%s[%s]: %s\n", l.c_str(), right.c_str(), bRet ? "TRUE": "FALSE", l < right ? "true": "false", 
//		bRet == l < right ? "Ok!": "BAD!!!");
	return bRet;
}


template <typename _CHAR, typename _SIZE>
inline bool item<_CHAR, _SIZE>::string::operator >(const std::string& right) const
{
	std::string::size_type i;
	for (i = 0; i < right.length() && i < _size && right[i] == _str[i]; ++i)
		;
	bool bRet = i < right.length() && i < _size && right[i] < _str[i] || i == right.length() && i < _size;

	//	std::string r(right._str, right._size);
	//	log::getInstance()->trace("2: \"%s\" < \"%s\"\t==\t%s[%s]: %s\n", left.c_str(), r.c_str(), bRet ? "TRUE": "FALSE", left < r ? "true": "false", 
	//		bRet == left < r ? "Ok!": "BAD!!!");
	return bRet;
}

template<typename _CHAR, typename _SIZE>
void item<_CHAR, _SIZE>::string::clear()
{
	_str = nullptr;
}

template<typename _CHAR, typename _SIZE>
bool item<_CHAR, _SIZE>::string::operator!() const
{
	return !_str || !_size;
}

template<typename _CHAR, typename _SIZE>
item<_CHAR, _SIZE>::string::operator bool() const
{
	return _str && _size;
}

template<typename _CHAR, typename _SIZE>
inline typename item<_CHAR, _SIZE>::_CPTRCHAR item<_CHAR, _SIZE>::string::str() const
{
	return _str;
}

template<typename _CHAR, typename _SIZE>
inline typename item<_CHAR, _SIZE>::SIZETP item<_CHAR, _SIZE>::string::size() const
{
	return _size;
}



template <typename _CHAR, typename _SIZE>
bool operator <(const std::string& left, typename const item<_CHAR, _SIZE>::string& right)
{
	typename _LEFTSTR::size_type i;
	for(i = 0; i < left.length() && i < right._size && left[i] == right._str[i]; ++i)
		;
	bool bRet = i < left.length() && i < right._size && left[i] < right._str[i] || i == left.length() && i < right._size;
	
	not found;
	util::map line 24;
//	std::string r(right._str, right._size);
//	log::getInstance()->trace("2: \"%s\" < \"%s\"\t==\t%s[%s]: %s\n", left.c_str(), r.c_str(), bRet ? "TRUE": "FALSE", left < r ? "true": "false", 
//		bRet == left < r ? "Ok!": "BAD!!!");
	return bRet;
}




}
}
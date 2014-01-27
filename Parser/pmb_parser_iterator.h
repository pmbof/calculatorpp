#pragma once
#include "pmb_parser_node.h"


namespace pmb
{
namespace parser
{

struct wrapper 
{
};


struct iterator
{
	const char* _expr;
	int _i;

//	iterator(const char* expr): _expr(expr), _i(0) { }

//	iterator(const iterator& it): _expr(it._expr + it._i), _i(0) { }

	iterator& operator= (const char* str)
	{
		_expr = str;
		_i = 0;
		return *this;
	}

	inline char operator()() const
	{
		return _expr[_i];
	}
	inline char operator()(int i) const
	{
		return _expr[_i + i];
	}

	inline const char* operator+(int ini) const
	{
		return _expr + ini;
	}

	inline iterator& operator++()
	{
		++_i;
		return *this;
	}
	inline iterator& operator--()
	{
		--_i;
		return *this;
	}
	inline iterator& operator+=(int i)
	{
		_i += i;
		return *this;
	}
	inline iterator& operator-=(int i)
	{
		_i -= i;
		return *this;
	}

	inline bool operator <(int i) const
	{
		return _i < i; 
	}
	inline bool operator >(int i) const
	{
		return _i > i;
	}
	inline bool operator ==(int i) const
	{
		return _i == i;
	}

	inline bool operator !() const
	{
		return !_i;
	}

	inline operator int() const
	{
		return _i;
	}

	inline operator const char*() const
	{
		return _expr;
	}

	inline char lower() const
	{
		return tolower(_expr[_i]);
	}
	inline char upper() const
	{
		return toupper(_expr[_i]);
	}
	inline bool isSpace() const
	{
		return _expr[_i] == ' ' || _expr[_i] == '\t' || _expr[_i] == '\n';
	}

	friend inline bool operator <(int i, const iterator& expr);
	friend inline bool operator >(int i, const iterator& expr);
	friend inline bool operator ==(int i, const iterator& expr);
};


bool operator <(int i, const iterator& expr)
{
	return i < expr._i;
}
bool operator >(int i, const iterator& expr)
{
	return i > expr._i;
}
bool operator ==(int i, const iterator& expr)
{
	return i == expr._i;
}





}
}
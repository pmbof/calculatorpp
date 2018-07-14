#pragma once
#include "pmb_parser_node.h"


namespace pmb
{
namespace parser
{


struct wrapper 
{
};


template<typename _CHAR, typename _IDX>
struct iterator
{
	typedef const _CHAR* tdstring;
	typedef _IDX idx;

	tdstring _expr;
	idx _i;

//	iterator(tdstring expr): _expr(expr), _i(0) { }

//	iterator(const iterator& it): _expr(it._expr + it._i), _i(0) { }

	iterator& operator= (tdstring str)
	{
		_expr = str;
		_i = 0;
		return *this;
	}

	inline char operator()() const
	{
		return _expr[_i];
	}
	inline char operator()(idx i) const
	{
		return _expr[_i + i];
	}

	inline tdstring operator+(idx ini) const
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
	inline iterator& operator+=(idx i)
	{
		_i += i;
		return *this;
	}
	inline iterator& operator-=(idx i)
	{
		_i -= i;
		return *this;
	}

	inline bool operator <(idx i) const
	{
		return _i < i; 
	}
	inline bool operator >(idx i) const
	{
		return _i > i;
	}
	inline bool operator ==(idx i) const
	{
		return _i == i;
	}

	inline bool operator !() const
	{
		return !_i;
	}

	inline explicit operator idx() const
	{
		return _i;
	}

	inline operator tdstring() const
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


//	Friends:
	template<typename _CHAR, typename _IDX>
	friend inline bool operator <(_IDX i, const iterator& expr);

	template<typename _CHAR, typename _IDX>
	friend inline bool operator >(_IDX i, const iterator& expr);
	
	template<typename _CHAR, typename _IDX>
	friend inline bool operator ==(_IDX i, const iterator& expr);
};




template<typename _CHAR, typename _IDX>
bool operator <(_IDX i, const iterator<_CHAR, _IDX>& expr)
{
	return i < expr._i;
}

template<typename _CHAR, typename _IDX>
bool operator >(_IDX i, const iterator<_CHAR, _IDX>& expr)
{
	return i > expr._i;
}

template<typename _CHAR, typename _IDX>
bool operator ==(_IDX i, const iterator<_CHAR, _IDX>& expr)
{
	return i == expr._i;
}





}
}
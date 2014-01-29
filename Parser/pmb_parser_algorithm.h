#pragma once

#include "pmb_parser_tree.h"
#include "pmb_parser_iterator.h"

#include "pmb_parser_iterators_unknow.h"
#include "pmb_parser_iterators_space.h"
#include "pmb_parser_iterators_alpha.h"
#include "pmb_parser_iterators_number.h"
#include "pmb_parser_iterators_string.h"
#include "pmb_parser_iterators_parentheses.h"
#include "pmb_parser_iterators_list.h"

#include "pmb_parser_operation.h"
#include "pmb_parser_symbol.h"



namespace pmb
{
namespace parser
{



template <int i, class _IT, class _TVALUE>
struct auto_iterator
{
	auto_iterator(_IT& expr): _next1(expr), _space(expr), _unknow(expr) { }

	node<_TVALUE>* operator()()
	{
		_unknow.initLoop();
		node<_TVALUE>* nd = _space(); 
		if(!nd && !(nd = _next1()))
			nd = _unknow();
		return nd;
	}

	void initLoop()
	{
		_unknow.initLoop();
	}

	node<_TVALUE>* release()
	{
		return _unknow.release();
	}

	bool space()
	{
		return _space.release();
	}


protected:
	auto_iterator<1, _IT, _TVALUE> _next1;
	iterators::space<_IT, wrapper, _TVALUE> _space;
	iterators::unknow<_IT, wrapper, _TVALUE> _unknow;
};


template <class _IT, class _TVALUE>
struct auto_iterator<5, _IT, _TVALUE>
{
	auto_iterator(_IT& expr): _list(expr) { }

	node<_TVALUE>* operator()()
	{
		return _list(); 
	}

protected:
	iterators::list<_IT, wrapper, _TVALUE> _list;
};


template <class _IT, class _TVALUE>
struct auto_iterator<4, _IT, _TVALUE>
{
	auto_iterator(_IT& expr): _next5(expr), _parentheses(expr) { }

	node<_TVALUE>* operator()()
	{
		node<_TVALUE>* nd = _parentheses();
		if(!nd)
			nd = _next5();
		return nd;
	}

protected:
	auto_iterator<5, _IT, _TVALUE> _next5;
	iterators::parentheses<_IT, wrapper, _TVALUE> _parentheses;
};



template <class _IT, class _TVALUE>
struct auto_iterator<3, _IT, _TVALUE>
{
	auto_iterator(_IT& expr): _next4(expr), _string(expr) { }

	node<_TVALUE>* operator()()
	{
		node<_TVALUE>* nd = _string();
		if(!nd)
			nd = _next4();
		return nd;
	}

protected:
	auto_iterator<4, _IT, _TVALUE> _next4;
	iterators::string<_IT, '\\', wrapper, _TVALUE> _string;
};



template <class _IT, class _TVALUE>
struct auto_iterator<2, _IT, _TVALUE>
{
	auto_iterator(_IT& expr): _next3(expr), _number(expr) { }

	node<_TVALUE>* operator()()
	{
		node<_TVALUE>* nd = _number();
		if(!nd)
			nd = _next3();
		return nd;
	}

protected:
	auto_iterator<3, _IT, _TVALUE> _next3;
	iterators::number<_IT, wrapper, _TVALUE> _number;
};



template <class _IT, class _TVALUE>
struct auto_iterator<1, _IT, _TVALUE>
{
	auto_iterator(_IT& expr): _next2(expr), _alpha(expr) { }

	node<_TVALUE>* operator()()
	{
		node<_TVALUE>* nd = _alpha(); 
		if(!nd)
			nd = _next2();
		return nd;
	}

protected:
	auto_iterator<2, _IT, _TVALUE> _next2;
	iterators::alpha<_IT, wrapper, _TVALUE> _alpha;
};






template <class _TVALUE, class _IT = iterator, class _OPRTABLE = operation_table<_TVALUE>, class _SYMBOLS = symbol<_TVALUE> >
class algorithm
{
public:
	typedef typename _SYMBOLS _tdSymbols;
	typedef typename _OPRTABLE _tdOprTable;

public:
	algorithm(_SYMBOLS& symbols);
	~algorithm();

	void clear();
	void initialize();

	void parser(const char* expr);

	const tree<_TVALUE>* getTree() const;

protected:
	virtual void populate();
	virtual void mapUnknow();
	virtual void calc();

	void getValue(node<_TVALUE>* nd, _TVALUE& value, bool canCreateVariable = false) const;
	
protected:
	tree<_TVALUE>* _tree;
	_IT _expr;

	mutable _SYMBOLS&	_symbols;
};



}
}
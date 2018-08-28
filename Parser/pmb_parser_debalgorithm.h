#pragma once

#include "pmb_parser_algorithm.h"
#include "pmb_parser_calc_block.h"
#include "pmb_parser_calc_transporter.hpp"
#include "pmb_calculate_number.h"
#include "pmb_calculate_magnitude.h"
#include "pmb_calculate_symbol.h"


namespace pmb
{
namespace parser
{
namespace debug
{


struct iterator: parser::iterator<char, unsigned short>
{
	int _step0;
	idx _i0;
	int _subStep0;
	int _subStep1;

	iterator& operator= (const char* str)
	{
		_expr = str;
		_i0 = _i = 0;
		_step0 = -1;
		_subStep0 = 0;
		_subStep1 = 0;
		return *this;
	}
	
	inline operator bool() const
	{
		return _expr[_i] || _step0 < 6;
	}

	void incrStep()
	{
		++_step0;
		_subStep0 = 0;
	}
};


template <int i, class _IT, class _ITEM, typename _NDTYPE>
struct auto_iterator
{
	auto_iterator(_IT& expr): _it(expr), _next(expr), _space(expr) { }

	node<_ITEM, _NDTYPE>* operator()()
	{
		node<_ITEM, _NDTYPE>* nd = NULL;
		switch(_it._step0) {
		case -1:
			_next.initLoop();
			break;
		case 0:
			_it._i0 = _it._i;
			nd = _space();
			break;
		default:
			nd = _next();
			break;
		}
		return nd;
	}


	node<_ITEM, _NDTYPE>* release()
	{
		return _next.release();
	}

	bool space()
	{
		return _space.release();
	}


	const node<_ITEM, _NDTYPE>* getUnknow() const
	{
		return _next.getUnknow();
	}


	const char* getClassName() const
	{
		return _it._step0 == -1 ? "initLoop": _it._step0 == 0 ? "space": _next.getClassName();
	}

protected:
	_IT& _it;
	auto_iterator<1, _IT, _ITEM, _NDTYPE> _next;
	iterators::space<_IT, wrapper, _ITEM, _NDTYPE> _space;
};


template <class _IT, class _ITEM, typename _NDTYPE>
struct auto_iterator<6, _IT, _ITEM, _NDTYPE>
{
	auto_iterator(_IT& expr): _it(expr), _unknown(expr) { }

	node<_ITEM, _NDTYPE>* operator()()
	{
		node<_ITEM, _NDTYPE>* nd = _it._step0 == 6 ? _unknown() : NULL;
		if(6 < _it._step0)
			_it._step0 = -2;
		return nd;
	}


	void initLoop()
	{
		_unknown.initLoop();
	}


	node<_ITEM, _NDTYPE>* release()
	{
		return _unknown.release();
	}


	const char* getClassName() const
	{
		return _it._step0 == 6 ? "unknow": NULL;
	}

protected:
	_IT& _it;
	iterators::unknown<_IT, wrapper, _ITEM, _NDTYPE> _unknown;
};




template <class _IT, class _ITEM, typename _NDTYPE>
struct auto_iterator<5, _IT, _ITEM, _NDTYPE>
{
	auto_iterator(_IT& expr): _it(expr), _next(expr), _list(expr) { }

	node<_ITEM, _NDTYPE>* operator()()
	{
		return _it._step0 == 5 ? _list(): _next();
	}


	void initLoop()
	{
		_next.initLoop();
	}


	node<_ITEM, _NDTYPE>* release()
	{
		return _next.release();
	}


	const char* getClassName() const
	{
		return _it._step0 == 5 ? "list": _next.getClassName();
	}

protected:
	_IT& _it;
	auto_iterator<6, _IT, _ITEM, _NDTYPE> _next;
	iterators::list<_IT, wrapper, _ITEM, _NDTYPE> _list;
};




template <class _IT, class _ITEM, typename _NDTYPE>
struct auto_iterator<4, _IT, _ITEM, _NDTYPE>
{
	auto_iterator(_IT& expr): _it(expr), _next(expr), _parentheses(expr) { }

	node<_ITEM, _NDTYPE>* operator()()
	{
		return _it._step0 == 4 ? _parentheses(): _next();
	}


	void initLoop()
	{
		_next.initLoop();
	}


	node<_ITEM, _NDTYPE>* release()
	{
		return _next.release();
	}


	const char* getClassName() const
	{
		return _it._step0 == 4 ? "parentheses": _next.getClassName();
	}

protected:
	_IT& _it;
	auto_iterator<5, _IT, _ITEM, _NDTYPE> _next;
	iterators::parentheses<_IT, wrapper, _ITEM, _NDTYPE> _parentheses;
};



template <class _IT, class _ITEM, typename _NDTYPE>
struct auto_iterator<3, _IT, _ITEM, _NDTYPE>
{
	auto_iterator(_IT& expr): _it(expr), _next(expr), _string(expr) { }

	node<_ITEM, _NDTYPE>* operator()()
	{
		return _it._step0 == 3 ? _string(): _next();
	}


	void initLoop()
	{
		_next.initLoop();
	}


	node<_ITEM, _NDTYPE>* release()
	{
		return _next.release();
	}

	const char* getClassName() const
	{
		return _it._step0 == 3 ? "string": _next.getClassName();
	}

protected:
	_IT& _it;
	auto_iterator<4, _IT, _ITEM, _NDTYPE> _next;
	iterators::string<_IT, '\\', wrapper, _ITEM, _NDTYPE> _string;
};



template <class _IT, class _ITEM, typename _NDTYPE>
struct auto_iterator<2, _IT, _ITEM, _NDTYPE>
{
	auto_iterator(_IT& expr): _it(expr),  _next(expr), _number(expr) { }

	node<_ITEM, _NDTYPE>* operator()()
	{
		return _it._step0 == 2 ? _number(): _next();
	}


	void initLoop()
	{
		_next.initLoop();
	}


	node<_ITEM, _NDTYPE>* release()
	{
		return _next.release();
	}


	const char* getClassName() const
	{
		return _it._step0 == 2 ? "number": _next.getClassName();
	}

protected:
	_IT& _it;
	auto_iterator<3, _IT, _ITEM, _NDTYPE> _next;
	iterators::number<_IT, wrapper, _ITEM, _NDTYPE> _number;
};



template <class _IT, class _ITEM, typename _NDTYPE>
struct auto_iterator<1, _IT, _ITEM, _NDTYPE>
{
	auto_iterator(_IT& expr): _it(expr), _next(expr), _alpha(expr) { }

	node<_ITEM, _NDTYPE>* operator()()
	{
		return _it._step0 == 1 ? _alpha(): _next();
	}


	void initLoop()
	{
		_next.initLoop();
	}


	node<_ITEM, _NDTYPE>* release()
	{
		return _next.release();
	}


	const char* getClassName() const
	{
		return _it._step0 == 1 ? "alpha": _next.getClassName();
	}

protected:
	_IT& _it;
	auto_iterator<2, _IT, _ITEM, _NDTYPE> _next;
	iterators::alpha<_IT, wrapper, _ITEM, _NDTYPE> _alpha;
};


typedef pmb::calculate::units::magnitude<double, unsigned short, char, unsigned short> number_double;
//typedef calculate::number<double> number_double;
typedef item<char, typename iterator::idx> citem;
typedef tree<citem, ndtype> debug_tree;
typedef debug_tree::cnode tnode; 
typedef calc::transporter<number_double, unsigned short> transporter;
typedef calc::transporter_args<transporter, unsigned char> transporter_args;
typedef calc::build_in_function_table<calc::build_in_function<transporter_args, char, unsigned char>, unsigned short> build_in_function_table;
typedef calc::iterator<transporter_args, debug_tree> citerator;
//typedef pmb::parser::symbol<transporter, citem::string> symbol;
typedef pmb::calculate::units::symbol<char, unsigned char, transporter, citem::string> symbol;
typedef calc::block<citerator, build_in_function_table, symbol> block;

typedef parser::operation<transporter_args> operation;
typedef parser::operation_table<operation, tnode> operation_table;
// Class For Debug:

class algorithm : public parser::algorithm<block, operation_table, iterator>
{
public:
//	typedef transporter<number<double> > transporter;
//	typedef calc::block<calc::transporter<number<double>, unsigned short>, item<char, short>, ndtype> block;
	typedef typename block::cItem item;
	typedef parser::algorithm<block, operation_table, iterator> _base;
	typedef typename block::cNdType ndtype;
	typedef typename _base::tptree tptree;
	typedef typename _base::node node;

public:
	algorithm(operation_table* opr_table, block* pBlock);
	~algorithm();

	const auto_iterator<0, iterator, item, ndtype>* getIterators() const;
	const iterator* getIterator() const;
	const tptree* getTree() const;

	const node* getNewNode() const;
	const node* getNewNodeUnknow() const;

	bool nextStep();

protected:
	void populate();
	void mapUnknown();

	auto_iterator<0, iterator, item, ndtype> _ai;

	node* _newNode;
	node* _newNodeUnknown;
};



}
}
}
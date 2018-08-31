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
#include "pmb_parser_function.h"
#include "pmb_parser_symbol.h"


namespace pmb
{
namespace parser
{



template <int i, class _IT, class _ITEM, typename _NDTYPE>
struct auto_iterator
{
	auto_iterator(_IT& expr): _next1(expr), _space(expr), _unknown(expr) { }

	node<_ITEM, _NDTYPE>* operator()()
	{
		_unknown.initLoop();
		node<_ITEM, _NDTYPE>* nd = _space();
		if(!nd && !(nd = _next1()))
			nd = _unknown();
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

	bool space()
	{
		return _space.release();
	}


protected:
	auto_iterator<1, _IT, _ITEM, _NDTYPE> _next1;
	iterators::space<_IT, wrapper, _ITEM, _NDTYPE> _space;
	iterators::unknown<_IT, wrapper, _ITEM, _NDTYPE> _unknown;
};


template <class _IT, class _ITEM, typename _NDTYPE>
struct auto_iterator<5, _IT, _ITEM, _NDTYPE>
{
	auto_iterator(_IT& expr): _list(expr) { }

	node<_ITEM, _NDTYPE>* operator()()
	{
		return _list(); 
	}

protected:
	iterators::list<_IT, wrapper, _ITEM, _NDTYPE> _list;
};


template <class _IT, class _ITEM, typename _NDTYPE>
struct auto_iterator<4, _IT, _ITEM, _NDTYPE>
{
	auto_iterator(_IT& expr): _next5(expr), _parentheses(expr) { }

	node<_ITEM, _NDTYPE>* operator()()
	{
		node<_ITEM, _NDTYPE>* nd = _parentheses();
		if(!nd)
			nd = _next5();
		return nd;
	}

protected:
	auto_iterator<5, _IT, _ITEM, _NDTYPE> _next5;
	iterators::parentheses<_IT, wrapper, _ITEM, _NDTYPE> _parentheses;
};



template <class _IT, class _ITEM, typename _NDTYPE>
struct auto_iterator<3, _IT, _ITEM, _NDTYPE>
{
	auto_iterator(_IT& expr): _next4(expr), _string(expr) { }

	node<_ITEM, _NDTYPE>* operator()()
	{
		node<_ITEM, _NDTYPE>* nd = _string();
		if(!nd)
			nd = _next4();
		return nd;
	}

protected:
	auto_iterator<4, _IT, _ITEM, _NDTYPE> _next4;
	iterators::string<_IT, '\\', wrapper, _ITEM, _NDTYPE> _string;
};



template <class _IT, class _ITEM, typename _NDTYPE>
struct auto_iterator<2, _IT, _ITEM, _NDTYPE>
{
	auto_iterator(_IT& expr): _next3(expr), _number(expr) { }

	node<_ITEM, _NDTYPE>* operator()()
	{
		node<_ITEM, _NDTYPE>* nd = _number();
		if(!nd)
			nd = _next3();
		return nd;
	}

protected:
	auto_iterator<3, _IT, _ITEM, _NDTYPE> _next3;
	iterators::number<_IT, wrapper, _ITEM, _NDTYPE> _number;
};



template <class _IT, class _ITEM, typename _NDTYPE>
struct auto_iterator<1, _IT, _ITEM, _NDTYPE>
{
	auto_iterator(_IT& expr): _next2(expr), _alpha(expr) { }

	node<_ITEM, _NDTYPE>* operator()()
	{
		node<_ITEM, _NDTYPE>* nd = _alpha();
		if(!nd)
			nd = _next2();
		return nd;
	}

protected:
	auto_iterator<2, _IT, _ITEM, _NDTYPE> _next2;
	iterators::alpha<_IT, wrapper, _ITEM, _NDTYPE> _alpha;
};





//template <class _TVALUE, class _ITEM, typename _NDTYPE, class _BLOCK,
//	class _IT = iterator<typename _ITEM::CHAR, typename _ITEM::SIZETP>,
//	class _OPRTABLE = operation_table<_TVALUE, _ITEM, _NDTYPE>,
//	class _FNCTABLE = function_table<_TVALUE, _ITEM, _NDTYPE>,
//	class _SYMBOLS = symbol<_TVALUE> >
template <class _BLOCK,
		class _OPRTABLE,
		class _IT = iterator<typename _BLOCK::CHAR, typename _BLOCK::szcItem> >
class algorithm
{
public:
	typedef typename _BLOCK::istring sitem;
	///typedef typename _SYMBOLS _tdSymbols;
	typedef typename _OPRTABLE _tdOprTable;
	///typedef typename _FNCTABLE _tdFncTable;
	///typedef util::map<std::string, _TVALUE, sitem> argmap;
	typedef typename _BLOCK::tptree tptree;
	typedef typename _BLOCK::tnode node;
	typedef typename node::cItem cItem;
	typedef typename node::cNdType cNdType;
	typedef typename _tdOprTable::operation operation;
	typedef typename _BLOCK::build_in_function build_in_function;
	typedef typename _BLOCK::iterator user_def_function;
	typedef typename _BLOCK::symbol symbol;
	typedef typename _BLOCK::CHAR tpChar;

protected:
/*	struct stack
	{
		stack(algorithm* alg) {
			_node = alg->_tree->getRootNode();
			_functionDef = alg->_functionDef = alg->isFunctionDef(_node, alg->_fncName, alg->_fncArgs);
		}
		stack(node* nd) : _node(nd), _functionDef(false) {}

		node* _node;
		bool _functionDef;
	};

	typedef std::list<stack> lststack;

	typedef typename _BLOCK::map_args map_args;
	typedef typename _BLOCK::vector_args vector_args;*/

public:
	algorithm(_tdOprTable* operation_table, _BLOCK* pBlock);
	~algorithm();

	void clear();
	void initialize();

	bool parser(const typename tpChar* expr);
	bool calculate();
	bool calculate(const typename tpChar* expr);

	const tptree* getTree() const;
	const tpChar* expression() const;


protected:
	virtual void populate();
	virtual void mapUnknown();
///	virtual void calc();

///	virtual bool isFunctionDef(const node* nd, sitem& fname, argmap& args) const;

///	bool getValue(node* nd, _TVALUE& value, bool functionDef, bool canCreateVariable = false) const;


protected:
	_tdOprTable* _oprTable;

	_BLOCK* _pBlock;
	tptree* _tree;

	_IT _expr;
	bool _findFirstInFunction;

	sitem _fncName;
};









}
}
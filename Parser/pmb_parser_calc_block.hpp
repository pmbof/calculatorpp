#include "stdafx.h"
#pragma once

#include "pmb_parser_calc_block.h"
#include "pmb_parser_nodes_unknow.h"
#include "pmb_parser_nodes_parentheses.h"
#include "pmb_parser_exception.h"


namespace pmb
{
namespace parser
{
namespace calc
{














template<class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
inline block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::stack::stack(iterator* list)
	: _it_calc(list), _delete(false), _expr(nullptr)
{
}


template<class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
inline block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::stack::stack(tptree* tree)
	: _delete(true)
{
	_it_calc = new iterator(tree);
}


template<class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
inline block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::stack::~stack()
{
	if (_it_calc && _delete)
		delete _it_calc;
}



template<class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
inline void block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::stack::begin(_CPTRCHAR expr)
{
	_expr = expr;
	if (_it_calc->begin())
	{
		const nodes::unknown<cItem, cNdType>* uk = static_cast<const nodes::unknown<cItem, cNdType>*>(_it_calc->rootNode());
		if (uk && uk->getType() == ndUnknown)
		{
			// uk is =
			if (uk->isBinary() && uk->canCreateLVariable() && uk->getLeft() && uk->getLeft()->getType() == ndUnknown)
			{
				// uk is *    (operator call functions)
				uk = static_cast<const nodes::unknown<cItem, cNdType>*>(uk->getLeft());
				if (!uk->isBinary()
					|| !uk->getLeft() || uk->getLeft()->getType() != ndAlpha   // function name
					|| !uk->getRight() || uk->getRight()->getType() != ndParentheses // (  or  ()
					|| static_cast<const nodes::parentheses<cItem, cNdType>*>(uk->getRight())->getOpened() != 0  // ()
					&& static_cast<const nodes::parentheses<cItem, cNdType>*>(uk->getRight())->getOpened() != 1  // (
					|| !uk->getRight()->getRight() && static_cast<const nodes::parentheses<cItem, cNdType>*>(uk->getRight())->getOpened() == 1
					|| uk->getRight()->getRight() && static_cast<const nodes::parentheses<cItem, cNdType>*>(uk->getRight())->getOpened() == 0)
				{
					_fncName.clear();
					return; /// false
				}
				else
				{
					_margs.clear();
					_it_calc->parameters_clear();

					_fncName = uk->getLeft()->getString(_expr);
					if (!_fncName)
						return;
					_it_calc->function(uk->firstCalc()); // uk->getRight() is (
					uk = static_cast<const nodes::unknown<cItem, cNdType>*>(uk->getRight());
					const nodes::unknown<cItem, cNdType>* first = uk;
					// getting function's parameters
					for (uk = uk ? static_cast<const nodes::unknown<cItem, cNdType>*>(uk->getRight()) : nullptr;
							uk && (uk->getType() == ndList && uk->getLeft() && uk->getLeft()->getType() == ndList);
							uk = static_cast<const nodes::unknown<cItem, cNdType>*>(uk->getLeft()))
						;
					for (const tnode* lstparam = uk; lstparam;
							lstparam = lstparam->getRight() && lstparam->getRight()->getType() == ndParentheses ? lstparam->getRight() : 
							lstparam->getParent() == first && lstparam->getRight() ? lstparam->getRight()->getRight() : lstparam->getParent())
					{
						if ((lstparam->getType() != ndAlpha && lstparam->getType() != ndList && lstparam->getType() != ndParentheses)
								|| lstparam->getType() == ndParentheses && (static_cast<const nodes::parentheses<cItem, cNdType>*>(lstparam)->getOpened() != -1
								|| lstparam->getLeft() || lstparam->getRight()))
						{
							_it_calc->function(nullptr);
							_fncName.clear();
							_margs.clear();
							break;
						}
						else if (lstparam->getType() == ndParentheses)
							break;
						if (lstparam->getType() == ndList && lstparam->getLeft() && lstparam->getLeft()->getType() != ndList)
							_it_calc->parameters_add(_margs[lstparam->getLeft()->getString(_expr).getString()]);
						const tnode* param = lstparam->getType() == ndList ? lstparam->getRight() : lstparam;
						_it_calc->parameters_add(_margs[param->getString(_expr).getString()]);
						if (param->getRight() && param->getRight()->getType() == ndParentheses && !static_cast<const nodes::parentheses<cItem, cNdType>*>(param->getRight())->getOpened())
							break;
					}
				}
			}
		}
	}
}


template<class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
inline bool block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::stack::function_def() const
{
	return _fncName;
}

template<class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
inline bool block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::stack::function_call() const
{
	return _margs.empty() && !_vargs.empty();
}

template<class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
inline bool block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::stack::in_function_breaknode() const
{
	return _fncName && _it_calc->node() == _it_calc->function();
}


template<class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
inline typename block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::iterator*
	block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::stack::release()
{
	iterator* vret = _it_calc;
	if (_it_calc)
	{
		_it_calc->end();
		_it_calc = nullptr;
	}
	_margs.clear();
	return vret;
}


















template <class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
inline block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::bnode::bnode(block* root, symbol* variables)
	: _root(root), _parent(root->_ndActual), _child(nullptr), _next(nullptr), _variables(variables), _internal(false)
{
}


template <class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
inline block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::bnode::bnode(bnode* parent)
	: _root(parent->_root), _parent(parent), _child(nullptr), _next(nullptr), _internal(true)
{
	_variables = new symbol;
	_root->_variables->addSetVariable("");
}


template<class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
inline block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::bnode::~bnode()
{
	if (_internal && _variables)
		delete _variables;

	for (mapfnc::const_iterator f = _functions.begin(); f != _functions.end(); ++f)
	{
		for (map_nargs_iterator::const_iterator fn = f->second.begin(); fn != f->second.end(); ++fn)
		{
			delete fn->second->tree();
			delete fn->second;
		}
	}

	for (lststack::iterator s = _stack.begin(); s != _stack.end(); ++s)
		delete *s;
}


























/*template <class _CITERATOR, class _BIN_FNCTABLE>
inline block<_CITERATOR, _BIN_FNCTABLE>::block(const _BIN_FNCTABLE* bin_fnc_table)
	: _fncTable(bin_fnc_table), _ndActual(nullptr), _lastResult(nullptr)
{
	_root = new bnode(this);
}*/



template<class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
inline block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::block(const _BIN_FNCTABLE* bin_fnc_table, symbol* variables)
	: _fncTable(bin_fnc_table), _ndActual(nullptr), _lastResult(nullptr)
{
	_root = new bnode(this, variables);
}


template <class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
inline block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::~block()
{
	if (_lastResult)
		delete _lastResult;

	for (bnode* nd = _root; nd; )
	{
		if (nd->_child)
			nd = nd->_child;
		else
		{
			bnode* next = nd->_next ? nd->_next : nd->_parent;
			delete nd;
			nd = next;
		}
	}
}



template<class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
inline void	block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::init(tptree* tree)
{
	_ndActual = _root;
	_ndActual->_stack.push_back(new stack(tree));
}

template<class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
inline typename block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::iterator*
	block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::begin(_CPTRCHAR expr)
{
	stack* stck_actual = _ndActual->_stack.back();
	stck_actual->begin(expr);
	return stck_actual->_it_calc;
}


template <class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
inline typename block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::iterator*
	block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::call_function(iterator* function, transporter_args& args)
{
	stack* fncstack = new stack(function);
	_ndActual->_stack.push_back(fncstack);
	function->parameters_set(args);
	return function;
}




template<class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
inline block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::operator bool() const
{
	return _ndActual && _ndActual->_stack.size();
}



template<class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
inline bool block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::calculate()
{
	stack* stck = _ndActual->_stack.back();
	return false;
}



template<class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
inline void block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::next()
{
	if (_ndActual)
	{
		if (_lastResult)
			delete _lastResult;
		_lastResult = _ndActual->_stack.back()->release();
		delete _ndActual->_stack.back();
		_ndActual->_stack.pop_back();
		if (_ndActual->_stack.size())
		{
			stack* stck = _ndActual->_stack.back();
			if (_lastResult && _lastResult->function())
			{
				stck->_it_calc->result() = _lastResult->right();
				_lastResult = nullptr;
			}
			stck->_it_calc->setBegined();
			return;
		}
		if (_ndActual->_child)
			_ndActual = _ndActual->_child;
		else if (_ndActual->_next)
			_ndActual = _ndActual->_next;
		else
		{// falta pensarlo mejor:
			for (bnode* nd = _ndActual->_parent; nd; )
			{
				if (nd->_child == _ndActual)
					nd = nd->_next;
				else
					nd = nd->_parent;
			}
		}
	}
}

template<class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
inline typename const block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::transporter&
	block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::tresult() const
{
	if (!_lastResult || _lastResult->getValues().result().isNull())
		throw exception<cItem>("No result found");
	return _lastResult->getValues().result();
}



template<class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
inline typename const block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::tpvalue& 
	block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::result() const
{
	if (!_lastResult || _lastResult->getValues().result().isNull())
		throw exception<cItem>("No result found");
	return **(_lastResult->getValues().result());
}

template<class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
inline typename const block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::tpvalue::_TypeValue&
	block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::nresult() const
{
	if (!_lastResult || _lastResult->getValues().result().isNull())
		throw exception<cItem>("No result found");
	return _lastResult->getValues().result()->_number;
}


template<class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
inline typename const block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::tnode* 
	block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::rootNode() const
{
	return _ndActual && !_ndActual->_stack.empty() ? _ndActual->_stack.back()->_it_calc->rootNode() : _lastResult ? _lastResult->rootNode() : nullptr;
}


template<class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
inline typename block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::stack*
	block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::actualStack()
{
	return _ndActual->_stack.back();
}


template<class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
inline bool block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::is_functionDef() const
{
	return _ndActual->_stack.back()->function_def();
}

template<class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
inline bool block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::insert_function()
{
	stack* a_stack = actualStack();
	bool bRet = a_stack->in_function_breaknode();
	if (bRet)
		insert_function(a_stack->_fncName, a_stack->release());
	return bRet;
}



template <class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
void block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::insert_function(const istring& fncName, iterator* clist)
{
	_ndActual->_functions[fncName.getString()][clist->parameters_amount()] = clist;
}




template <class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
inline typename block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::pair_function
	block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::find_function(const tnode* nd, typename _CPTRCHAR expr, bool toRight, bool firstFindBuildInFunctions) const
{
	const tnode* chFnc = toRight ? nd->getLeft() : nd->getRight();
	if (chFnc && chFnc->getType() == ndAlpha)
	{
		nargs nArgs = nd->nArguments(toRight);

		if (firstFindBuildInFunctions)
		{
			const typename build_in_function* bin_fnc = _fncTable->find(chFnc->getCharPtr(expr), chFnc->len(), nArgs);
			if (bin_fnc)
				return pair_function(bin_fnc);
		}

		istring isfnc = chFnc->getString(expr);

		for (bnode* cursor = _ndActual; cursor; cursor = cursor->_parent)
		{
			mapfnc::const_iterator cif = cursor->_functions.find(isfnc);
			if (cif != cursor->_functions.end())
			{
				map_nargs_iterator::const_iterator ciff = cif->second.find(nArgs);
				if (ciff != cif->second.end())
					return pair_function(ciff->second, ciff->second->parameters_amount());
			}
		}

		if (!firstFindBuildInFunctions)
		{
			const typename build_in_function* bin_fnc = _fncTable->find(chFnc->getCharPtr(expr), chFnc->len(), nArgs);
			if (bin_fnc)
				return pair_function(bin_fnc);
		}
	}
	return pair_function();
}



template<class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
inline typename block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::transporter_args& 
	block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::getValues()
{
	stack* st = _ndActual->_stack.back();
	typename transporter_args& values = st->_it_calc->getValues();
	if (!st->_it_calc->calculated() && !st->_it_calc->calculating())
	{
		unsigned short nArgs;
		const tnode* childs[2];
		typename transporter* vals[2];
		bool bForAlpha[2];
		bool bCanCreateVariable[2];
		const nodes::unknown<cItem, cNdType>* uk = static_cast<const nodes::unknown<cItem, cNdType>*>(st->_it_calc->node());
		if (uk->getType() == ndUnknown && uk->isValid())
		{
			nArgs = uk->isBinary() ? 2 : 1;
			childs[0] = uk->isBinary() && uk->isFirstLeft() || !uk->isBinary() && uk->getLeft() ? uk->getLeft() : uk->getRight();
			vals[0] = uk->isBinary() && uk->isFirstLeft() || !uk->isBinary() ? &values.left() : &values.right();
			bForAlpha[0] = !uk->isCallFunction() || !uk->isFirstLeft();
			bCanCreateVariable[0] = uk->isFirstLeft() && uk->canCreateLVariable() || uk->isFirstRight() && uk->canCreateRVariable();
			if (1 < nArgs)
			{
				childs[1] = uk->isFirstLeft() ? uk->getRight() : uk->getLeft();
				vals[1] = uk->isFirstLeft() && !uk->isCallFunction() ? &values.right() : &values.left();
				bForAlpha[1] = !uk->isCallFunction() || uk->isFirstLeft();
				bCanCreateVariable[1] = uk->isFirstLeft() && uk->canCreateRVariable() || uk->isFirstRight() && uk->canCreateLVariable();
			}
		}
		else if (uk->getType() == ndList)
		{
			nArgs = 2;
			childs[0] = uk->getLeft();
			vals[0] = &values.left();
			bForAlpha[0] = true;
			bCanCreateVariable[0] = false;
			childs[1] = uk->getRight();
			vals[1] = &values.right();
			bForAlpha[1] = true;
			bCanCreateVariable[1] = false;
		}
		else if (uk->getType() == ndParentheses)
		{
			nArgs = 1;
			childs[0] = uk->getRight();
			vals[0] = &values.right();
			bForAlpha[0] = true;
			bCanCreateVariable[0] = false;
		}
		else
			throw exception<cItem>(uk, "can not get values for %item");

		for (unsigned short arg = 0; arg < nArgs; ++arg)
		{
			if (!childs[arg])
				continue;
			if (childs[arg]->getType() == ndAlpha)
			{
				if (bForAlpha[arg])
				{
					if (!find_variable(childs[arg]->getString(st->_expr), *vals[arg], bCanCreateVariable[arg]))
						throw exception<cItem>(childs[arg], "undefined symbol %item");
				}
			}
			else if (childs[arg]->getType() == ndNumber)
			{
				const istring& snumber = childs[arg]->getString(st->_expr);
				*vals[arg] = new transporter::tpvalue(snumber, childs[arg]->len());
			}
			else if (!st->_it_calc->is_varDependent() && st->_it_calc->lastNode() != childs[arg] && st->_it_calc->lastJoinNode() != childs[arg])
			{

				if (st->_it_calc->prev() && st->_it_calc->prev()->_node == childs[arg])
				{
					if (childs[arg] == uk->getLeft())
						st->_it_calc->prev()->_transporter.join_l2r(values, true, true);
					else
						st->_it_calc->prev()->_transporter.join_r2l(values, true, true);
				}
				else
				{
					pmb::log::instance()->trace(logError, "node = 0x%08X\nchilds[%d] = 0x%08X\nlast node = 0x%08X\nlast join node = 0x%08X\nprev node = 0x%08X\n",
						uk, arg, childs[arg], st->_it_calc->lastNode(), st->_it_calc->lastJoinNode(), st->_it_calc->prev() ? st->_it_calc->prev()->_node : nullptr);
					throw exception<cItem>(uk, childs[arg] == uk->getLeft() && !uk->isCallFunction() ? "can not get lvalue for %item" : uk->isCallFunction() ? "can get arguments for call function" : "can not get rvalue for %item");
				}
			}
		}
	}
	return values;
}





template <class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
bool block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::find_variable(const istring& symbol, transporter& value, bool canCreate)
{
	stack* st = _ndActual->_stack.back();
	if (st->function_def())
	{
		map_args::iterator arg = st->_margs.find(symbol);
		if (arg != st->_margs.end())
		{
			value = arg->second;
			st->_it_calc->varDependent(true);
			return true;
		}
	}

	for (bnode* nd = _ndActual; nd; nd = nd->_parent)
	{
		if (nd->_variables->find(symbol, value, nd == _ndActual && canCreate))
			return true;
	}
	return false;
}



template<class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
inline typename block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::symbol*
	block<_CITERATOR, _BIN_FNCTABLE, _SYMBOL>::variables()
{
	return _root->_variables;
}



}
}
}

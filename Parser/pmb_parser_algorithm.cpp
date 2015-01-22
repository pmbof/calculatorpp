#include "StdAfx.h"
#include "pmb_parser_algorithm.h"
#include "pmb_parser_iterators_space.h"
#include "pmb_parser_iterators_alpha.h"
#include "pmb_parser_iterators_number.h"
#include "pmb_parser_iterators_string.h"
#include "pmb_parser_iterators_parentheses.h"

#include "pmb_parser_iterator.cpp"
#include "pmb_parser_iterators_space.cpp"
#include "pmb_parser_iterators_alpha.cpp"
#include "pmb_parser_iterators_number.cpp"
#include "pmb_parser_iterators_string.cpp"
#include "pmb_parser_iterators_parentheses.cpp"
#include "pmb_parser_iterators_unknow.cpp"

#include "pmb_parser_nodes_unknow.h"
#include "pmb_parser_nodes_unknow.cpp"

#include "pmb_parser_function.cpp"



namespace pmb
{
namespace parser
{



template <class _TVALUE, class _IT, class _OPRTABLE, class _FNCTABLE, class _SYMBOLS>
algorithm<_TVALUE, _IT, _OPRTABLE, _FNCTABLE, _SYMBOLS>::algorithm(_SYMBOLS& symbols)
	: _tree(NULL), _symbols(symbols)
{
}

template <class _TVALUE, class _IT, class _OPRTABLE, class _FNCTABLE, class _SYMBOLS>
algorithm<_TVALUE, _IT, _OPRTABLE, _FNCTABLE, _SYMBOLS>::~algorithm()
{
	clear();
}

template <class _TVALUE, class _IT, class _OPRTABLE, class _FNCTABLE, class _SYMBOLS>
void algorithm<_TVALUE, _IT, _OPRTABLE, _FNCTABLE, _SYMBOLS>::clear()
{
	if(_tree)
	{
		delete _tree;
		_tree = NULL;
	}
}


template <class _TVALUE, class _IT, class _OPRTABLE, class _FNCTABLE, class _SYMBOLS>
void algorithm<_TVALUE, _IT, _OPRTABLE, _FNCTABLE, _SYMBOLS>::initialize()
{
	clear();
}



template <class _TVALUE, class _IT, class _OPRTABLE, class _FNCTABLE, class _SYMBOLS>
void algorithm<_TVALUE, _IT, _OPRTABLE, _FNCTABLE, _SYMBOLS>::parser(const char* expr)
{
	AfxTrace(L"Paresing expression: \n\t\'%s\'\n", (LPCTSTR)CString(expr));
	if(!expr)
		return;
	if(_tree)
		delete _tree;
	_expr = expr;
	_tree = new tree<_TVALUE>;

	populate();
	mapUnknow();
	calc();
}



template <class _TVALUE, class _IT, class _OPRTABLE, class _FNCTABLE, class _SYMBOLS>
void algorithm<_TVALUE, _IT, _OPRTABLE, _FNCTABLE, _SYMBOLS>::populate()
{
	auto_iterator<0, _IT, _TVALUE> ai(_expr);
	while(_expr())
	{
		node<_TVALUE>* newNode = ai();
		if(newNode || ai.space())
		{
			node<_TVALUE>* ndUnknow = ai.release();
			_tree->insert(ndUnknow);
			_tree->insert(newNode);
		}
	}
	ai.initLoop();
	node<_TVALUE>* ndUnknow = ai.release();
	_tree->insert(ndUnknow);
}


template <class _TVALUE, class _IT, class _OPRTABLE, class _FNCTABLE, class _SYMBOLS>
void algorithm<_TVALUE, _IT, _OPRTABLE, _FNCTABLE, _SYMBOLS>::mapUnknow()
{
	node<_TVALUE>* nd;
	_OPRTABLE oprTable;
	_FNCTABLE fncTable;
	for(nd = _tree->getRootNode()->getFirstUnknowNode(); nd; nd = nd->getNextUnknowNode())
	{
		const operation<_TVALUE>* opr = oprTable.find(nd, _expr._expr);
		if(opr)
		{
			const function<_TVALUE>* fnc = NULL;
			if(opr->canCallFunction())
			{
				if(_findFirstInFunction)
					fnc = fncTable.find(nd, _expr._expr, opr->isLeftToRight());
				else
				{
					const node<_TVALUE>* child = nd->getChild(opr->isLeftToRight());
					if(child && child->getType() == ndAlpha && !_symbols.find(child->getString(_expr)))
						fnc = fncTable.find(nd, _expr._expr, opr->isLeftToRight());
				}
			}
			if(fnc)
				AfxTrace(L"\t\t\t+ Function found: %s, nArgs = %d (%s). _findFirstInFunction = %s\n", (LPCTSTR)CString(fnc->getName()), fnc->getNArgs(), (LPCTSTR)CString(fnc->getDescription()), _findFirstInFunction ? L"true": L"false");

			static_cast<nodes::unknow<_TVALUE>*>(nd)->setOperation(opr, fnc);
			CString trace(L"\t- Mapping operation: ");
			trace += opr->getSymbol();
			trace += "<";
			trace += opr->getName();
			trace += ", ";
			trace += opr->getDescription();
			trace += ">[";
			trace += opr->isBinary() ? "True, ": "FALSE, ";
			trace += opr->isLeftToRight() ? "True": "FALSE";
			AfxTrace(trace);
			TRACE_NODE("], node: ", nd);
		}
		else
			TRACE_NODE("\t- No mapping for: ", nd);
	}
}


template <class _TVALUE, class _IT, class _OPRTABLE, class _FNCTABLE, class _SYMBOLS>
void algorithm<_TVALUE, _IT, _OPRTABLE, _FNCTABLE, _SYMBOLS>::calc()
{
	node<_TVALUE>* nd = _tree->getRootNode();
	_TVALUE lValue(true);
	_TVALUE rValue(true);
	for (; nd && nd->isCalcType();)
	{
		nodes::calc<_TVALUE>* uc = static_cast<nodes::calc<_TVALUE>*>(nd);
		uc = uc->nextCalc();
		if(uc)
		{
			TRACE_NODE("\t\t+ Calculating", uc);
			if(uc->getType() == ndUnknow)
			{
				nodes::unknow<_TVALUE>* uk = static_cast<nodes::unknow<_TVALUE>*>(uc);
				const operation<_TVALUE>* opr = uk->getOperation();
				const function<_TVALUE>* fnc = uk->getFunction();
				if(opr->isBinary())
				{
					if((!fnc || !opr->isLeftToRight()))// && (!opr->canCallFunction() || !opr->isLeftToRight() || uk->nArguments(true) < 2))
					{
						AfxTrace(L"\t\t\t* getting lValue: (right nArguments = %d)%s\n", uk->nArguments(true), opr->canCreateLVariable() ? L"[create Variable]": L"");
						getValue(uk->getLeft(), lValue, opr->canCreateLVariable());
					}
					else
						AfxTrace(L"\t\t\t- skipping get lValue: is a call function, right nArguments = %d.\n", uk->nArguments(true));
					if((!fnc ||  opr->isLeftToRight()))// && (!opr->canCallFunction() || opr->isLeftToRight() || uk->nArguments(false) < 2))
					{
						AfxTrace(L"\t\t\t* getting rValue: (left nArguments = %d)%s\n", uk->nArguments(false), opr->canCreateRVariable() ? L"[create Variable]": L"");
						getValue(uk->getRight(), rValue, opr->canCreateRVariable());
					}
					else
						AfxTrace(L"\t\t\t- skipping get rValue: is a call function, left nArguments = %d.\n", uk->nArguments(false));
					if(!lValue)
						TRACE_NODE("\t\t\t- not lValue for", uk->getLeft());
					if(!rValue)
						TRACE_NODE("\t\t\t- not rValue for", uk->getRight());
					if((!lValue && !opr->canCreateLVariable() || !rValue && !opr->canCreateRVariable())
						 && (!fnc || !rValue && opr->isLeftToRight() || !lValue && !opr->isLeftToRight()))
							break;
					if(fnc)
					{
						AfxTrace(L"\t\t\t+ Function found: %s, nArgs = %d (%s). _findFirstInFunction = %s\n", (LPCTSTR)CString(fnc->getName()), fnc->getNArgs(), (LPCTSTR)CString(fnc->getDescription()), _findFirstInFunction ? L"true": L"false");
						_TVALUE* args = uk->getArguments();
						(*fnc)(uk->getValue(), fnc->getNArgs(), args);
						getValue(uk->getRight(), uk->getValue(), opr->canCreateRVariable());
					}
					else
					{
						AfxTrace(L"\t\t\t* getting result:\n");
						(*opr)(uk->getValue(), lValue, rValue);
					}
					lValue.release();
					rValue.release();
				}
				else // opr is unitary
				{
					AfxTrace(L"\t\t\t* getting value:\n");
					getValue(opr->isLeftToRight() ? uk->getLeft(): uk->getRight(), lValue);
					if(!lValue)
					{
						TRACE_NODE("\t\t\t- not value for", opr->isLeftToRight() ? uk->getLeft(): uk->getRight());
						break;
					}
					AfxTrace(L"\t\t\t* getting result:\n");
					(*opr)(uk->getValue(), lValue);
					lValue.release();
				}
			}
			else if(uc->getType() == ndParentheses)
			{
				nodes::parentheses<_TVALUE>* pr = static_cast<nodes::parentheses<_TVALUE>*>(uc);
				if(pr->getRight()->getType() != ndList && pr->getRight()->getType() != ndParentheses)
				{
					AfxTrace(L"\t\t\t* getting value:\n");
					getValue(pr->getRight(), pr->getValue());
				}
				else
					AfxTrace(L"\t\t\t* skipping value\n");
				if(!pr->getValue())
				{
					TRACE_NODE("\t\t\t- not value for", pr->getRight());
//					break;
				}
			}
			else // uc->getType() == ndList
			{
				nodes::list<_TVALUE>* ls = static_cast<nodes::list<_TVALUE>*>(uc);
				if(ls->getLeft()->getType() != ndList && ls->getLeft()->getType() != ndParentheses)
				{
					AfxTrace(L"\t\t\t* getting lValue:\n");
					getValue(ls->getLeft(), ls->getLValue());
				}
				else
					AfxTrace(L"\t\t\t* skipping lValue:\n");
				if(ls->getRight()->getType() != ndList && ls->getRight()->getType() != ndParentheses)
				{
					AfxTrace(L"\t\t\t* getting rValue:\n");
					getValue(ls->getRight(), ls->getRValue());
				}
				else
					AfxTrace(L"\t\t\t* skipping rValue\n");
					
				if(!ls->getLValue())
					TRACE_NODE("\t\t\t- not lValue for", ls->getLeft());
				if(!ls->getRValue())
					TRACE_NODE("\t\t\t- not rValue for", ls->getRight());
//				if(!ls->getLValue() || !ls->getRValue())
//					break;
			}
			uc->setCalculated();
			TRACE_NODE("\t\t- Calculated!!!", uc);
		}
		nd = uc;
	}
}



template <class _TVALUE, class _IT, class _OPRTABLE, class _FNCTABLE, class _SYMBOLS>
void algorithm<_TVALUE, _IT, _OPRTABLE, _FNCTABLE, _SYMBOLS>::getValue(node<_TVALUE>* nd, _TVALUE& value, bool canCreateVariable) const
{
	switch(nd->getType())
	{
	case ndNumber:
		value = _TVALUE::getNewValue(nd->getCharPtr(_expr), nd->len());
		break;
	case ndAlpha:
		_symbols.find(nd->getString(_expr), value, canCreateVariable);
		break;
	case ndParentheses:
		value = static_cast<nodes::parentheses<_TVALUE>*>(nd)->getValue();
		break;
	case ndList:
		value = static_cast<nodes::list<_TVALUE>*>(nd)->getRValue();
		break;
	case ndString:
		break;
	case ndUnknow:
		value = static_cast<nodes::unknow<_TVALUE>*>(nd)->getValue();
		break;
	default:
		break;
	}
}



template <class _TVALUE, class _IT, class _OPRTABLE, class _FNCTABLE, class _SYMBOLS>
const tree<_TVALUE>* algorithm<_TVALUE, _IT, _OPRTABLE, _FNCTABLE, _SYMBOLS>::getTree() const
{
	return _tree;
}



}
}
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



template <class _BLOCK, class _OPRTABLE, class _IT>
algorithm<_BLOCK, _OPRTABLE, _IT>::algorithm(_tdOprTable* operation_table, _BLOCK* pBlock)
	: _oprTable(operation_table), _pBlock(pBlock), _tree(nullptr)
{
}

template <class _BLOCK, class _OPRTABLE, class _IT>
algorithm<_BLOCK, _OPRTABLE, _IT>::~algorithm()
{
	clear();
}

template <class _BLOCK, class _OPRTABLE, class _IT>
void algorithm<_BLOCK, _OPRTABLE, _IT>::clear()
{
	if(_tree)
	{
		delete _tree;
		_tree = nullptr;
	}
}


template <class _BLOCK, class _OPRTABLE, class _IT>
void algorithm<_BLOCK, _OPRTABLE, _IT>::initialize()
{
	clear();
}



template <class _BLOCK, class _OPRTABLE, class _IT>
void algorithm<_BLOCK, _OPRTABLE, _IT>::parser(const char* expr)
{
	log* pLg = log::beginFunction(pmb::logDebug, "pmb::parser::algorithm::parser");
	if (!expr)
	{
		pLg->trace(logError, "expression is NULL").endFunction(logError);
		return;
	}

	if(_tree)
		delete _tree;
	_expr = expr;
	_tree = new tptree;

	pLg->trace(logDebug, "Populating: '%s'\n", expr).trace(logDebug, "");
	pLg->traceN(         "             ", strlen(expr), "\n");
	populate();
	_tree->trace(_expr);

	pLg->trace(logDebug, "Mapping: '%s'\n", expr).trace(logDebug, "");
	pLg->traceN(         "          ", strlen(expr), "\n");
	mapUnknown();
	_tree->trace(_expr);

	pLg->trace(logDebug, "Calculating: '%s'\n", expr).trace(logDebug, "");
	pLg->traceN(         "              ", strlen(expr), "\n");
	calculate();
	_tree->trace(_expr);

	pLg->endFunction(logDebug);
}



template <class _BLOCK, class _OPRTABLE, class _IT>
void algorithm<_BLOCK, _OPRTABLE, _IT>::populate()
{
	auto_iterator<0, _IT, cItem, cNdType> ai(_expr);
	while(_expr())
	{
		node* newNode = ai();
		if(newNode || ai.space())
		{
			node* ndUnknown = ai.release();
			_tree->insert(ndUnknown);
			_tree->insert(newNode);
		}
	}
	ai.initLoop();
	node* ndUnknown = ai.release();
	_tree->insert(ndUnknown);
	_tree->check();
}


template <class _BLOCK, class _OPRTABLE, class _IT>
void algorithm<_BLOCK, _OPRTABLE, _IT>::mapUnknown()
{
	typedef nodes::unknown<cItem, cNdType> node_unknown;

	log* plg = log::beginFunction(logDebug, "pmb::parser::algorithm::mapUnknown");
	node* nd;
	for(nd = _tree->getRootNode()->getFirstUnknownNode(); nd; nd = nd->getNextUnknownNode())
	{
		const operation* opr = _oprTable->find(nd, _expr._expr);
		if(opr)
		{
			const tptree* fncTree = NULL;
			typename _BLOCK::pair_function fnc;
			if(opr->canCallFunction())
				fnc = _pBlock->find_function(nd, _expr._expr, opr->isLeftToRight(), _findFirstInFunction);
			if (fnc._userDef)
				plg->trace(logDebug, "\t\t\t+ user function found. _findFirstInFunction = %s\n", _findFirstInFunction ? "true" : "false");
			if (fnc._buildin)
				plg->trace(logDebug, "\t\t\t+ Function found: %s, nArgs = %d (%s). _findFirstInFunction = %s\n", fnc._buildin->getName(), fnc._buildin->getNArgs(), fnc._buildin->getDescription(), _findFirstInFunction ? "true": "false");

			static_cast<node_unknown*>(nd)->set(
				reinterpret_cast<const void*>(opr), opr->isBinary(), opr->isLeftToRight(), opr->getPrecedence(), opr->canCreateLVariable(), opr->canCreateRVariable(),
				reinterpret_cast<const void*>(fnc._buildin),
				reinterpret_cast<void*>(fnc._userDef));
			plg->trace(logDebug, "\t- Mapping operation: ");
			if (fnc._userDef)
				*plg << "(call user function) ";
			if (fnc._buildin)
				*plg << "(call build-in function) ";
			*plg << "'" << opr->getSymbol() << "'"
				<< " <" << opr->getName() << ", " << opr->getDescription() << ">["
				<< (opr->isBinary() ? "b": "u") << "," << (opr->isLeftToRight() ? "L": "R") << "," << opr->getPrecedence() << "]";
			TRACE_NODE(logDebug, ",", _expr, nd);
		}
		else
			TRACE_NODE(logWarning, "\t- No mapping for: ", _expr, nd);
	}
	plg->endFunction();
}





template<class _BLOCK, class _IT, class _OPRTABLE>
inline bool algorithm<_BLOCK, _IT, _OPRTABLE>::calculate()
{
	typedef nodes::calc<cItem, cNdType> node_calc;
	typedef nodes::unknown<cItem, cNdType> node_unknown;
	log* plg = log::beginFunction(logDebug, "pmb::parser::algorithm::calculate");
	_BLOCK& block = *_pBlock;

	for (block.init(_tree); block; block.next())
	{
		typename _BLOCK::iterator* it_calc = block.begin(_expr);

		for ( ; *it_calc; ++(*it_calc))
		{
			const typename node_calc* uc = static_cast<const typename node_calc*>(it_calc->node());
			if (uc && uc->isCalcType())
			{
				if (block.insert_function())
				{
					_tree->trace(_expr);
					_tree = NULL;
					plg->trace(logDebug, "\t\t\t. End calculate. Function definition!\n");
					break;
				}
				TRACE_NODE(logDebug, "\t\t+ Calculating:", _expr, uc);

				if (uc->getType() == ndUnknown)
				{
					const nodes::unknown<cItem, cNdType>* uk = static_cast<const nodes::unknown<cItem, cNdType>*>(uc);
					if (uk->isValid())
					{
						transporter_args& args = block.getValues();
						if (!it_calc->is_varDependent() || !block.is_functionDef())
						{
							if (uk->isCallFunction())
							{
								if (uk->isCallBuildInFunction())
								{
									try
									{
										const build_in_function* function = reinterpret_cast<const build_in_function*>(uk->pointer());
										(*function)(args);
										args.removeRights();
										it_calc->calculated(true);
									}
									catch (build_in_function::exception& ex)
									{
										throw exception<cItem>(uc, ex.message());
									}
								}
								else
								{
									try
									{
										user_def_function* function = const_cast<user_def_function*>(reinterpret_cast<const user_def_function*>(uk->pointer()));
										it_calc = block.call_function(function, args);
										continue;
									}
									catch (exception<cItem>& ex)
									{
										throw exception<cItem>(uc, ex.message(_expr).c_str());
									}
								}
							}
							else
							{
								try
								{
									const operation* opr = reinterpret_cast<const operation*>(uk->pointer());
									(*opr)(args);
									it_calc->calculated(true);
								}
								catch (operation::exception& ex)
								{
									throw exception<cItem>(uc, ex.message());
								}
							}
						}
					}
					else
						throw exception<cItem>(uc, "Unknown, no matching symbol %item");
				}
				else if (uc->getType() == ndList)
				{
					block.getValues();
					it_calc->calculated(true);
				}
				else if (uc->getType() == ndParentheses)
				{
					transporter_args& args = block.getValues();
					if (uc->getRight() && uc->getRight()->getType() != ndList)
						args.removeLefts();
					it_calc->calculated(true);
				}
				else
					throw exception<cItem>(uc, "unknown symbol %item");
				TRACE_NODE(logDebug, "\t\t- Calculated finale status:", _expr, uc);
			}
		}
	}
	plg->endFunction(logDebug);
	return false;
}




template <class _BLOCK, class _OPRTABLE, class _IT>
const typename algorithm<_BLOCK, _OPRTABLE, _IT>::tptree*
	algorithm<_BLOCK, _OPRTABLE, _IT>::getTree() const
{
	return _tree;
}



}
}
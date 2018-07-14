#include "StdAfx.h"
#pragma once
#include "pmb_parser_debalgorithm.h"
#include "pmb_parser_algorithm.cpp"
#include "pmb_parser_operation.cpp"


namespace pmb
{
namespace parser
{
namespace debug
{


algorithm::algorithm(operation_table* opr_table, block* pBlock)
	: _base(opr_table, pBlock), _ai(_expr), _newNode(NULL), _newNodeUnknown(NULL)
{
}


algorithm::~algorithm()
{
}


const auto_iterator<0, iterator, algorithm::item, ndtype>* algorithm::getIterators() const
{
	return &_ai;
}

const iterator* algorithm::getIterator() const
{
	return &_expr;
}

const typename algorithm::tptree* algorithm::getTree() const
{
	return _tree;
}


void algorithm::populate()
{
}


const typename algorithm::node* algorithm::getNewNode() const
{
	return _newNode;
}


const typename algorithm::node* algorithm::getNewNodeUnknow() const
{
	return _newNodeUnknown;
}



bool algorithm::nextStep()
{
	if(_expr)
	{
		if(!_newNode)
			_newNode = _ai();
		else
		{
			_ai.space();
			_newNodeUnknown = _ai.release();
			if(_newNodeUnknown)
				_tree->insert(_newNodeUnknown);
			_tree->insert(_newNode);
			_newNode = NULL;
		}
		if(!_newNode)
			_expr.incrStep();
		else
			_expr._subStep0 = 1;
	}
	else
	{
		_newNodeUnknown = _ai.release();
		if(_newNodeUnknown)
			_tree->insert(_newNodeUnknown);
	}
	return _expr;
}


void algorithm::mapUnknown()
{
}


}
}
}
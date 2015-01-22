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


algorithm::algorithm(_base::_tdSymbols& symbols)
	: _base(symbols), _ai(_expr), _newNode(NULL), _newNodeUnknow(NULL)
{
}


algorithm::~algorithm()
{
}


const auto_iterator<0, iterator, algorithm::transporter>* algorithm::getIterators() const
{
	return &_ai;
}

const iterator* algorithm::getIterator() const
{
	return &_expr;
}

const tree<algorithm::transporter>* algorithm::getTree() const
{
	return _tree;
}


void algorithm::populate()
{
}


const node<algorithm::transporter>* algorithm::getNewNode() const
{
	return _newNode;
}


const node<algorithm::transporter>* algorithm::getNewNodeUnknow() const
{
	return _newNodeUnknow;
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
			_newNodeUnknow = _ai.release();
			if(_newNodeUnknow)
				_tree->insert(_newNodeUnknow);
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
		_newNodeUnknow = _ai.release();
		if(_newNodeUnknow)
			_tree->insert(_newNodeUnknow);
	}
	return _expr;
}


void algorithm::mapUnknow()
{
}


}
}
}
#include "StdAfx.h"
#include "pmb_parser_node.h"
#include "pmb_parser_nodes_parentheses.h"
#include "pmb_parser_nodes_unknow.h"
#include "pmb_parser_nodes_unknow.cpp"
#include "pmb_parser_nodes_list.h"
#include "pmb_parser_nodes_list.cpp"


namespace pmb
{
namespace parser
{



template <class _TVALUE>
node<_TVALUE>::node(ndtype type, int ini, int end)
	: item(ini, end), _parent(NULL), _left(NULL), _right(NULL), _type(type)
{
	TRACE_NODE("New", this, true);
}


template <class _TVALUE>
node<_TVALUE>::~node()
{
}

template <class _TVALUE>
node<_TVALUE>* node<_TVALUE>::newNodeSpace(int ini, int end)
{
	return new node<_TVALUE>(ndSpace, ini, end);
}

template <class _TVALUE>
node<_TVALUE>* node<_TVALUE>::newNodeAlpha(int ini, int end)
{
	return new node<_TVALUE>(ndAlpha, ini, end);
}

template <class _TVALUE>
node<_TVALUE>* node<_TVALUE>::newNodeNumber(int ini, int end)
{
	return new node<_TVALUE>(ndNumber, ini, end);
}

template <class _TVALUE>
node<_TVALUE>* node<_TVALUE>::newNodeString(int ini, int end)
{
	return new node<_TVALUE>(ndString, ini, end);
}

template <class _TVALUE>
node<_TVALUE>* node<_TVALUE>::newNodeParentheses(int ini, int end, char type, int opened)
{
	return new nodes::parentheses<_TVALUE>(ini, end, type, opened);
}

template <class _TVALUE>
node<_TVALUE>* node<_TVALUE>::newNodeList(int end)
{
	return new nodes::list<_TVALUE>(end);
}

template <class _TVALUE>
node<_TVALUE>* node<_TVALUE>::newNodeUnknow(int ini, int end)
{
	return new nodes::unknow<_TVALUE>(ini, end);
}

template <class _TVALUE>
node<_TVALUE>* node<_TVALUE>::newNodeUnknowEmpty(const node<_TVALUE>* parent, const node<_TVALUE>* newNode)
{
	return 	node<_TVALUE>::newNodeUnknow(1 < newNode->_ini - parent->_end ? newNode->_ini - 1: parent->_end, newNode->_ini);
}


template <class _TVALUE>
const node<_TVALUE>* node<_TVALUE>::getRootNode() const
{
	const node<_TVALUE>* rt;
	for(rt = this; rt && rt->_parent; rt = rt->_parent)
		;
	return rt;
}


template <class _TVALUE>
node<_TVALUE>* node<_TVALUE>::getRootNode()
{
	node<_TVALUE>* rt;
	for(rt = this; rt && rt->_parent; rt = rt->_parent)
		;
	return rt;
}


template <class _TVALUE>
const node<_TVALUE>* node<_TVALUE>::getFirstLeftNode() const
{
	const node<_TVALUE>* lf;
	for(lf = this; lf && lf->_left; lf = lf->_left)
		;
	return lf;
}


template <class _TVALUE>
node<_TVALUE>* node<_TVALUE>::getFirstLeftNode()
{
	node<_TVALUE>* lf;
	for(lf = this; lf && lf->_left; lf = lf->_left)
		;
	return lf;
}


template <class _TVALUE>
const node<_TVALUE>* node<_TVALUE>::getFirstUnknowNode() const
{
	const node<_TVALUE>* nd = getFirstNode();
	if(nd && nd->_type != ndUnknow)
		nd = nd->getNextUnknowNode();
	return nd;
}


template <class _TVALUE>
node<_TVALUE>* node<_TVALUE>::getFirstUnknowNode()
{
	node<_TVALUE>* nd = getFirstNode();
	if(nd && nd->_type != ndUnknow)
		nd = nd->getNextUnknowNode();
	return nd;
}



template <class _TVALUE>
const node<_TVALUE>* node<_TVALUE>::getNextUnknowNode() const
{
	const node<_TVALUE>* ret;
	for(ret = getNextNode(); ret && ret->_type != ndUnknow; ret = ret->getNextNode())
		;
	return ret;
}


template <class _TVALUE>
node<_TVALUE>* node<_TVALUE>::getNextUnknowNode()
{
	node<_TVALUE>* ret;
	for(ret = getNextNode(); ret && ret->_type != ndUnknow; ret = ret->getNextNode())
		;
	return ret;
}


template <class _TVALUE>
const node<_TVALUE>* node<_TVALUE>::getFirstNode() const
{
	return getRootNode()->getFirstLeftNode();
}


template <class _TVALUE>
node<_TVALUE>* node<_TVALUE>::getFirstNode()
{
	return getRootNode()->getFirstLeftNode();
}


template <class _TVALUE>
const node<_TVALUE>* node<_TVALUE>::getNextNode() const
{
	const node<_TVALUE>* ret = this && _right ? _right->getFirstLeftNode():
		this && _parent && _parent->_left == this ? _parent: NULL;
	if(!ret && this && _parent)
	{
		ret = _parent;
		for(const node<_TVALUE>* right = this; ret && ret->_right == right; ret = ret->_parent)
			right = ret;
	}
	return ret;
}


template <class _TVALUE>
node<_TVALUE>* node<_TVALUE>::getNextNode()
{
	node<_TVALUE>* ret = this && _right ? _right->getFirstLeftNode():
		this && _parent && _parent->_left == this ? _parent: NULL;
	if(!ret && this && _parent)
	{
		ret = _parent;
		for(node<_TVALUE>* right = this; ret && ret->_right == right; ret = ret->_parent)
			right = ret;
	}
	return ret;
}



template <class _TVALUE>
const node<_TVALUE>* node<_TVALUE>::getParent() const
{
	return _parent;
}

template <class _TVALUE>
const node<_TVALUE>* node<_TVALUE>::getLeft() const
{
	return _left;
}

template <class _TVALUE>
node<_TVALUE>* node<_TVALUE>::getLeft()
{
	return _left;
}

template <class _TVALUE>
const node<_TVALUE>* node<_TVALUE>::getRight() const
{
	return _right;
}

template <class _TVALUE>
node<_TVALUE>* node<_TVALUE>::getRight()
{
	return _right;
}


template <class _TVALUE>
char node<_TVALUE>::getType() const
{
	return _type;
}


template <class _TVALUE>
bool node<_TVALUE>::isCalcType() const
{
	return _type == ndUnknow || _type == ndParentheses || _type == ndList;
}


template <class _TVALUE>
void node<_TVALUE>::insertToRight(node<_TVALUE>* newNode)
{
	AfxTrace(L"node::insertToRight\n");

	node<_TVALUE>* ndR;
	for(ndR = this; ndR->_right; ndR = ndR->_right)
		;
	ndR->_right = newNode;
	ndR->_right->_parent = this;
}


template <class _TVALUE>
void node<_TVALUE>::insertToLeft(node<_TVALUE>* newNode)
{
	AfxTrace(L"node::insertToLeft\n");

	node<_TVALUE>* ndL;
	for(ndL = this; ndL->_left; ndL = ndL->_left)
		;
	ndL->_left = newNode;
	ndL->_left->_parent = this;
}


template <class _TVALUE>
void node<_TVALUE>::insertInThisRight(node<_TVALUE>* newNode)
{
	AfxTrace(L"node::insertInThisRight\n");

	node<_TVALUE>* nd;
	for(nd = this; nd->_right; nd = nd->_right)
		;
	node<_TVALUE>* p = nd->_parent;
	nd->_parent = newNode;
	newNode->_left = nd;
	newNode->_parent = p;
	if(p)
	{
		if(p->_right == nd)
			p->_right = newNode;
		else // p->_left == nd
			p->_left = newNode;
	}
}



template <class _TVALUE>
void node<_TVALUE>::insertInThisNode(node<_TVALUE>* newNode)
{
	AfxTrace(L"node::insertInThisNode\n");

	node<_TVALUE>* tmpParent = _parent;
	_parent = newNode;
	newNode->_left = this;
	newNode->_parent = tmpParent;

	if(tmpParent)
	{
		if(tmpParent->_right == this)
			tmpParent->_right = newNode;
		else // tmpParent->_left == nd
			tmpParent->_left = newNode;
	}
}



template <class _TVALUE>
node<_TVALUE>* node<_TVALUE>::insertEmptyUnknow(const node<_TVALUE>* newNode)
{
	AfxTrace(L"node::insertEmptyUnknow\n");

	node<_TVALUE>* emptyOperator = node<_TVALUE>::newNodeUnknowEmpty(this, newNode);
	insertInThisNode(emptyOperator);
	return emptyOperator;
}


template <class _TVALUE>
node<_TVALUE>* node<_TVALUE>::deleteThisNode()
{
	AfxTrace(L"node::deleteThisNode ANDA MAL!!!\n");
	ASSERT(!_right);

	if(_parent)
	{
		if(_parent->_right == this)
			_parent->_right = _left;
		else
			_parent->_left = _left;
	}
	if(_left)
		_left->_parent = _parent;

	node<_TVALUE>* parent = _parent ? _parent: _left;
	delete this;
	return parent;
}


template <class _TVALUE>
void node<_TVALUE>::replaceThisNode(node<_TVALUE>* newNode)
{
	AfxTrace(L"node::replaceThisNode\n");

	if(newNode->_parent = _parent)
	{
		if(_parent->_left == this)
			_parent->_left = newNode;
		else
			_parent->_right = newNode;
	}
	if(newNode->_left = _left)
		newNode->_left->_parent = newNode;
	if(newNode->_right = _right)
		newNode->_right->_parent = newNode;

	TRACE_NODE("deleting for replace",  this);
	delete this;
}


template <class _TVALUE>
void node<_TVALUE>::upLeftToThisNode()
{
	AfxTrace(L"node::upLeftToThisNode\n");

	if(_left->_parent = _parent)
	{
		if(_parent->_left == this)
			_parent->_left = _left;
		else
			_parent->_right = _left;
	}
	TRACE_NODE("deleting for up left to this",  this);
	if(_right)
		TRACE_NODE("Memory leak in up left to this", _right);
	delete(this);
}


template <class _TVALUE>
void node<_TVALUE>::switchToUpNode()
{
	AfxTrace(L"node::switchToUpNode\n");
	
	if(_parent)
	{
		if(_parent->_parent)
		{
			if(_parent->_parent->_left == _parent)
				_parent->_parent->_left = this;
			else
			{
				ASSERT(_parent->_parent->_right == _parent);
				_parent->_parent->_right = this;
			}
		}
		node<_TVALUE>* oldParent = _parent;
		_parent = _parent->_parent;
		oldParent->_parent = this;

		if(oldParent->_right == this)
		{
			node<_TVALUE>* oldLeft = _left;
			_left = oldParent;
			if(oldLeft)
				oldLeft->_parent = oldParent;
			oldParent->_right = oldLeft;
		}
		else // oldParent->_left == this
		{
			ASSERT(oldParent->_left == this);
			node<_TVALUE>* oldRight = _right;
			_right = oldParent;
			if(oldRight)
				oldRight->_parent = oldParent;
			oldParent->_left = oldRight;
		}
	}
}


template <class _TVALUE>
node<_TVALUE>* node<_TVALUE>::insert(node<_TVALUE>* newNode)
{
	node<_TVALUE>* ndRet;
	switch(newNode->_type)
	{
	case ndSpace:
		ndRet = insert_space(newNode);
		break;
	case ndAlpha:
		ndRet = insert_alpha(newNode);
		break;
	case ndNumber:
		ndRet = insert_number(newNode);
		break;
	case ndString:
		ndRet = insert_string(newNode);
		break;
	case ndParentheses:
		ndRet = insert_parentheses(newNode);
		break;
	case ndList:
		ndRet = insert_list(newNode);
		break;
	case ndUnknow:
		ndRet = insert_unknow(newNode);
		break;
	default:
		ndRet = NULL;
		break;
	}
	return ndRet;
}


template <class _TVALUE>
node<_TVALUE>* node<_TVALUE>::insert_alpha(node<_TVALUE>* newNode)
{
	node<_TVALUE>* ndRet = NULL;
	switch(_type)
	{
	case ndAlpha:
	case ndNumber:
	case ndString:
		insertEmptyUnknow(newNode)->insertToRight(ndRet = newNode);
		break;
	case ndParentheses:
		if(static_cast<nodes::parentheses<_TVALUE>*>(this)->getOpened() < 0)
		{
			node<_TVALUE>* emptyUnknow = insertUnknowListInParentheses(node<_TVALUE>::newNodeUnknowEmpty(this, newNode));
			if(emptyUnknow)
				emptyUnknow->insertToRight(ndRet = newNode);
		}
		else
			insertToRight(ndRet = newNode);
		break;
	case ndUnknow:
	case ndList:
		insertToRight(ndRet = newNode);
		break;
	case ndSpace:
		if(_left)
		{
			_type = ndUnknow;
			insertToRight(ndRet = newNode);
		}
		else
		{
			node<_TVALUE>* parent = deleteThisNode();
			if(parent)
				parent->insertToRight(ndRet = newNode);
			else
				ndRet = newNode;
		}
		break;
	default:
		TRACE_NODE("Memory leak in node::insert_alpha", newNode);
		break;
	}
	return ndRet;
}


template <class _TVALUE>
node<_TVALUE>* node<_TVALUE>::insert_number(node<_TVALUE>* newNode)
{
	return insert_alpha(newNode);
}


template <class _TVALUE>
node<_TVALUE>* node<_TVALUE>::insert_string(node<_TVALUE>* newNode)
{
	return insert_alpha(newNode);
}


template <class _TVALUE>
node<_TVALUE>* node<_TVALUE>::insert_space(node<_TVALUE>* newNode)
{
	node<_TVALUE>* ndRet = NULL;
	switch(_type)
	{
	case ndAlpha:
	case ndNumber:
	case ndString:
		insertInThisNode(ndRet = newNode);
		break;
	case ndParentheses:
		insertToRight(ndRet = newNode);
		break;
	case ndUnknow:
	case ndList:
		TRACE_NODE("deleting space for List", newNode);
		delete newNode;
		ndRet = this;
		break;
	default:
		TRACE_NODE("Memory leak in node::insert_alpha", newNode);
		break;
	}
	return ndRet;
}


template <class _TVALUE>
node<_TVALUE>* node<_TVALUE>::insert_parentheses(node<_TVALUE>* newNode)
{
	switch(_type)
	{
	case ndParentheses:
		{
			nodes::parentheses<_TVALUE>* thisPa = static_cast<nodes::parentheses<_TVALUE>*>(this),
							  * newPa  = static_cast<nodes::parentheses<_TVALUE>*>(newNode);
			if(thisPa->getOpened() <= 0 && 0 <= newPa->getOpened())
			{
				node<_TVALUE>* unknow = node<_TVALUE>::newNodeUnknowEmpty(thisPa, newPa);
				unknow = insert_unknow(unknow);
				unknow->insertToRight(newNode);
			}
			else
				insertToRight(newNode);
		}
		break;
	case ndAlpha:
	case ndNumber:
	case ndString:
		if((_type != ndParentheses || static_cast<nodes::parentheses<_TVALUE>*>(this)->getOpened() < 0) 
					&& 0 <= static_cast<nodes::parentheses<_TVALUE>*>(newNode)->getOpened())
			insertEmptyUnknow(newNode)->insertToRight(newNode);
		else
			insertToRight(newNode);
		break;
	case ndSpace:
		{
			_type = ndUnknow;
			insertToRight(newNode);
//			node<_TVALUE>* parent = deleteThisNode();
//			if(parent)
//				parent->insertToRight(newNode);
		}
		break;
	default:
		insertToRight(newNode);
		break;
	}
	return newNode;
}


template <class _TVALUE>
void node<_TVALUE>::insertWithLowPriority(node<_TVALUE>* newNode)
{
	node<_TVALUE>* top;
	for(top = this; top && top->_parent && 
			(top->_parent->_type == ndUnknow || top->_parent->_type == ndAlpha || top->_parent->_type == ndNumber 
				|| top->_parent->_type == ndString || top->_parent->_type == ndSpace); top = top->_parent)
		;
	if(top)
		top->insertInThisNode(newNode);
}


template <class _TVALUE>
node<_TVALUE>* node<_TVALUE>::insert_list(node<_TVALUE>* newNode)
{
	node<_TVALUE>* ndRet = NULL;
	switch(_type)
	{
	case ndSpace:
		{
			node<_TVALUE>* parent = deleteThisNode();
			if(parent)
				parent->insertWithLowPriority(ndRet = newNode);
			else
				ndRet = newNode;
		}
		break;
	case ndAlpha:
	case ndNumber:
	case ndString:
	case ndUnknow:
		insertWithLowPriority(ndRet = newNode);
		break;
	case ndList:
	case ndParentheses:
		ndRet = insertUnknowListInParentheses(newNode);
		break;
	default:
		TRACE_NODE("Memory leak in node::insert_alpha", newNode);
		break;
	}
	return ndRet;
}



template <class _TVALUE>
node<_TVALUE>* node<_TVALUE>::insert_unknow(node<_TVALUE>* newNode)
{
	node<_TVALUE>* ndRet;
	switch(_type)
	{
	case ndSpace:
		replaceThisNode(ndRet = newNode);
		break;
	case ndAlpha:
	case ndNumber:
	case ndString:
		insertInThisRight(ndRet = newNode);
		break;
	case ndList:
	case ndUnknow:
		insertToRight(ndRet = newNode);
		break;
	case ndParentheses:
		ndRet = insertUnknowListInParentheses(newNode);
		break;
	default:
		TRACE_NODE("Memory leak in node::insert_unknow", newNode);
		break;
	}
	return ndRet;
}



template <class _TVALUE>
node<_TVALUE>* node<_TVALUE>::insertUnknowListInParentheses(node<_TVALUE>* newUnknow)
{
	if(_type != ndParentheses || (newUnknow->_type != ndUnknow && newUnknow->_type != ndList))
		return NULL;

	if(0 <= static_cast<nodes::parentheses<_TVALUE>*>(this)->getOpened())
		insertToRight(newUnknow);
	else
	{
		node<_TVALUE>* rpar = foundOpenParentheses();
		if(!rpar)
		{
			TRACE_NODE("Error: parentheses close very much. Memory leak", newUnknow);
			return NULL;
		}
		if(newUnknow->_type == ndList)
			rpar->insertWithLowPriority(newUnknow);
		else
			rpar->insertInThisNode(newUnknow);
	}
	return newUnknow;
}


template <class _TVALUE>
node<_TVALUE>* node<_TVALUE>::foundOpenParentheses()
{
	if(_type != ndParentheses)
		return NULL;
	nodes::parentheses<_TVALUE>* parentheses = static_cast<nodes::parentheses<_TVALUE>*>(this);
	int count = parentheses->getOpened();
	node<_TVALUE>* nd;
	for(nd = _parent; nd && count < 0; nd = count < 0 ? nd->_parent: nd)
		if(parentheses->isThisType(nd))
			count += static_cast<nodes::parentheses<_TVALUE>*>(nd)->getOpened();
	return count < 0 ? NULL: nd;
}



template <class _TVALUE>
int node<_TVALUE>::getMetricsNodes(int& size) const
{
	int vRet = 0;
	if(this)
	{
		vRet = _left->getMetricsNodes(size) + 1 + _right->getMetricsNodes(size);
		size += getSizeofNode(_type);
	}
	return vRet;
}


template <class _TVALUE>
int node<_TVALUE>::getMetricsNodes(ndtype type, int& size) const
{
	int vRet = 0;
	if(this)
		vRet = _left->getMetricsNodes(type, size) + (_type == type ? 1: 0) + _right->getMetricsNodes(type, size);
	size = getSizeofNode(type);
	return vRet;
}


template <class _TVALUE>
int node<_TVALUE>::getSizeofNode(ndtype type)
{
	return type == ndList ? sizeof(nodes::calc<_TVALUE>): 
		type == ndParentheses ? sizeof(nodes::parentheses<_TVALUE>):
		type == ndUnknow ? sizeof(nodes::unknow<_TVALUE>): sizeof(node<_TVALUE>);
}



}
}
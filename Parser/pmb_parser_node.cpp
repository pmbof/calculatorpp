#include "StdAfx.h"
#include "pmb_parser_node.h"
#include "pmb_parser_nodes_number.h"
#include "pmb_parser_nodes_number.cpp"
#include "pmb_parser_nodes_parentheses.h"
#include "pmb_parser_nodes_parentheses.cpp"
#include "pmb_parser_nodes_unknow.h"
#include "pmb_parser_nodes_unknow.cpp"
#include "pmb_parser_nodes_list.h"
#include "pmb_parser_nodes_list.cpp"

#include "pmb_parser_exception.h"



namespace pmb
{
namespace parser
{



template <class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>::node(cNdType type, ISIZE ini, ISIZE end)
	: item(ini, end), _parent(nullptr), _left(nullptr), _right(nullptr), _type(type)
{
	TRACE_NODE(logDebug, "New", nullptr, this, true);
}


template <class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>::~node()
{
}

template <class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::newNodeSpace(ISIZE ini, ISIZE end)
{
	return new cnode(ndSpace, ini, end);
}

template <class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::newNodeAlpha(ISIZE ini, ISIZE end)
{
	return new cnode(ndAlpha, ini, end);
}

template <class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::newNodeNumber(ISIZE ini, ISIZE end)
{
	return new nodes::number<_ITEM, _NDTYPE>(ini, end);
}

template <class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::newNodeString(ISIZE ini, ISIZE end)
{
	return new cnode(ndString, ini, end);
}

template <class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::newNodeParentheses(ISIZE ini, ISIZE end, char type, int opened)
{
	return new nodes::parentheses<_ITEM, _NDTYPE>(ini, end, type, opened);
}

template <class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::newNodeList(ISIZE end)
{
	return new nodes::list<_ITEM, _NDTYPE>(end);
}

template <class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::newNodeUnknown(ISIZE ini, ISIZE end)
{
	return new nodes::unknown<_ITEM, _NDTYPE>(ini, end);
}

template <class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::newNodeUnknownEmpty(const cnode* parent, const cnode* newNode)
{
	return 	cnode::newNodeUnknown(1 < newNode->_ini - parent->_end ? newNode->_ini - 1: parent->_end, newNode->_ini);
}


template <class _ITEM, class _NDTYPE>
const node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::getRootNode() const
{
	const cnode* rt;
	for(rt = this; rt && rt->_parent; rt = rt->_parent)
		;
	return rt;
}


template <class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::getRootNode()
{
	cnode* rt;
	for(rt = this; rt && rt->_parent; rt = rt->_parent)
		rt->check();
	return rt;
}


template <class _ITEM, class _NDTYPE>
const node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::getFirstLeftNode() const
{
	const cnode* lf;
	for(lf = this; lf && lf->_left; lf = lf->_left)
		lf->check();
	return lf;
}


template <class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::getFirstLeftNode()
{
	cnode* lf;
	for(lf = this; lf && lf->_left; lf = lf->_left)
		lf->check();
	return lf;
}


template <class _ITEM, class _NDTYPE>
const node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::getFirstUnknownNode() const
{
	const cnode* nd = getFirstNode();
	if(nd && nd->_type != ndUnknown)
		nd = nd->getNextUnknownNode();
	return nd;
}


template <class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::getFirstUnknownNode()
{
	cnode* nd = getFirstNode();
	if(nd && nd->_type != ndUnknown)
		nd = nd->getNextUnknownNode();
	return nd;
}



template <class _ITEM, class _NDTYPE>
const node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::getNextUnknownNode() const
{
	const cnode* ret;
	for(ret = getNextNode(); ret && ret->_type != ndUnknown; ret = ret->getNextNode())
		;
	return ret;
}


template <class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::getNextUnknownNode()
{
	cnode* ret;
	for(ret = getNextNode(); ret && ret->_type != ndUnknown; ret = ret->getNextNode())
		;
	return ret;
}




template <class _ITEM, class _NDTYPE>
const node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::getFirstParenthesisNode() const
{
	const cnode* nd = getFirstNode();
	if (nd && nd->_type != ndParentheses)
		nd = nd->getNextParenthesisNode();
	return nd;
}


template <class _ITEM, class _NDTYPE>
const node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::getNextParenthesisNode() const
{
	const cnode* ret;
	for (ret = getNextNode(); ret && ret->_type != ndParentheses; ret = ret->getNextNode())
		;
	return ret;
}





template <class _ITEM, class _NDTYPE>
const node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::getFirstNode() const
{
	return getRootNode()->getFirstLeftNode();
}


template <class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::getFirstNode()
{
	return getRootNode()->getFirstLeftNode();
}


template <class _ITEM, class _NDTYPE>
const node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::getNextNode() const
{
	const cnode* ret = this && _right ? _right->getFirstLeftNode():
		this && _parent && _parent->_left == this ? _parent: nullptr;
	if(!ret && this && _parent)
	{
		ret = _parent;
		for(const cnode* right = this; ret && ret->_right == right; ret = ret->_parent)
			right = ret;
	}
	return ret;
}


template <class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::getNextNode()
{
	cnode* ret = this && _right ? _right->getFirstLeftNode() :
		this && _parent && _parent->_left == this ? _parent : nullptr;
	if (!ret && this && _parent)
	{
		ret = _parent;
		for (cnode* right = this; ret && ret->_right == right; ret = ret->_parent)
			right = ret;
	}
	return ret;
}



template <class _ITEM, class _NDTYPE>
int node<_ITEM, _NDTYPE>::nArguments(bool toRight) const
{
	const cnode* ret = toRight && _right ? _right : !toRight && _left ? _left : nullptr;
	return ret ? ret->_type != ndParentheses ? 1 : reinterpret_cast<const nodes::parentheses<_ITEM, _NDTYPE>*>(ret)->countListChildNodes() : 0;
}



template <class _ITEM, class _NDTYPE>
const node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::getParent() const
{
	return _parent;
}

template <class _ITEM, class _NDTYPE>
const node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::getLeft() const
{
	return _left;
}

template <class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::getLeft()
{
	return _left;
}

template <class _ITEM, class _NDTYPE>
const node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::getChild(bool bLeft) const
{
	return bLeft ? _left: _right;
}

template <class _ITEM, class _NDTYPE>
const node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::getRight() const
{
	return _right;
}

template <class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::getRight()
{
	return _right;
}


template <class _ITEM, class _NDTYPE>
char node<_ITEM, _NDTYPE>::getType() const
{
	return _type;
}


template <class _ITEM, class _NDTYPE>
bool node<_ITEM, _NDTYPE>::isCalcType() const
{
	return _type == ndUnknown || _type == ndParentheses || _type == ndList;
}


template<class _ITEM, typename _NDTYPE>
void node<_ITEM, _NDTYPE>::insertInRight(cnode* newnode)
{
	node* right = _right;
	_right = newnode;
	newnode->_parent = this;
	newnode->_right = right;
	if (right)
		right->_parent = newnode;
}


template <class _ITEM, class _NDTYPE>
void node<_ITEM, _NDTYPE>::insertToRight(cnode* newNode)
{
	pmb::log* pLg = pmb::log::beginFunction(logDebug, "pmb::parser::node::insertToRight");

	cnode* ndR;
	for(ndR = this; ndR->_right; ndR = ndR->_right)
		;
	ndR->_right = newNode;
	newNode->_parent = ndR;

	pLg->endFunction();
}


template <class _ITEM, class _NDTYPE>
void node<_ITEM, _NDTYPE>::insertToLeft(node<_ITEM, _NDTYPE>* newNode)
{
	pmb::log* pLg = pmb::log::beginFunction(logDebug, "pmb::parser::node::insertToLeft");

	cnode* ndL;
	for(ndL = this; ndL->_left; ndL = ndL->_left)
		;
	ndL->_left = newNode;
	ndL->_left->_parent = this;

	pLg->endFunction();
}


template <class _ITEM, class _NDTYPE>
void node<_ITEM, _NDTYPE>::insertInThisRight(node<_ITEM, _NDTYPE>* newNode)
{
	pmb::log* pLg = pmb::log::beginFunction(logDebug, "pmb::parser::node::insertInThisRight");

	node<_ITEM, _NDTYPE>* nd;
	for(nd = this; nd->_right; nd = nd->_right)
		;
	node<_ITEM, _NDTYPE>* p = nd->_parent;
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

	pLg->endFunction();
}



template <class _ITEM, class _NDTYPE>
void node<_ITEM, _NDTYPE>::insertInThisNode(node<_ITEM, _NDTYPE>* newNode)
{
	pmb::log* pLg = pmb::log::beginFunction(logDebug, "pmb::parser::node::insertInThisNode");

	cnode* tmpParent = _parent;
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

	pLg->endFunction();
}



template <class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::insertEmptyUnknown(const node<_ITEM, _NDTYPE>* newNode)
{
	pmb::log* pLg = pmb::log::beginFunction(logDebug, "pmb::parser::node::insertEmptyUnknown");

	node<_ITEM, _NDTYPE>* emptyOperator = node<_ITEM, _NDTYPE>::newNodeUnknownEmpty(this, newNode);
	insertInThisNode(emptyOperator);

	pLg->endFunction();
	return emptyOperator;
}


template <class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::deleteThisNode()
{
	pmb::log* pLg = pmb::log::beginFunction(logDebug, "pmb::parser::node::deleteThisNode");
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

	node<_ITEM, _NDTYPE>* parent = _parent ? _parent : _left;
	delete this;
	pLg->endFunction();
	return parent;
}


template <class _ITEM, class _NDTYPE>
void node<_ITEM, _NDTYPE>::replaceThisNode(node<_ITEM, _NDTYPE>* newNode)
{
	pmb::log* pLg = pmb::log::beginFunction(logDebug, "pmb::parser::node::replaceThisNode");

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

	TRACE_NODE(logDebug, "deleting for replace", nullptr,  this);
	delete this;
	pLg->endFunction();
}


template <class _ITEM, class _NDTYPE>
void node<_ITEM, _NDTYPE>::upLeftToThisNode()
{
	pmb::log* pLg = pmb::log::beginFunction(logDebug, "pmb::parser::node::upLeftToThisNode");

	if(_left->_parent = _parent)
	{
		if(_parent->_left == this)
			_parent->_left = _left;
		else
			_parent->_right = _left;
	}
	TRACE_NODE(logDebug, "deleting for up left to this",  this);
	if(_right)
		TRACE_NODE(logError, "Memory leak in up left to this", _right);
	delete(this);
	pLg->endFunction();
}


template <class _ITEM, class _NDTYPE>
void node<_ITEM, _NDTYPE>::switchToUpNode()
{
	pmb::log* pLg = pmb::log::beginFunction(pmb::logDebug, "pmb::parser::node::switchToUpNode");
	
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
		node<_ITEM, _NDTYPE>* oldParent = _parent;
		_parent = _parent->_parent;
		oldParent->_parent = this;

		if(oldParent->_right == this)
		{
			node<_ITEM, _NDTYPE>* oldLeft = _left;
			_left = oldParent;
			if(oldLeft)
				oldLeft->_parent = oldParent;
			oldParent->_right = oldLeft;
		}
		else // oldParent->_left == this
		{
			ASSERT(oldParent->_left == this);
			node<_ITEM, _NDTYPE>* oldRight = _right;
			_right = oldParent;
			if(oldRight)
				oldRight->_parent = oldParent;
			oldParent->_left = oldRight;
		}
	}
	pLg->endFunction(pmb::logDebug);
}


template <class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::insert(node<_ITEM, _NDTYPE>* newNode)
{
	cnode* ndRet;
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
	case ndUnknown:
		ndRet = insert_unknown(newNode);
		break;
	default:
		ndRet = nullptr;
		break;
	}
	return ndRet;
}


template <class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::insert_alpha(node<_ITEM, _NDTYPE>* newNode)
{
	cnode* ndRet = nullptr;
	switch(_type)
	{
	case ndAlpha:
	case ndNumber:
	case ndString:
		insertEmptyUnknown(newNode)->insertToRight(ndRet = newNode);
		break;
	case ndParentheses:
		if (static_cast<nodes::parentheses<_ITEM, _NDTYPE>*>(this)->getOpened() < 0)
		{
			node<_ITEM, _NDTYPE>* emptyUnknown = insertUnknownListInParentheses(node<_ITEM, _NDTYPE>::newNodeUnknownEmpty(this, newNode));
			if(emptyUnknown)
				emptyUnknown->insertToRight(ndRet = newNode);
		}
		else
			insertToRight(ndRet = newNode);
		break;
	case ndUnknown:
	case ndList:
		insertToRight(ndRet = newNode);
		break;
	case ndSpace:
		if(_left)
		{
			_type = ndUnknown;
			insertToRight(ndRet = newNode);
		}
		else
		{
			node<_ITEM, _NDTYPE>* parent = deleteThisNode();
			if(parent)
				parent->insertToRight(ndRet = newNode);
			else
				ndRet = newNode;
		}
		break;
	default:
		TRACE_NODE(logError, "Memory leak in node::insert_alpha", nullptr, newNode);
		break;
	}
	return ndRet;
}


template <class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::insert_number(node<_ITEM, _NDTYPE>* newNode)
{
	return insert_alpha(newNode);
}


template <class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::insert_string(node<_ITEM, _NDTYPE>* newNode)
{
	return insert_alpha(newNode);
}


template <class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::insert_space(node<_ITEM, _NDTYPE>* newNode)
{
	node<_ITEM, _NDTYPE>* ndRet = nullptr;
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
	case ndUnknown:
	case ndList:
		TRACE_NODE(logDebug, "deleting space for List", nullptr, newNode);
		delete newNode;
		ndRet = this;
		break;
	default:
		TRACE_NODE(logError, "Memory leak in node::insert_alpha", nullptr, newNode);
		break;
	}
	return ndRet;
}


template <class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::insert_parentheses(node<_ITEM, _NDTYPE>* newNode)
{
	short newOpened = static_cast<nodes::parentheses<_ITEM, _NDTYPE>*>(newNode)->getOpened();
	switch(_type)
	{
	case ndParentheses:
		{
			nodes::parentheses<_ITEM, _NDTYPE>* thisPa = static_cast<nodes::parentheses<_ITEM, _NDTYPE>*>(this),
				*newPa = static_cast<nodes::parentheses<_ITEM, _NDTYPE>*>(newNode);
			if(thisPa->getOpened() <= 0 && 0 <= newOpened)
			{
				cnode* unknown = node<_ITEM, _NDTYPE>::newNodeUnknownEmpty(thisPa, newPa);
				unknown = insert_unknown(unknown);
				unknown->insertToRight(newNode);
			}
			else
				insertToRight(newNode);
		}
		break;
	case ndAlpha:
	case ndNumber:
	case ndString:
		if (0 <= newOpened)
			insertEmptyUnknown(newNode)->insertToRight(newNode);
		else
			newNode = insertParenthesesClose(newNode);
		break;
	case ndSpace:
		{
			_type = ndUnknown;
			insertToRight(newNode);
		}
		break;
	case ndUnknown:
		if (0 <= newOpened)
			insertToRight(newNode);
		else
			newNode = insertParenthesesClose(newNode);
		break;
	default:
		insertToRight(newNode);
		break;
	}
	return newNode;
}


template <class _ITEM, class _NDTYPE>
void node<_ITEM, _NDTYPE>::insertWithLowPriority(node<_ITEM, _NDTYPE>* newNode)
{
	node<_ITEM, _NDTYPE>* top;
	for(top = this; top && top->_parent && 
			(top->_parent->_type == ndUnknown || top->_parent->_type == ndAlpha || top->_parent->_type == ndNumber 
				|| top->_parent->_type == ndString || top->_parent->_type == ndSpace); top = top->_parent)
		;
	if(top)
		top->insertInThisNode(newNode);
}



template <class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::insert_list(cnode* newNode)
{
	node<_ITEM, _NDTYPE>* ndRet = nullptr;
	switch(_type)
	{
	case ndSpace:
		{
			node<_ITEM, _NDTYPE>* parent = deleteThisNode();
			if(parent)
				parent->insertWithLowPriority(ndRet = newNode);
			else
				ndRet = newNode;
		}
		break;
	case ndAlpha:
	case ndNumber:
	case ndString:
	case ndUnknown:
		insertWithLowPriority(ndRet = newNode);
		if (newNode->getType() == ndList && newNode->_parent && newNode->_parent->getType() == ndList)
			newNode->switchToUpNode();
		break;
	case ndList:
	case ndParentheses:
		ndRet = insertUnknownListInParentheses(newNode);
		break;
	default:
		TRACE_NODE(logError, "Memory leak in node::insert_alpha", nullptr, newNode);
		break;
	}
	return ndRet;
}



template <class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::insert_unknown(cnode* newNode)
{
	cnode* ndRet;
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
	case ndUnknown:
		insertToRight(ndRet = newNode);
		break;
	case ndParentheses:
		ndRet = insertUnknownListInParentheses(newNode);
		break;
	default:
		TRACE_NODE(logError, "Error node::insert_unknown", nullptr, newNode);
		throw exception<_ITEM>(newNode, "insert unknown", true);
		break;
	}
	return ndRet;
}



template<class _ITEM, typename _NDTYPE>
node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::insertParenthesesClose(node* newParenthesesClose)
{
	node* pNode = this;
	for (short close = static_cast<nodes::parentheses<_ITEM, _NDTYPE>*>(newParenthesesClose)->getOpened(); close < 0; )
	{
		for (; pNode && (pNode->_type != ndParentheses || static_cast<nodes::parentheses<_ITEM, _NDTYPE>*>(pNode)->getOpened() < 0); pNode = pNode->_parent)
			;
		if (!pNode)
			throw exception<_ITEM>(newParenthesesClose, "no open parenthesis found", true);

		nodes::parentheses<_ITEM, _NDTYPE>* pParentheses = static_cast<nodes::parentheses<_ITEM, _NDTYPE>*>(pNode);
		short open = pParentheses->getOpened();
		node* pnewParenClose = nullptr;
		if (-close < open)
			pNode = pParentheses->split_right(-close);
		else if (open < -close)
			pnewParenClose = static_cast<nodes::parentheses<_ITEM, _NDTYPE>*>(newParenthesesClose)->split(open);
		newParenthesesClose->_parent = pNode->_parent;
		newParenthesesClose->_left = pNode;
		if (newParenthesesClose->_parent)
		{
			if (newParenthesesClose->_parent->_right == pNode)
				newParenthesesClose->_parent->_right = newParenthesesClose;
			else if (newParenthesesClose->_parent->_left == pNode)
				newParenthesesClose->_parent->_left = newParenthesesClose;
		}
		pNode->_parent = newParenthesesClose;

		pNode = newParenthesesClose->_parent;
		if (pnewParenClose)
			newParenthesesClose = pnewParenClose;
		close += open;
	}
	return newParenthesesClose;
}



template <class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::insertUnknownListInParentheses(cnode* newUnknown)
{
	if(_type != ndParentheses || (newUnknown->_type != ndUnknown && newUnknown->_type != ndList))
		return nullptr;

	if (0 <= static_cast<nodes::parentheses<_ITEM, _NDTYPE>*>(this)->getOpened())
		insertToRight(newUnknown);
	else
		insertInThisNode(newUnknown);
	return newUnknown;
}


template <class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>* node<_ITEM, _NDTYPE>::foundOpenParentheses(short& closed, node*& lastParentOpened)
{
	lastParentOpened = nullptr;
	if(_type != ndParentheses)
		return nullptr;
	nodes::parentheses<_ITEM, _NDTYPE>* parentheses = static_cast<nodes::parentheses<_ITEM, _NDTYPE>*>(this);
	short count = closed = parentheses->getOpened();
	closed *= -1;
	cnode* nd;
	for (nd = _parent; nd && count < 0; nd = count < 0 ? nd->_parent : nd)
	{
		if (parentheses->isThisType(nd))
		{
			short opened = static_cast<nodes::parentheses<_ITEM, _NDTYPE>*>(lastParentOpened = nd)->getOpened();
			count += opened;
			if (opened < closed)
				closed -= opened;
		}
	}
	if (count < 1)
		lastParentOpened = nullptr;
	return count < 0 ? nullptr: nd;
}



template <class _ITEM, class _NDTYPE>
int node<_ITEM, _NDTYPE>::getMetricsNodes(int& size) const
{
	int vRet = 0;
	if(this)
	{
		vRet = _left->getMetricsNodes(size) + 1 + _right->getMetricsNodes(size);
		size += getSizeofNode(_type);
	}
	return vRet;
}


template <class _ITEM, class _NDTYPE>
int node<_ITEM, _NDTYPE>::getMetricsNodes(cNdType type, int& size) const
{
	int vRet = 0;
	if(this)
		vRet = _left->getMetricsNodes(type, size) + (_type == type ? 1: 0) + _right->getMetricsNodes(type, size);
	size = getSizeofNode(type);
	return vRet;
}


template <class _ITEM, class _NDTYPE>
int node<_ITEM, _NDTYPE>::getSizeofNode(cNdType type)
{
	return type == ndList ? sizeof(nodes::calc<_ITEM, _NDTYPE>) :
		type == ndParentheses ? sizeof(nodes::parentheses<_ITEM, _NDTYPE>) :
		type == ndUnknown ? sizeof(nodes::unknown<_ITEM, _NDTYPE>) : sizeof(node<_ITEM, _NDTYPE>);
}


template <class _ITEM, class _NDTYPE>
const char* node<_ITEM, _NDTYPE>::getTypeL() const
{
	return _type == ndList ? "ndList" :
		_type == ndParentheses ? "ndParentheses" :
		_type == ndUnknown ? "ndUnknown" : 
		_type == ndSpace ? "ndSpace" :
		_type == ndAlpha ? "ndAlpha" :
		_type == ndNumber ? "ndNumber" :
		_type == ndString ? "ndString" : "<nd error>";
}


template<class _ITEM, typename _NDTYPE>
bool node<_ITEM, _NDTYPE>::checkParentheses() const
{
	short opened = 0;

	const cnode* open = nullptr;
	const cnode* close = nullptr;
	for (const cnode* nuk = getFirstParenthesisNode(); nuk; nuk = nuk->getNextParenthesisNode() )
	{
		if (nuk->getType() == ndParentheses)
		{
			short o = static_cast<const nodes::parentheses<_ITEM, _NDTYPE>*>(nuk)->getOpened();
			o < 0 ? close : open = nuk;
			opened += o;
		}
	}
	if (opened)
	{
		if (opened < 0)
			throw exception<_ITEM>(close, "too many closed parentheses");
		throw exception<_ITEM>(open, "missing close parentheses");
	}
	return true;
}


template<class _ITEM, typename _NDTYPE>
bool node<_ITEM, _NDTYPE>::check() const
{
	if (_parent && _parent->_left != this && _parent->_right != this)
		throw exception<_ITEM>(this, "Error in node parent for %item");
	if (_left && _left == _right)
		throw exception<_ITEM>(this, "Error childs are equals for %item");
	if (_left && _left->_parent != this)
		throw exception<_ITEM>(this, "Error in node left for %item");
	if (_right && _right->_parent != this)
		throw exception<_ITEM>(this, "Error in node right for %item");
	return true;
}




}
}
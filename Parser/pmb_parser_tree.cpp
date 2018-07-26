#include "StdAfx.h"
#include "pmb_parser_tree.h"


int get_DeltaTimeMS(const SYSTEMTIME& s0, const SYSTEMTIME& s1);



namespace pmb
{
namespace parser
{



template<class _ITEM, class _NDTYPE>
tree<_ITEM, _NDTYPE>::tree()
	: _root(nullptr)
{
}


template<class _ITEM, class _NDTYPE>
tree<_ITEM, _NDTYPE>::~tree()
{
	pmb::log* pLg = pmb::log::beginFunction(logDebug, "pmb::parser::tree::~tree");
	pLg->trace(logDebug, "deleting tree 0x08%x, 0x08%x\n", this, _root);
	pLg->flush();
	if(_root)
	{
		SYSTEMTIME si;
		GetSystemTime(&si);
		pLg->trace(logDebug, "%02d:%02d:%02d.%03d Begin deleting tree\n", si.wHour, si.wMinute, si.wSecond, si.wMilliseconds);
		for(cnode* nd = _root->getRootNode()->getFirstLeftNode(), * nxt; nd; nd = nxt)
		{
			nxt = nd->_parent;
			if (nxt && nxt->_right && nxt->_right != nd)
			{
				for (nxt = nxt->_right->getFirstLeftNode(); nxt->_right; nxt = nxt->_right->getFirstLeftNode())
					;
			}
			TRACE_NODE(logDebug, "del ", nullptr, nd, false, false);
			delete nd;
		}
		SYSTEMTIME si1;
		GetSystemTime(&si1);
		int dmsec = get_DeltaTimeMS(si, si1);
		pLg->trace(logDebug, "%02d:%02d:%02d.%03d End deleted tree time take: %02d:%02d:%02d.%03d\n", si1.wHour, si1.wMinute, si1.wSecond, si1.wMilliseconds,
			dmsec / 1000 / 3600, dmsec / 1000 / 60 % 60, dmsec / 1000 % 60, dmsec % 1000);
	}
	pLg->endFunction(logDebug);
}


template<class _ITEM, class _NDTYPE>
void tree<_ITEM, _NDTYPE>::insert(cnode* newNode)
{
	if(newNode)
	{
		if(!_root)
			_root = _cursor = newNode;
		else
		{
			cnode* nd = _cursor->insert(newNode);
			if(nd)
				_cursor = nd;
		}
	}
}


template<class _ITEM, class _NDTYPE>
const node<_ITEM, _NDTYPE>* tree<_ITEM, _NDTYPE>::getRootNode() const
{
	return _root ? const_cast<const cnode*>(_root)->getRootNode(): nullptr;
}


template<class _ITEM, class _NDTYPE>
node<_ITEM, _NDTYPE>* tree<_ITEM, _NDTYPE>::getRootNode()
{
	return _root ? _root->getRootNode(): nullptr;
}

template<class _ITEM, class _NDTYPE>
void tree<_ITEM, _NDTYPE>::check() const
{
	if (_root)
		_root->checkParentheses();
}




#ifdef DEBUG
template<class _ITEM, class _NDTYPE>
void tree<_ITEM, _NDTYPE>::trace(const char* expr) const
{
	pmb::log* pLg = pmb::log::beginFunction(logDebug, "pmb::parser::tree::trace");
	if (!this)
	{
		pLg->trace(logWarning, "NULL TREE!\n");
	}
	else if (_root)
	{
		std::map<cnode*, bool> mnodes;
		int cycle = 0;
		for (cnode* nd = _root->getRootNode()->getFirstLeftNode(); nd; )
		{
			TRACE_NODE(logDebug, "", expr, nd, false);

			if (nd->_left && nd->_left == nd->_right)
				pLg->trace(logError, "Error my childs are equals: left = 0x%08X == right = 0x%08X\n", nd->_left, nd->_right);
			if (nd->_left && nd->_left->_parent != nd)
				pLg->trace(logError, "Error my left child not has me like parent: left = 0x%08X -> parent = 0x%08X != this = 0x%08X\n", nd->_left, nd->_left->_parent, nd);
			if (nd->_right && nd->_right->_parent != nd)
				pLg->trace(logError, "Error my right child not has me like parent: right = 0x%08X -> parent = 0x%08X != this = 0x%08X\n", nd->_right, nd->_right->_parent, nd);

			if (nd->_parent && nd->_parent->_left != nd && nd->_parent->_right != nd)
				pLg->trace(logError, "Error my parent not has me: parent = 0x%08X -> parent->left = 0x%08X != this = 0x%08X && parent->right = 0x%08X != this = 0x%08X\n", nd->_parent, nd->_parent->_left, nd, nd->_parent->_right, nd);
			if (nd->_parent && nd->_parent->_left && nd->_parent->_left == nd->_parent->_right)
				pLg->trace(logError, "Error my parent has repited childs: parent = 0x%08X -> parent->left = 0x%08X == parent->right = 0x%08X\n", nd->_parent, nd->_parent->_left, nd->_parent->_right);

			if (mnodes.find(nd) != mnodes.end())
			{
				pLg->trace(logError, "Error Cyclic tree!!! repited node reference: node = 0x%08X\n", nd);
				mnodes[nd] = true;
				++cycle;
			}
			else
				mnodes[nd] = false;

			cnode* nxt;
			if (!nd->_right)
			{
				nxt = nd->_parent;
				for (cnode* prev = nd; nxt && nxt->_right == prev; nxt = nxt->_parent)
					prev = nxt;
			}
			else
				nxt = nd->_right->_left ? nd->_right->getFirstLeftNode() : nd->_right;
			nd = nxt;
		}
		pLg->trace(logDebug, "%d nodes found, %d cycle nodes\n", mnodes.size(), cycle);
	}
	pLg->endFunction(logDebug);
}

#endif // DEBUG


}
}
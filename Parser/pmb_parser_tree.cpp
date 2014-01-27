#include "StdAfx.h"
#include "pmb_parser_tree.h"


int get_DeltaTimeMS(const SYSTEMTIME& s0, const SYSTEMTIME& s1);



namespace pmb
{
namespace parser
{



template<class _TVALUE>
tree<_TVALUE>::tree()
	: _root(NULL)
{
}


template<class _TVALUE>
tree<_TVALUE>::~tree()
{
	if(_root)
	{
		SYSTEMTIME si;
		GetSystemTime(&si);
		AfxTrace(_T("%02d:%02d:%02d.%03d Begin deleting tree\n"), si.wHour, si.wMinute, si.wSecond, si.wMilliseconds);
		for(node<_TVALUE>* nd = _root->getRootNode()->getFirstLeftNode(); nd; )
		{
			node<_TVALUE>* nxt;
			if(!nd->_right)
			{
				nxt = nd->_parent;
				if(nxt && nxt->_right == nd)
					nxt->_right = NULL;
				TRACE_NODE("del ", nd, false, false);
				switch(nd->_type)
				{
				case ndUnknow:
					static_cast<nodes::unknow<_TVALUE>*>(nd)->~unknow();
					break;
				case ndParentheses:
					static_cast<nodes::parentheses<_TVALUE>*>(nd)->~parentheses();
					break;
				case ndList:
					static_cast<nodes::list<_TVALUE>*>(nd)->~list();
					break;
				}
				delete nd;
			}
			else
				nxt = nd->_right->_left ? nd->_right->getFirstLeftNode(): nd->_right;
			nd = nxt;
		}
		SYSTEMTIME si1;
		GetSystemTime(&si1);
		int dmsec = get_DeltaTimeMS(si, si1);
		AfxTrace(_T("%02d:%02d:%02d.%03d End deleted tree time take: %02d:%02d:%02d.%03d\n"), si1.wHour, si1.wMinute, si1.wSecond, si1.wMilliseconds,
			dmsec / 1000 / 3600, dmsec / 1000 / 60 % 60, dmsec / 1000 % 60, dmsec % 1000);
	}
}


template<class _TVALUE>
void tree<_TVALUE>::insert(node<_TVALUE>* newNode)
{
	if(newNode)
	{
		if(!_root)
			_root = _cursor = newNode;
		else
		{
			node<_TVALUE>* nd = _cursor->insert(newNode);
			if(nd)
				_cursor = nd;
		}
	}
}


template<class _TVALUE>
const node<_TVALUE>* tree<_TVALUE>::getRootNode() const
{
	return _root ? const_cast<const node<_TVALUE>*>(_root)->getRootNode(): NULL;
}


template<class _TVALUE>
node<_TVALUE>* tree<_TVALUE>::getRootNode()
{
	return _root ? _root->getRootNode(): NULL;
}



}
}
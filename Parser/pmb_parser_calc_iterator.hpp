#pragma once

#include "pmb_parser_calc_iterator.h"


namespace pmb
{
namespace parser
{
namespace calc
{



template<class _TVARGS, class _TREE>
inline iterator<_TVARGS, _TREE>::iterator(tptree* tree)
	: _tree(tree), _root(nullptr), _function(nullptr), _rootCalc(nullptr)
{
}


template<class _TVARGS, class _TREE>
inline iterator<_TVARGS, _TREE>::~iterator()
{
	clear(_rootCalc);
	clear(_root);
}


template<class _TVARGS, class _TREE>
inline void iterator<_TVARGS, _TREE>::clear(inode* root)
{
	for (_cursor = root; _cursor; _cursor = root)
	{
		root = _cursor->_next;
		delete _cursor;
	}
}



template<class _TVARGS, class _TREE>
inline typename iterator<_TVARGS, _TREE>::inode* iterator<_TVARGS, _TREE>::begin()
{
	if (!_root)
	{
		_begined = false;
		_root = new inode;
		_root->_prev = _root->_next = nullptr;
		_root->_isVarDependent = _root->_isCalculated = false;
		_root->_node = _rootNode = _tree->getRootNode();
		_root->_nodeJoinLast = _root->_nodeLast = nullptr;
		if (_root->_node && _root->_node->isCalcType())
		{
			const nodes::calc<_TREE::cItem, _TREE::cNdType>* uc = static_cast<const nodes::calc<_TREE::cItem, _TREE::cNdType>*>(_root->_node);
			_root->_node = uc->firstCalc();
		}
	}
	else if (_begined)
	{
		_begined = false;
		_cursor->_isCalculated = true;
		++(*this);
		return _cursor;
	}
	return _cursor = _root;
}


template<class _TVARGS, class _TREE>
inline void iterator<_TVARGS, _TREE>::end()
{
	if (!_cursor)
		_cursor = _root;
	if (_cursor && !_cursor->_isVarDependent && !_function)
	{
		_cmap.clear();
		for (inode* nd = _root; nd; nd = nd->_next)
			_cmap[nd->_node] = nd;
		_cursor->_transporter.clean();
	}
}


template<class _TVARGS, class _TREE>
inline bool iterator<_TVARGS, _TREE>::operator()() const
{
	return *this;
}

template<class _TVARGS, class _TREE>
inline iterator<_TVARGS, _TREE>::operator bool() const
{
	return _cursor && _cursor->_node && _cursor->_node->isCalcType();
}

template<class _TVARGS, class _TREE>
inline void iterator<_TVARGS, _TREE>::calculated(bool bCalculated)
{
	_cursor->_isCalculated = bCalculated;
}

template<class _TVARGS, class _TREE>
inline bool iterator<_TVARGS, _TREE>::calculated() const
{
	return _cursor->_isCalculated;
}

template<class _TVARGS, class _TREE>
inline bool iterator<_TVARGS, _TREE>::calculating() const
{
	return _rootCalc;
}

template<class _TVARGS, class _TREE>
inline void iterator<_TVARGS, _TREE>::varDependent(bool bVarDependent)
{
	_cursor->_isVarDependent = bVarDependent;
}

template<class _TVARGS, class _TREE>
inline bool iterator<_TVARGS, _TREE>::is_varDependent() const
{
	return _cursor->_isVarDependent;
}

template<class _TVARGS, class _TREE>
inline typename const iterator<_TVARGS, _TREE>::tnode*
	iterator<_TVARGS, _TREE>::rootNode() const
{
	return _rootNode;
}


template<class _TVARGS, class _TREE>
inline typename iterator<_TVARGS, _TREE>::nargs iterator<_TVARGS, _TREE>::getNArgs() const
{
	return nargs(0); /// falta ver los nodes::list
}

template<class _TVARGS, class _TREE>
inline typename iterator<_TVARGS, _TREE>::transporter_args& 
	iterator<_TVARGS, _TREE>::getValues()
{
	return _cursor->_transporter;
}


template<class _TVARGS, class _TREE>
inline bool iterator<_TVARGS, _TREE>::hasValue(nargs argN) const
{
	return argN < _cursor->_transporter.size() ? _cursor->_transporter[argN].isNotNull(): false;
}


template<class _TVARGS, class _TREE>
inline bool iterator<_TVARGS, _TREE>::hasPlace(nargs argN) const
{
	return argN < _cursor->_transporter.size() ? _cursor->_transporter[argN].hasPlace() : false;
}

template<class _TVARGS, class _TREE>
inline typename iterator<_TVARGS, _TREE>::transporter&
	iterator<_TVARGS, _TREE>::operator[] (nargs argN)
{
	return _cursor->_transporter[argN];
}


template<class _TVARGS, class _TREE>
inline typename iterator<_TVARGS, _TREE>::transporter&
	iterator<_TVARGS, _TREE>::left()
{
	return _cursor->_transporter.left();
}


template<class _TVARGS, class _TREE>
inline typename iterator<_TVARGS, _TREE>::transporter&
	iterator<_TVARGS, _TREE>::right()
{
	return _cursor->_transporter.right();
}


template<class _TVARGS, class _TREE>
inline typename iterator<_TVARGS, _TREE>::transporter&
	iterator<_TVARGS, _TREE>::result()
{
	return _cursor->_transporter.result();
}

template<class _TVARGS, class _TREE>
inline void iterator<_TVARGS, _TREE>::parameters_clear()
{
	_parameters.clean();
}

template<class _TVARGS, class _TREE>
inline typename iterator<_TVARGS, _TREE>::nargs iterator<_TVARGS, _TREE>::parameters_amount() const
{
	return _parameters.capacity();
}

template<class _TVARGS, class _TREE>
inline void iterator<_TVARGS, _TREE>::parameters_add(transporter& param)
{
	_parameters.add_back(param);
}


template<class _TVARGS, class _TREE>
inline void iterator<_TVARGS, _TREE>::parameters_set(transporter_args& arguments)
{
	_parameters = arguments;
	clear(_rootCalc);
	_rootCalc = _cursor = new inode;
	*_cursor = { 0 };
}



template<class _TVARGS, class _TREE>
inline typename iterator<_TVARGS, _TREE>::tptree* iterator<_TVARGS, _TREE>::tree()
{
	return _tree;
}

template<class _TVARGS, class _TREE>
inline void iterator<_TVARGS, _TREE>::setBegined()
{
	_begined = true;
}

template<class _TVARGS, class _TREE>
inline bool iterator<_TVARGS, _TREE>::function() const
{
	return _function;
}

template<class _TVARGS, class _TREE>
inline void iterator<_TVARGS, _TREE>::function(const tnode* breakNode)
{
	_function = breakNode;
}


template<class _TVARGS, class _TREE>
inline typename const iterator<_TVARGS, _TREE>::tnode*
	iterator<_TVARGS, _TREE>::node() const
{
	return _cursor->_node;
}

template<class _TVARGS, class _TREE>
inline typename const iterator<_TVARGS, _TREE>::tnode*
	iterator<_TVARGS, _TREE>::lastNode() const
{
	return _cursor->_nodeLast;
}

template<class _TVARGS, class _TREE>
inline typename const iterator<_TVARGS, _TREE>::tnode*
	iterator<_TVARGS, _TREE>::lastJoinNode() const
{
	return _cursor->_nodeJoinLast;
}


template<class _TVARGS, class _TREE>
inline typename iterator<_TVARGS, _TREE>::inode*
	iterator<_TVARGS, _TREE>::operator++()
{
	if (_rootCalc && !_cursor->_node)
	{
		*_cursor = *_root;
		_cursor->_prev = _cursor->_next = nullptr;
		_cursor->_isVarDependent = false;
		return _cursor;
	}

	const tnode* parent = _cursor->_node->getParent();
	if (parent)
	{
		if (_cursor->_node->getType() == ndUnknown)
		{
			if (parent->getType() == ndUnknown && static_cast<const nodes::unknown<_TREE::cItem, typename _TREE::cNdType>*>(parent)->isBinary())
			{
				if (!static_cast<const nodes::unknown<_TREE::cItem, typename _TREE::cNdType>*>(parent)->isCallFunction() && _cursor->_transporter.capacity() < 2)
				{
					if (parent->getLeft() == _cursor->_node)
						_cursor->_transporter.add_back();
					else if (parent->getRight() == _cursor->_node)
						_cursor->_transporter.add_front();
				}
				else if (static_cast<const nodes::unknown<_TREE::cItem, typename _TREE::cNdType>*>(parent)->isCallFunction() && 1 < _cursor->_transporter.capacity())
				{
					if (parent->getLeft() == _cursor->_node)
						_cursor->_transporter.removeRights();
					else if (parent->getRight() == _cursor->_node)
						_cursor->_transporter.removeLefts();
				}
			}
			if (_cursor->_isCalculated)
			{
				if (parent->getLeft() == _cursor->_node)
					_cursor->_transporter.moveToLeft();
				else if (parent->getRight() == _cursor->_node)
					_cursor->_transporter.moveToRight();
			}
		}
		else if (parent->getType() != ndParentheses && _cursor->_node->getType() == ndParentheses)
		{
			if (parent->getType() == ndList
				|| parent->getType() == ndUnknown && !static_cast<const nodes::unknown<_TREE::cItem, typename _TREE::cNdType>*>(parent)->isCallFunction() 
					&& (!_cursor->_nodeLast || _cursor->_nodeLast->getType() != ndList) && static_cast<const nodes::unknown<_TREE::cItem, typename _TREE::cNdType>*>(parent)->isBinary())
			{
				if (parent->getLeft() == _cursor->_node)
					_cursor->_transporter.add_back();
				else if (parent->getRight() == _cursor->_node)
					_cursor->_transporter.add_front();
			}
		}
		else if (parent->getType() == ndList && _cursor->_node->getType() == ndList)
		{
			if (parent->getLeft() == _cursor->_node)
				_cursor->_transporter.add_back();
			else if (parent->getRight() == _cursor->_node)
				_cursor->_transporter.add_front();
		}
	}

	const nodes::calc<_TREE::cItem, typename _TREE::cNdType>* uc = static_cast<const nodes::calc<_TREE::cItem, _TREE::cNdType>*>(_cursor->_node);
	if (_rootCalc)
	{
		uc = static_cast<const nodes::calc<_TREE::cItem, _TREE::cNdType>*>(next_calc());
		if (!uc)
			return _cursor;
	}
	else
		uc = uc->nextCalc();

	if (!uc)
		_cursor = nullptr;
	else if (_cursor->_node->getParent() != uc || !_cursor->_isCalculated && !_function && !_cursor->_isVarDependent && !_cursor->_next)
	{
		_cursor->_next = new inode;
		_cursor->_next->_prev = _cursor;
		_cursor = _cursor->_next;
		_cursor->_next = nullptr;
		_cursor->_isVarDependent = _cursor->_prev && _cursor->_prev->_isVarDependent && uc == _cursor->_prev->_node->getParent();
		_cursor->_nodeJoinLast = _cursor->_nodeLast = nullptr;
	}
	else
	{
		if (_cursor->_prev && (_cursor->_prev->_node == uc->getLeft() || _cursor->_prev->_node == uc->getRight()))
		{
			_cursor = _cursor->_prev;
			bool bOnlyFirst = uc->getType() != ndList || !uc->getLeft() || uc->getLeft()->getType() != ndList;
			bool bOnlyLast = uc->getType() != ndList || !uc->getRight() || uc->getRight()->getType() != ndList;
			if (_cursor->_node == uc->getLeft())
				_cursor->_transporter.join_l2r(_cursor->_next->_transporter, bOnlyFirst, bOnlyLast);
			else
				_cursor->_transporter.join_r2l(_cursor->_next->_transporter, bOnlyFirst, bOnlyLast);
			if (!_cursor->_next->_isVarDependent)
			{
				_cursor->_nodeLast = _cursor->_next->_node;
				_cursor->_nodeJoinLast = _cursor->_node;
				delete _cursor->_next;
				_cursor->_next = nullptr;
			}
		}
		else
			_cursor->_nodeLast = _cursor->_node;
	}

	if (_cursor)
	{
		_cursor->_isCalculated = false;
		_cursor->_node = uc;
	}
	return _cursor;
}


template<class _TVARGS, class _TREE>
inline typename iterator<_TVARGS, _TREE>::inode*
	iterator<_TVARGS, _TREE>::next()
{
	++(*this);
}


template<class _TVARGS, class _TREE>
inline typename iterator<_TVARGS, _TREE>::inode*
	iterator<_TVARGS, _TREE>::prev()
{
	return _cursor->_prev;
}



/*template<class _TVARGS, class _TREE>
inline typename iterator<_TVARGS, _TREE>::tnode*
	iterator<_TVARGS, _TREE>::_nextCalc()
{
	//	log* pLg = log::beginFunction(logDebug, "pmb::parser::nodes::calc::nextCalc");
	calc<_ITEM, _NDTYPE>* c,
		*nc;
	for (c = this, nc = this; c && nc; )
	{
		//		TRACE_NODE(logDebug, "\t\t- ", c, false, false);
		nc = NULL;
		if (c->_type == ndUnknown)
		{
			unknown<_ITEM, _NDTYPE>* uk = static_cast<unknown<_ITEM, _NDTYPE>*>(c);
			if (uk->getOperation())
			{
				if (uk->getOperation()->isFirstLeft())
				{
					if (isNotCalculated(uk->_left))
						nc = static_cast<calc<_ITEM, _NDTYPE>*>(uk->_left);
					if (!nc && isNotCalculated(uk->_right))
						nc = static_cast<calc<_ITEM, _NDTYPE>*>(uk->_right);
				}
				if (!nc && uk->getOperation()->isFirstRight())
				{
					if (isNotCalculated(uk->_right))
						nc = static_cast<calc<_ITEM, _NDTYPE>*>(uk->_right);
					if (!nc && isNotCalculated(uk->_left))
						nc = static_cast<calc<_ITEM, _NDTYPE>*>(uk->_left);
				}
				if (!nc)
				{
					if (uk->_bCalculated)
						nc = static_cast<calc<_ITEM, _NDTYPE>*>(uk->_parent);
				}
				else if (!nc)
					TRACE_NODE(logError, "pmb::parser::nodes::calc::nextCalc ERROR: Condition not match", nullptr, uk);
			}
		}
		else //if(c->_type == ndParentheses || c->_type == ndList)
		{
			if (isNotCalculated(c->_left))
				nc = static_cast<calc<_ITEM, _NDTYPE>*>(c->_left);
			if (!nc && isNotCalculated(c->_right))
				nc = static_cast<calc<_ITEM, _NDTYPE>*>(c->_right);
			if (!nc)
			{
				//				if(!c->_bCalculated)
				//					c->_bCalculated = true;
				if (c->_bCalculated)
					nc = static_cast<calc<_ITEM, _NDTYPE>*>(c->_parent);
			}
		}
		if (nc)
			c = nc;
	}
	//	pLg->endFunction();
	return !nc && c ? c->_bCalculated ? NULL : c : nc;
}*/




// this function is a copy of nodes:calc::nextCalc()
template<class _TVARGS, class _TREE>
inline typename const iterator<_TVARGS, _TREE>::tnode*
	iterator<_TVARGS, _TREE>::next_calc()
{
	const nodes::calc<_TREE::cItem, _TREE::cNdType>* nc,
		* nd = static_cast<const nodes::calc<_TREE::cItem, _TREE::cNdType>*>(_cursor->_node);

	if (nd->getParent() && nd->getParent()->isCalcType())
	{
		const nodes::calc<_TREE::cItem, _TREE::cNdType>* parent = static_cast<const nodes::calc<_TREE::cItem, _TREE::cNdType>*>(nd->getParent());
		if (parent->getType() == ndUnknown)
		{
			const nodes::unknown<_TREE::cItem, _TREE::cNdType>* parent_uk = static_cast<const nodes::unknown<_TREE::cItem, _TREE::cNdType>*>(parent);
			if (parent_uk->isValid())
			{
				if (parent_uk->isFirstLeft() && parent_uk->getLeft() == nd)
					nc = static_cast<const nodes::calc<_TREE::cItem, _TREE::cNdType>*>(parent->getRight() && parent->getRight()->isCalcType() ? parent->getRight() : parent);
				else if (parent_uk->isFirstRight() && parent_uk->getRight() == nd)
					nc = static_cast<const nodes::calc<_TREE::cItem, _TREE::cNdType>*>(parent->getLeft() && parent->getLeft()->isCalcType() ? parent->getLeft() : parent);
				else
					nc = parent;
			}
			else
				nc = parent;
		}
		else if (parent->getType() == ndList)
		{
			if (parent->getLeft() == nd)
				nc = static_cast<const nodes::calc<_TREE::cItem, _TREE::cNdType>*>(parent->getRight() && parent->getRight()->isCalcType() ? parent->getRight() : parent);
			else
				nc = parent;
		}
		else
			nc = parent;
	}
	else
		nc = nullptr;

	if (!nc)
		_cursor = nullptr;
	else if (nc != nd->getParent())
	{
		if (_cmap.find(nc) == _cmap.end())
			nc = nc->firstCalc();
	}

	if (nc)
	{
		if (nc == _function)
			_cursor->_node = nc = nullptr;
		else
		{
			map::const_iterator cf = _cmap.find(nc);
			if (cf != _cmap.end())
			{
				_cursor->_next = new inode;
				*_cursor->_next = *cf->second;
				_cursor->_next->_prev = _cursor;
				_cursor = _cursor->_next;
				_cursor->_next = nullptr;
				_cursor->_isVarDependent = false;
				nc = nullptr;
			}
		}
	}

	return nc;
}




















template<class _BUILD_IN_FUNCTION, class _CITERATOR>
inline pair_function<_BUILD_IN_FUNCTION, _CITERATOR>::pair_function()
	: _userDef(nullptr), _buildin(nullptr)
{
}

template<class _BUILD_IN_FUNCTION, class _CITERATOR>
inline pair_function<_BUILD_IN_FUNCTION, _CITERATOR>::pair_function(iterator* userDef, nargs nArgs)
	: _userDef(userDef), _buildin(nullptr), _nArgs(nArgs)
{
}

template<class _BUILD_IN_FUNCTION, class _CITERATOR>
inline pair_function<_BUILD_IN_FUNCTION, _CITERATOR>::pair_function(const tpbin_function* pFnc)
	: _userDef(nullptr), _buildin(pFnc)
{
	_nArgs = _buildin->getNArgs();
}

template<class _BUILD_IN_FUNCTION, class _CITERATOR>
inline bool pair_function<_BUILD_IN_FUNCTION, _CITERATOR>::operator!() const
{
	return !_userDef && !_buildin;
}

template<class _BUILD_IN_FUNCTION, class _CITERATOR>
inline pair_function<_BUILD_IN_FUNCTION, _CITERATOR>::operator bool() const
{
	return !*this;
}



}
}
}
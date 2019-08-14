#pragma once

#include <map>
#include <string>


namespace pmb 
{
namespace util
{



template<class _Ty, class _TyR>
struct less
//		: public binary_function<_Ty, _Ty, bool>
{	// functor for operator<
	bool operator()(const _Ty& left, const _Ty& right) const
	{	
		return left < right;
	}
	bool operator()(const _Ty& left, const _TyR& right) const
	{	
		return right > left;
	}
	bool operator()(const _TyR& left, const _Ty& right) const
	{	
		return left < right;
	}
};





#if _ITERATOR_DEBUG_LEVEL < 2
#define _PMB_DEBUG_LT_PRED(pred, x, y)	std::pred(x, y)

#else /* _ITERATOR_DEBUG_LEVEL < 2 */
#define _PMB_DEBUG_LT_PRED(pred, x, y) std::_Debug_lt_pred(pred, x, y)
#endif




template<class _Kty, typename _Ty, class _KFty, class _less = less<_Kty, _KFty>>
class map: public std::map<_Kty, _Ty, _less>
{
public:
	typedef typename _Kty _tpKey;
	typedef std::map<_Kty, _Ty, _less> _Mybase;
	typedef typename std::_Tmap_traits<_Kty, _Ty, _less, std::allocator<std::pair<const _Kty, _Ty>>, false> _Traits;
	typedef typename _Mybase::const_iterator const_iterator;
	typedef typename _Mybase::iterator iterator;
	typedef _Mybase::_Nodeptr _Nodeptr;
	typedef typename _Mybase::value_type value_type;

public:
	const_iterator find(const _Kty& key) const
	{
		return _Mybase::find(key);
	}

	iterator find(const _Kty& key)
	{
		return _Mybase::find(key);
	}

	// old version:
	const_iterator find_vold(const _KFty& kfind) const
	{
		// find an element in nonmutable sequence that matches _Keyval
		const_iterator _Where = lower_bound(kfind);
		return _Where == end() || _PMB_DEBUG_LT_PRED(this->_Getcomp(), kfind, this->_Key(_Where._Ptr)) ? end() : _Where;
	}
	
	// old version:
	iterator find_vold(const _KFty& kfind)
	{
		// find an element in nonmutable sequence that matches _Keyval
		iterator _Where = lower_bound(kfind);
		return _Where == end() || _PMB_DEBUG_LT_PRED(this->_Getcomp(), kfind, this->_Key(_Where._Ptr)) ? end() : _Where;
	}

	// old version
	_NODISCARD const_iterator lower_bound(const _KFty& _Keyval) const
	{	// find leftmost node not less than _Keyval in nonmutable tree
		return (const_iterator(_Lbound(_Keyval), _STD addressof(this->_Get_data())));
	}

	// old version
	_Nodeptr _Lbound(const _KFty& _Keyval) const
	{	// find leftmost node not less than _Keyval
		_Nodeptr _Wherenode = this->_Get_data()._Myhead;	// end() if search fails
		_Nodeptr _Pnode = _Wherenode->_Parent;

		while (!_Pnode->_Isnil)
			if (_Compare(this->_Key(_Pnode), _Keyval))
				_Pnode = _Pnode->_Right;	// descend right subtree
			else
			{	// _Pnode not less than _Keyval, remember it
				_Wherenode = _Pnode;
				_Pnode = _Pnode->_Left;	// descend left subtree
			}

		return _Wherenode;	// return best remembered candidate
	}

	// old version
	_NODISCARD iterator lower_bound(const _KFty& _Keyval)
	{	// find leftmost node not less than _Keyval in nonmutable tree
		return (iterator(_Lbound(_Keyval), _STD addressof(this->_Get_data())));
	}

	// old version
	_Nodeptr _Lbound(const _KFty& _Keyval)
	{	// find leftmost node not less than _Keyval
		_Nodeptr _Wherenode = this->_Get_data()._Myhead;	// end() if search fails
		_Nodeptr _Pnode = _Wherenode->_Parent;

		while (!_Pnode->_Isnil)
			if (_Compare(this->_Key(_Pnode), _Keyval))
				_Pnode = _Pnode->_Right;	// descend right subtree
			else
			{	// _Pnode not less than _Keyval, remember it
				_Wherenode = _Pnode;
				_Pnode = _Pnode->_Left;	// descend left subtree
			}

		return _Wherenode;	// return best remembered candidate
	}


	// std::map new version:

	_NODISCARD iterator find(const _KFty& _Keyval) {
		return iterator(_Find(_Keyval), _Get_scary());
	}


	_NODISCARD const_iterator find(const _KFty& _Keyval) const {
		return const_iterator(_Find(_Keyval), _Get_scary());
	}


	_NODISCARD _Nodeptr _Find(const _KFty& _Keyval) const {
		const std::_Tree_find_result<_Nodeptr> _Loc = _Find_lower_bound(_Keyval);
		if (_Lower_bound_duplicate(_Loc._Bound, _Keyval)) {
			return _Loc._Bound;
		}

		return _Get_scary()->_Myhead;
	}


	std::_Tree_find_result<_Nodeptr> _Find_lower_bound(const _KFty& _Keyval) const {
		const auto _Scary = _Get_scary();
		std::_Tree_find_result<_Nodeptr> _Result{ _Scary->_Myhead->_Parent, std::_Tree_child::_Right, _Scary->_Myhead };
		_Nodeptr _Trynode = _Result._Location._Parent;
		while (!_Trynode->_Isnil) {
			_Result._Location._Parent = _Trynode;
			if (std:: _DEBUG_LT_PRED(_Getcomp(), _Traits::_Kfn(_Trynode->_Myval), _Keyval)) {
				_Result._Location._Child = std::_Tree_child::_Right;
				_Trynode = _Trynode->_Right;
			}
			else {
				_Result._Location._Child = std::_Tree_child::_Left;
				_Result._Bound = _Trynode;
				_Trynode = _Trynode->_Left;
			}
		}

		return _Result;
	}


	bool _Lower_bound_duplicate(const _Nodeptr _Bound, const _KFty& _Keyval) const {
		return !_Bound->_Isnil && !std:: _DEBUG_LT_PRED(_Getcomp(), _Keyval, _Traits::_Kfn(_Bound->_Myval));
	}

};





}
}
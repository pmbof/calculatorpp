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
		return left < right;
	}
	bool operator()(const _TyR& left, const _Ty& right) const
	{	
		return left < right;
	}
};



template<class _Kty, typename _Ty, class _KFty>
class map: public std::map<_Kty, _Ty, less<_Kty, _KFty> >
{
public:
	typedef std::map<_Kty, _Ty, less<_Kty, _KFty> > _Mybase;
	typedef typename _Mybase::const_iterator const_iterator;

public:
	const_iterator find(const _Kty& key) const
	{
		return _Mybase::find(key);
	}
	const_iterator find(const _KFty& kfind) const
	{
		// find an element in nonmutable sequence that matches _Keyval
		const_iterator _Where = const_iterator(_Lbound(kfind), this);
		return _Where == end()
			|| _DEBUG_LT_PRED(this->_Getcomp(),
				kfind, this->_Key(_Where._Mynode()))
					? end() : _Where;
	}

	_Nodeptr _Lbound(const _KFty& _Keyval) const
	{	// find leftmost node not less than _Keyval
		_Nodeptr _Pnode = _Root();
		_Nodeptr _Wherenode = this->_Myhead;	// end() if search fails

		while (!this->_Isnil(_Pnode))
			if (_DEBUG_LT_PRED(this->_Getcomp(), this->_Key(_Pnode), _Keyval))
				_Pnode = this->_Right(_Pnode);	// descend right subtree
			else
			{	// _Pnode not less than _Keyval, remember it
				_Wherenode = _Pnode;
				_Pnode = this->_Left(_Pnode);	// descend left subtree
			}

		return _Wherenode;	// return best remembered candidate
	}

};





}
}
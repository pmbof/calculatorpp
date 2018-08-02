#pragma once


#include "pmb_parser_calc_transporter.h"


namespace pmb
{
namespace parser
{
namespace calc
{



template<class _VALUE, typename _NREF>
inline transporter<_VALUE, _NREF>::transporter()
{
	_prv = new sref_val;
	_prv->nref = 0;
	_prv->pvalue = nullptr;
	_prv->variable = true;
}


template<class _VALUE, typename _NREF>
inline transporter<_VALUE, _NREF>::transporter(bool bVariable)
	: _prv(nullptr)
{
}




template<class _VALUE, typename _NREF>
inline transporter<_VALUE, _NREF>::transporter(ptr_value pvalue)
{
	set(pvalue);
}


template<class _VALUE, typename _NREF>
inline transporter<_VALUE, _NREF>::transporter(const thisc& rcopy)
{
	_prv = rcopy._prv;
	add_ref();
}


template<class _VALUE, typename _NREF>
inline void transporter<_VALUE, _NREF>::add_ref()
{
	if (_prv)
		++_prv->nref;
}


template<class _VALUE, typename _NREF>
inline void transporter<_VALUE, _NREF>::set(ptr_value pvalue)
{
	_prv = new sref_val;
	_prv->nref = 0;
	_prv->pvalue = pvalue;
	_prv->variable = false;
}



template<class _VALUE, typename _NREF>
inline transporter<_VALUE, _NREF>::~transporter()
{
	clear();
}




template<class _VALUE, typename _NREF>
inline void transporter<_VALUE, _NREF>::clear()
{
	if (_prv)
	{
		if (!_prv->nref)
		{
			if (_prv->pvalue)
				delete _prv->pvalue;
			delete _prv;
		}
		else
			--_prv->nref;
		_prv = nullptr;
	}
}



template<class _VALUE, typename _NREF>
inline _VALUE* transporter<_VALUE, _NREF>::release()
{
	ptr_value vret;
	if (_prv)
	{
		vret = _prv->pvalue;
		_prv->pvalue = nullptr;
		if (!_prv->nref)
			delete _prv;
		else
			--_prv->nref;
		_prv = nullptr;
	}
	else
		vret = nullptr;
	return vret;
}





template<class _VALUE, typename _NREF>
inline typename transporter<_VALUE, _NREF>::thisc& transporter<_VALUE, _NREF>::operator=(typename thisc& right)
{
	if (_prv != right._prv)
	{
		if (_prv && _prv->variable)
		{
			if (_prv->pvalue)
				delete _prv->pvalue;
			if (right._prv)
			{
				if (right._prv->variable)
					_prv->pvalue = right._prv->pvalue ? new tpvalue(*right._prv->pvalue) : nullptr;
				else
					_prv->pvalue = right.release();
			}
			else
				_prv->pvalue = nullptr;
		}
		else
		{
			clear();
			_prv = right._prv;
			add_ref();
		}
	}
	else if (!_prv)
	{
		// Chequear si esto solo ocurre en la definicion de funciones entre los parametros de la funcion y las referencias internas
		set(nullptr);
		right.clear();
		right._prv = _prv;
		_prv->nref = 1;
	}
	else if (this != &right) // but _prv == right._prv
		add_ref();
	return *this;
}





template<class _VALUE, typename _NREF>
inline typename transporter<_VALUE, _NREF>::thisc& transporter<_VALUE, _NREF>::operator=(ptr_value right)
{
	if (_prv && _prv->pvalue != right)
	{
		if (!_prv->nref)
		{
			if (_prv->pvalue)
				delete _prv->pvalue;
			_prv->pvalue = right;
		}
		else
		{
			--_prv->nref;
			set(right);
		}
	}
	if (!_prv)
		set(right);
	return *this;
}





template<class _VALUE, typename _NREF>
inline bool transporter<_VALUE, _NREF>::hasPlace() const
{
	return _prv;
}

template<class _VALUE, typename _NREF>
inline bool transporter<_VALUE, _NREF>::isNull() const
{
	return !_prv || !_prv->pvalue;
}

template<class _VALUE, typename _NREF>
inline bool transporter<_VALUE, _NREF>::isNotNull() const
{
	return _prv && _prv->pvalue;
}

template<class _VALUE, typename _NREF>
inline bool transporter<_VALUE, _NREF>::operator!() const
{
	return isNull();
}


template<class _VALUE, typename _NREF>
_VALUE* transporter<_VALUE, _NREF>::operator*()
{
	return _prv ? _prv->pvalue : nullptr;
}

template<class _VALUE, typename _NREF>
_VALUE* transporter<_VALUE, _NREF>::operator*() const
{
	return _prv ? _prv->pvalue : nullptr;
}

template<class _VALUE, typename _NREF>
_VALUE* transporter<_VALUE, _NREF>::operator->()
{
	return _prv ? _prv->pvalue : nullptr;
}

template<class _VALUE, typename _NREF>
_VALUE* transporter<_VALUE, _NREF>::operator->() const
{
	return _prv ? _prv->pvalue : nullptr;
}









































template<class _TVALUE, typename _NARGS>
inline transporter_args<_TVALUE, _NARGS>::node::node()
	: t(false)
{
}




template<class _TVALUE, typename _NARGS>
inline transporter_args<_TVALUE, _NARGS>::transporter_args()
	: _root(nullptr), _result(false)
{
}


template<class _TVALUE, typename _NARGS>
inline transporter_args<_TVALUE, _NARGS>::~transporter_args()
{
	for (node* nd = _root; nd; )
	{
		node* del = nd;
		nd = nd->pnext;
		delete del;
	}
}



template<class _TVALUE, typename _NARGS>
inline transporter_args<_TVALUE, _NARGS>& transporter_args<_TVALUE, _NARGS>::operator=(const transporter_args& right)
{
	node* nd = _root;
	for (node* rnd = right._root; rnd; rnd = rnd->pnext, nd = nd->pnext)
	{
		if (!nd)
		{
			nd = new node;
			nd->pnext = nullptr;
			if (!_root)
			{
				_size = 0;
				_last = _root = nd;
			}
			else
			{
				_last->pnext = nd;
				_last = nd;
			}
			++_size;
		}
		nd->t = rnd->t;
	}
	if (nd)
	{
		for (node* del = nd; del; )
		{
			node* nxt = del->pnext;
			delete del;
			--_size;
			del = nxt;
		}
		if (nd == _root)
			_last = _root = nullptr;
		else
		{
			node* lst;
			for (lst = _root; lst->pnext != nd; lst = lst->pnext)
				;
			_last = lst;
		}
	}
	return *this;
}



template<class _TVALUE, typename _NARGS>
inline void transporter_args<_TVALUE, _NARGS>::placeFor(nargs argsN)
{
	for (_init(); _size < argsN; add_back())
		;
}


template<class _TVALUE, typename _NARGS>
inline void transporter_args<_TVALUE, _NARGS>::placeForResult()
{
	if (_result.isNull())
		_result = new transporter::tpvalue;
}

template<class _TVALUE, typename _NARGS>
inline typename transporter_args<_TVALUE, _NARGS>::transporter&
	transporter_args<_TVALUE, _NARGS>::operator[](nargs argN)
{
	node* pN = _root;
	for (int i = 0; pN && i < argN; ++i, pN = pN->pnext)
		;
	return pN->t;
}

template<class _TVALUE, typename _NARGS>
inline typename transporter_args<_TVALUE, _NARGS>::transporter&
	transporter_args<_TVALUE, _NARGS>::left()
{
	if (!_root)
		_init();
	return _root->t;
}

template<class _TVALUE, typename _NARGS>
inline typename transporter_args<_TVALUE, _NARGS>::transporter&
	transporter_args<_TVALUE, _NARGS>::right()
{
	if (!_root)
		_init();
	if (_root == _last)
		add_back();
	return _last->t;
}


template<class _TVALUE, typename _NARGS>
void transporter_args<_TVALUE, _NARGS>::moveToLeft()
{
	_root->t = _result.release();
}


template<class _TVALUE, typename _NARGS>
void transporter_args<_TVALUE, _NARGS>::moveToRight()
{
	_last->t = _result.release();
}


template<class _TVALUE, typename _NARGS>
inline void transporter_args<_TVALUE, _NARGS>::removeLefts()
{
	for (node* pN = _root; pN && pN != _last; --_size)
	{
		node* del = pN;
		pN = pN->pnext;
		delete del;
	}
	_root = _last;
}


template<class _TVALUE, typename _NARGS>
inline void transporter_args<_TVALUE, _NARGS>::removeRights()
{
	if (_root && _root->pnext && _root->pnext != _last)
	{
		_last = _root->pnext;
		for (node* pN = _last->pnext; pN; --_size)
		{
			node* del = pN;
			pN = pN->pnext;
			delete del;
		}
		_last->pnext = nullptr;
	}
}


template<class _TVALUE, typename _NARGS>
inline typename transporter_args<_TVALUE, _NARGS>::transporter& transporter_args<_TVALUE, _NARGS>::result()
{
	return _result;
}

template<class _TVALUE, typename _NARGS>
inline typename transporter_args<_TVALUE, _NARGS>::nargs transporter_args<_TVALUE, _NARGS>::capacity() const
{
	return _root ? _size: nargs(0);
}


template<class _TVALUE, typename _NARGS>
inline typename transporter_args<_TVALUE, _NARGS>::nargs transporter_args<_TVALUE, _NARGS>::size() const
{
	return nArgs();
}


template<class _TVALUE, typename _NARGS>
inline typename transporter_args<_TVALUE, _NARGS>::nargs transporter_args<_TVALUE, _NARGS>::nArgs() const
{
	nargs vret = nargs(0);
	for (node* pN = _root; pN; pN = pN->pnext)
		if (pN->t.isNotNull())
			++vret;
	return vret;
}


template<class _TVALUE, typename _NARGS>
inline bool transporter_args<_TVALUE, _NARGS>::haveLeft() const
{
	return _root && _root->t.isNotNull();
}

template<class _TVALUE, typename _NARGS>
inline bool transporter_args<_TVALUE, _NARGS>::haveRight() const
{
	return _root && _last->t.isNotNull();
}


template<class _TVALUE, typename _NARGS>
inline bool transporter_args<_TVALUE, _NARGS>::operator!() const
{
	for (node* pN = _root; pN; pN = pN->pnext)
		if (!pN->t)
			return true;
	return false;
}

template<class _TVALUE, typename _NARGS>
inline void transporter_args<_TVALUE, _NARGS>::join_l2r(transporter_args& targs, bool bOnlyFirst, bool bOnlyLast)
{
	if (bOnlyFirst)
	{
		for (node* nd = _root->pnext; nd; --_size)
		{
			node* del = nd;
			nd = nd->pnext;
			delete del;
		}
		_last = _root;
	}
	if (bOnlyLast)
	{
		if (_last->pnext = targs._last)
		{
			++_size;
			_last = _last->pnext;
		}
		for (node* nd = targs._root; nd && nd != targs._last; )
		{
			node* del = nd;
			nd = nd->pnext;
			delete del;
		}
	}
	else
	{
		if (_last->pnext = targs._root)
		{
			_size += targs._size;
			_last = targs._last;
		}
	}
	clean();
	targs._root = nullptr;
}


template<class _TVALUE, typename _NARGS>
inline void transporter_args<_TVALUE, _NARGS>::join_r2l(transporter_args & targs, bool bOnlyLast, bool bOnlyFirst)
{
	if (bOnlyLast)
	{
		for (node* nd = _root; nd && nd != _last; --_size)
		{
			node* del = nd;
			nd = nd->pnext;
			delete del;
		}
		_root = _last;
	}
	if (bOnlyFirst && targs._root)
	{
		for (node* nd = targs._root->pnext; nd; )
		{
			node* del = nd;
			nd = nd->pnext;
			delete del;
		}
		targs._root->pnext = _root;
		_root = targs._root;
		++_size;
	}
	else if (targs._root)
	{
		targs._last->pnext = _root;
		_root = targs._root;
		_size += targs._size;
	}
	clean();
	targs._root = nullptr;
}


template<class _TVALUE, typename _NARGS>
inline void transporter_args<_TVALUE, _NARGS>::_init()
{
	if (!_root)
	{
		_last = _root = new node;
		_root->pnext = nullptr;
		_size = nargs(1);
	}
}

template<class _TVALUE, typename _NARGS>
inline void transporter_args<_TVALUE, _NARGS>::add_back()
{
	if (!_root)
		_init();
	else
	{
		_last = _last->pnext = new node;
		_last->pnext = nullptr;
		++_size;
	}
}


template<class _TVALUE, typename _NARGS>
inline void transporter_args<_TVALUE, _NARGS>::add_back(transporter& arg)
{
	if (!_root)
		_init();
	else
	{
		_last = _last->pnext = new node;
		_last->pnext = nullptr;
		++_size;
	}
	_last->t = arg;
}


template<class _TVALUE, typename _NARGS>
inline void transporter_args<_TVALUE, _NARGS>::add_front()
{
	if (!_root)
		_init();
	else
	{
		node* second = _root;
		_root = new node;
		_root->pnext = second;
		++_size;
	}
}


template<class _TVALUE, typename _NARGS>
inline void transporter_args<_TVALUE, _NARGS>::clean()
{
	for (node* nd = _root, *prev = nullptr; nd; )
	{
		if (nd->t.isNull())
		{
			--_size;
			node* del = nd;
			nd = nd->pnext;
			if (prev)
				prev->pnext = nd;
			else
				_root = nd;
			if (_last == del)
				_last = prev;
			delete del;
		}
		else
		{
			prev = nd;
			nd = nd->pnext;
		}
	}
}





}
}
}
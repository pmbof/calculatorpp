#include "StdAfx.h"

#include "pmb_parser_transporter.h"



namespace pmb
{
namespace parser
{

template <class _VALUE>
_VALUE* transporter<_VALUE>::getNewValue(const char* str, int len)
{
	return new _VALUE(str, len);
}

/*template <class _VALUE>
transporter<_VALUE>::transporter(int)
	: _state(tpsNULL)
{
}*/

template <class _VALUE>
transporter<_VALUE>::transporter()
	: _state(tpsNULLNoTransferable), _next(NULL)
{
}

template <class _VALUE>
transporter<_VALUE>::transporter(bool)
	: _state(tpsNULL), _next(NULL)
{
}

template <class _VALUE>
transporter<_VALUE>::transporter(_valuePtr value, bool transferable)
	: _state(transferable ? tpsTransferable: tpsNoTransferable), _value(value), _next(NULL)
{
}

template <class _VALUE>
transporter<_VALUE>::transporter(_Myt* next)
	: _state(tpsNULL), _next(next)
{
}

template <class _VALUE>
transporter<_VALUE>::~transporter()
{
	_release();
}

template <class _VALUE>
transporter<_VALUE>* transporter<_VALUE>::_target()
{
	_Myt* ptr;
	for (ptr = this; ptr->_state == tpsTransporter; ptr = ptr->_pValue)
		;
	return ptr;
}

template <class _VALUE>
const transporter<_VALUE>* transporter<_VALUE>::_target() const
{
	const _Myt* ptr;
	for (ptr = this; ptr->_state == tpsTransporter; ptr = ptr->_pValue)
		;
	return ptr;
}


template <class _VALUE>
bool transporter<_VALUE>::operator!() const
{
	const _Myt* trg = _target();
	return trg->_state == tpsNULL || trg->_state == tpsNULLNoTransferable;
}

template <class _VALUE>
bool transporter<_VALUE>::isNull() const
{
	return operator!();
}

template <class _VALUE>
bool transporter<_VALUE>::isTransferable() const
{
	return _state == tpsTransferable;
}

template <class _VALUE>
bool transporter<_VALUE>::isNoTransferable() const
{
	return _state == tpsNULLNoTransferable || _state == tpsNoTransferable;
}

template <class _VALUE>
const transporter<_VALUE>* transporter<_VALUE>::getNext() const
{
	return _next;
}

template <class _VALUE>
void transporter<_VALUE>::setNext(transporter<_VALUE>* rValue)
{
	_next = rValue;
}


template <class _VALUE>
void transporter<_VALUE>::_release()
{
	for(_Myt* cursor = this; cursor && !cursor->isNull(); )
	{
		if (cursor->_state == tpsTransporter)
			break;
		if (!cursor->isNull() && cursor->_state != tpsTransferred && cursor->_value)
			delete cursor->_value;
		break;

		_Myt* del = cursor;
		cursor = cursor->_next;
		if(del != this)
			delete del;
	}
}

template <class _VALUE>
void transporter<_VALUE>::release()
{
	_release();
	if (_state != tpsTransporter && !isNull())
		_state = _state == tpsTransferable || _state == tpsTransferred ? tpsNULL : tpsNULLNoTransferable;
}


template <class _VALUE>
transporter<_VALUE>* transporter<_VALUE>::getPlace()
{
	_Myt* cursor;
	for(cursor = this; cursor && (cursor->_state == tpsTransferred || cursor->_state == tpsNoTransferable); cursor = cursor->_next)
		;
	return cursor && (cursor->_state == tpsTransferable || cursor->_state == tpsTransferableHiPriority) ? cursor: NULL; 
}


template <class _VALUE>
bool transporter<_VALUE>::copyFrom(const _Myt& src)
{
	_print(L"\t\t. copyFrom *this", true);
	src._print(L"\t\t. copyFrom  src&", true);

	_Myt* t = _target();
	const _Myt* st = src._target();
	t->release();
	if (!st->isNull())
	{
		if (st->_state == tpsTransferable)
		{
			t->_value = st->_value;
			src._state = tpsTransferred;
		}
		else if (st->_state == tpsTransferred || st->_state == tpsNoTransferable)
			t->_value = new _VALUE(st->_value);
		if (t->isNull() && !t->_next)
			t->_next = st->_next;
		t->_state = t->_state == tpsNULLNoTransferable ? tpsNoTransferable : tpsTransferable;
	}
	else if (st->_state == tpsNULLNoTransferable)
	{
		ASSERT(t->_state == tpsNULL);
		t->_pValue = const_cast<_Myt*>(&src);
		t->_state = tpsTransporter;
		return false;
	}
	return true;
}


template <class _VALUE>
transporter<_VALUE>& transporter<_VALUE>::operator=(const _Myt& right)
{
	if(this != &right)
	{
		copyFrom(right);
	}
	return *this;
}

template <class _VALUE>
transporter<_VALUE>& transporter<_VALUE>::operator=(const _valuePtr right)
{
	_Myt* target = _target();
	if (target->_value != right)
	{
		target->release();
		if (right)
		{
			target->_state = target->_state == tpsNULLNoTransferable ? tpsNoTransferable: tpsTransferable;
			target->_value = right;
		}
	}
	return *this;
}

template <class _VALUE>
transporter<_VALUE>& transporter<_VALUE>::operator=(const _TypeV& right)
{
	return operator=(new VALUE(right));
}


template <class _VALUE>
_VALUE* transporter<_VALUE>::operator*()
{
	_Myt* ptr = _target();
	return !ptr || ptr->isNull() ? NULL: ptr->_value;
}

template <class _VALUE>
_VALUE* const transporter<_VALUE>::operator*() const
{
	const _Myt* ptr = _target();
	return !ptr || ptr->isNull() ? NULL : ptr->_value;
}

template <class _VALUE>
_VALUE* transporter<_VALUE>::operator->()
{
	return operator*();
}

template <class _VALUE>
_VALUE* const transporter<_VALUE>::operator->() const
{
	return operator*();
}

template <class _VALUE>
transporter<_VALUE>& transporter<_VALUE>::operator()(_valuePtr target, _Myt& value)
{
	_Myt* vt = value._target();
	if(target == vt->_value)
		operator=(*vt);
	else
		operator=(target);
	return *this;
}

template <class _VALUE>
transporter<_VALUE>& transporter<_VALUE>::operator()(_valuePtr target, _Myt& left, _Myt& right)
{
	_Myt* lt = left._target(),
		* rt = right._target();
	if (lt->_state == tpsNoTransferable && rt->_state == tpsNoTransferable)
		operator =(new _VALUE(target));
	else if (left._state == tpsTransporter && lt->_state == tpsNULLNoTransferable)
		operator=(left = right);
	else if(!lt->isNull() && target == lt->_value)
		operator=(*lt);
	else if(!rt->isNull() && target == rt->_value)
		operator=(*rt);
	else
		operator=(target);
	return *this;
}


template <class _VALUE>
_VALUE* transporter<_VALUE>::operator[](int idx)
{
	_Myt* cursor = this;
	for(int i = 0; i < idx && cursor; cursor = cursor->_next, ++i)
		;
	return cursor ? cursor->_value: NULL; 
}

template <class _VALUE>
const _VALUE* transporter<_VALUE>::operator[](int idx) const
{
	return const_cast<_Myt*>(this)->operator[](idx);
}

template <class _VALUE>
bool transporter<_VALUE>::haveLeftPriority(const _Myt& left, const _Myt& right) const
{
	const _Myt* lt = left._target(),
		*rt = right._target();
	return (lt->_state == tpsNULLNoTransferable || rt->_state != tpsNULLNoTransferable) && 
		(lt->_state == tpsNoTransferable || lt->_state == tpsTransferable || lt->_state == tpsTransferred || !*rt);
}

/*template <class _VALUE>
bool transporter<_VALUE>::toHiPriority()
{
	bool bRet = _state == tpsTransferable;
	if(bRet)
		_state = tpsTransferableHiPriority;
	return bRet;
}*/


// For debug:
template <class _VALUE>
LPCTSTR transporter<_VALUE>::_getState() const
{
	static LPCTSTR sState[] = {L"tpsNULL", L"tpsNULLNoTransferable", L"tpsTransporter", L"tpsTransferable", L"tpsNoTransferable", L"tpsTransferred"};
	return sState[_state];
}

template <class _VALUE>
COLORREF transporter<_VALUE>::_getStateColor() const
{
	static COLORREF crState[] = { RGB(0x80, 0x80, 0x80), RGB(0x40, 0x40, 0x40), RGB(0xFF, 0x40, 0xFF), RGB(0x00, 0xFF, 0x00), RGB(0xFF, 0x00, 0x00), RGB(0x00, 0x00, 0xFF) };
	return crState[_state];
}

template <class _VALUE>
void transporter<_VALUE>::_print(LPCTSTR head, bool nl) const 
{
	AfxTrace(L"\t%s ", head);
	___print();
	if(nl)
		AfxTrace(L"\n");
}

template <class _VALUE>
void transporter<_VALUE>::___print() const
{
	if(!isNull()) {
		AfxTrace(L"<");
		for(const _Myt* cur = this; cur; cur = cur->_state == tpsTransporter ? cur->_pValue: cur->_next)
		{
			if(cur != this)
				AfxTrace(L" -> ");
			if(cur->_value)
				AfxTrace(L"0x%08X [%s]{0x%08X = %f}", this, _getState(), _value, *_value);
			else
				AfxTrace(L"0x%08X [%s]{0x%08X = 'NULL'}", this, _getState(), NULL);
		}
		AfxTrace(L">");
	}
	else
		AfxTrace(L"<0x%08X [%s]{'NULL'}>", this, _getState());
}


} // namespace parser
} // namespace pmb
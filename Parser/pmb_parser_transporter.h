#pragma once

namespace pmb
{
namespace parser
{


template <class _VALUE>
class transporter
{
public:
	typedef typename _VALUE  VALUE;
	typedef transporter<VALUE> _Myt;
	typedef VALUE* _valuePtr;

protected:
	enum state: char {
		tpsNULL = (char)0,
		tpsNULLNoTransferable,
		tpsTransporter,
		tpsTransferable,
		tpsNoTransferable,
		tpsTransferred
	};

public:
	static _valuePtr getNewValue(const char* str, int len);

protected:
//	explicit transporter(int);
public:
	explicit transporter();
	explicit transporter(bool);
	explicit transporter(_valuePtr value, bool transfeable);
	~transporter();

	void release();

	_Myt* getPlace();

	_Myt& operator=(const _Myt& right);
	_Myt& operator=(const _valuePtr right);

	bool isNull() const;
	bool operator!() const;
	bool isTransferable() const;
	bool isNoTransferable() const;

	_valuePtr operator*();
	_valuePtr const operator*() const;

	_valuePtr operator->();
	_valuePtr const operator->() const;

	_Myt& operator()(_valuePtr target, _Myt& value);
	_Myt& operator()(_valuePtr target, _Myt& left, _Myt& right);

	// low performance
	_VALUE* operator[](int idx);
	const _VALUE* operator[](int idx) const;

//	bool toHiPriority();

	bool haveLeftPriority(const _Myt& left, const _Myt& right) const;

private:
	void _release();
	_Myt* _target();
	bool copyFrom(const _Myt& src);
	const _Myt* _target() const;

	// For Debug:
public:
	LPCTSTR _getState() const;
	COLORREF _getStateColor() const;
	void _print(LPCTSTR head, bool nl = false) const;
private:
	void ___print() const;

	// data members
private:
	mutable state _state;
	union
	{
		_valuePtr _value;
		_Myt* _pValue;
	};
	_Myt*   _next;
};



} // namespace parser
} // namespace pmb	
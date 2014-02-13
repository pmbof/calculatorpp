#pragma once


#include <math.h>

namespace pmb
{
namespace parser
{

template<class _TYPE>
struct number
{
	explicit number(const number<_TYPE>* src) {
		_number = src->_number;
	}

	explicit number(const char* str, int len) {
		_number = 0;
		int ini = 0;
		int base = 10;
		if(2 < len && str[0] == '0') {
			ini = 2;
			switch(str[1]) {
			case 'x':
			case 'X':
				base = 16;
				break;
			case 'o':
			case 'O':
				base = 8;
				break;
			case 'b':
			case 'B':
				base = 2;
				break;
			default:
				ini = 0;
				break;
			}
		}
		bool bDecimal = false;
		_TYPE decimal = 1;
		for(int i = ini; i < len; ++i) {
			if(str[i] == '.')
				bDecimal = true;
			else
				_number = _number * (bDecimal ? 1: base) + (10 < base && 'A' <= str[i] ? str[i] < 'a' ? str[i] - 'A': str[i] - 'a': str[i] - '0') / decimal;
			if(bDecimal)
				decimal = decimal * base;
		}
	}


	void negative() {
		_number = -_number;
	}

	void factorial() {
		long res = 1;
		for(int i = 2; i <= _number; ++i)
			res *= i;
		_number = res;
	}

	void multiplication(const number<_TYPE>& right) {
		_number *= right._number;
	}

	void division(const number<_TYPE>& right) {
		_number /= right._number;
	}

	void modulo(const number<_TYPE>& right) {
		_number = (long int)_number % (long int)right._number;
	}

	void addition(const number<_TYPE>& right) {
		_number += right._number;
	}

	void substraction(const number<_TYPE>& right) {
		_number -= right._number;
	}

	void exponentiation(const number<_TYPE>& right) {
		_number = pow(_number, right._number);
	}

	void root(const number<_TYPE>& right) {
		_number = pow(_number, 1 / right._number);
	}

	_TYPE _number;
};




template <class _TYPE>
class value
{
public:
	typedef _TYPE* tpValuePtr;

protected:
	enum tpValue: char {
		// used for tree nodes unknow, list and parentheses (default constructor)
		tpvUnknowNodeType,
		// used for default constructor, to get lValue and rValue 
		tpvNoType,
		// this have a memory allocator in _ptr
		tpvOriginal,
		// this have a copy in _ptr (no delete)
		tpvCopy,
		// Used to allocate variables
		tpvNoTransitive,
		// When _ptr2 is valid
		tpvTransitive,
		// the _ptr2 is valid but it have a copied pointer
		tpvTransitiveCopy,
		// Used for initialize variable, but not allocate memory!
		tpvInitialVariable
	};

public:
	static _TYPE* getNewValue(const char* str, int len) {
		return new _TYPE(str, len);
	}

private:
	explicit value(tpValue type): _type(type) { _print(_T("+ construct.1 new value"), true); }

public:
	value(): _type(tpvInitialVariable) { _print(_T("\t+ construct.2 new value"), true); }
	explicit value(bool): _type(tpvUnknowNodeType) { _print(_T("\t+ construct.3 new value"), true);  }
	explicit value(int): _type(tpvNoType) { _print(_T("\t+ construct.4 new value"), true);  }
	value(_TYPE* ptr, bool bOriginal) : _ptr(ptr), _type(bOriginal ? tpvOriginal: tpvNoTransitive) { _print(_T("\t+ construct.5 new value"), true); }
	value(value<_TYPE>* ptr2) : _ptr2(ptr2), _type(tpvTransitive) { _print(_T("\t+ construct.6 new value"), true); }

	~value() {
		_print(_T("- destruct value"), false, false);
		free();
	}

	value<_TYPE>& operator=(_TYPE* ptr) {
		_print(_T("\t- Assignation operator= _TYPE* ptr"), true);
		free();
		_ptr = ptr;
		_type = tpvOriginal;
		_print(_T("\t+ Assignation operator= _TYPE* ptr"), true);
		return *this;
	}

	value<_TYPE>& operator=(value<_TYPE>& src) {
			_print(_T("\t- Assignation copy operator= *this"), true);
		src._print(_T("\t- Assignation copy operator=  src&"), true);
		if(src._ptr2 == this) {
			_print(L"\tERROR, recursive reference assignation operator= -> ", true);
			return *this;
		}
		if(_type == tpvUnknowNodeType)
			copyFrom(src);
		else {
			bool bElse = true;
			if(_type == tpvTransitive) {
				value<_TYPE>* s;
				for(s = _ptr2; s->_type == tpvTransitive; s = s->_ptr2)
					;
				if(s->_type == tpvInitialVariable) {
					s->_type = tpvNoTransitive;
					src._type = tpvCopy;
					s->_ptr = src._ptr;
					bElse = false;
				}
			}
			if(bElse) {
				free();
				_ptr2 = &src;
				_type = tpvTransitive;
			}
		}
			_print(_T("\t+ Assignation copy operator= *this"), true);
		src._print(_T("\t+ Assignation copy operator=  src&"), true);
		return *this;
	}

	bool operator!() const {
		return _type == tpvNoType || _type == tpvUnknowNodeType || _type == tpvTransitive && !get() || !_ptr;
	}

	_TYPE* get() const {
		const value<_TYPE>* pv;
		for(pv = this; pv && pv->_type == tpvTransitive || pv->_type == tpvTransitiveCopy; pv = pv->_ptr2)
			;
		return pv && pv->_type == tpvOriginal || pv->_type == tpvNoTransitive || pv->_type == tpvCopy ? pv->_ptr: NULL;
	}



	// Operations:
	void negative(value<_TYPE>& right) {
		copyFrom(right);
		_ptr->negative();
	}

	void factorial(value<_TYPE>& left) {
		copyFrom(left);
		_ptr->factorial();
	}

	void multiplication(value<_TYPE>& left, value<_TYPE>& right) {
		_ptr->multiplication(*copyFrom(left, right));
	}

	void division(value<_TYPE>& left, value<_TYPE>& right) {
		_ptr->division(*copyFrom(left, right));
	}

	void modulo(value<_TYPE>& left, value<_TYPE>& right) {
		_ptr->modulo(*copyFrom(left, right));
	}

	void addition(value<_TYPE>& left, value<_TYPE>& right) {
		_ptr->addition(*copyFrom(left, right));
	}

	void substraction(value<_TYPE>& left, value<_TYPE>& right) {
		_ptr->substraction(*copyFrom(left, right));
	}

	void exponentiation(value<_TYPE>& left, value<_TYPE>& right) {
		_ptr->exponentiation(*copyFrom(left, right));
	}

	void root(value<_TYPE>& left, value<_TYPE>& right) {
		_ptr->root(*copyFrom(left, right));
	}

	// For debug:
	LPCTSTR _getType() const {
		return _type == tpvUnknowNodeType ? L"tpvUnknowNodeType": _type == tpvNoType ? L"tpvNoType": _type == tpvOriginal ? L"tpvOriginal": 
			_type == tpvCopy ? L"tpvCopy": _type == tpvNoTransitive ? L"tpvNoTransitive": _type == tpvTransitiveCopy ? L"tpvTransitiveCopy": 
			_type == tpvInitialVariable ? L"tpvInitialVariable": L"tpvTransitive";
	}

	COLORREF _getTypeColor() const {
		return	_type == tpvOriginal ? RGB(0xFF, 0x00, 0x00):
				_type == tpvNoType ? RGB(0x80, 0x80, 0x80):
				_type == tpvCopy ? RGB(0x00, 0xFF, 0x00):
				_type == tpvNoTransitive ? RGB(0x00, 0x00, 0xFF):
				_type == tpvInitialVariable ? RGB(0x00, 0xFF, 0xFF):
								RGB(0xFF, 0xFF, 0x00);
	}

	void _print(LPCTSTR head, bool nl = false, bool recursive = true) const {
		AfxTrace(L"\t%s ", head);
		___print(recursive);
		if(nl)
			AfxTrace(L"\n");
	}


protected:
	void free() {
		if((_type == tpvOriginal || _type == tpvNoTransitive) && _ptr) {
			AfxTrace(_T("\t- deleting address memory [0x%08X] = %f\n"), _ptr, *_ptr);
			delete _ptr;
		}
		else
			AfxTrace(L"\n");
	}

	bool copyFrom(value<_TYPE>& src, bool force = true) {
			_print(L"\t\t. copyFrom *this", true);
		src._print(L"\t\t. copyFrom  src&", true);
		if(src._type == tpvOriginal) {
			free();
			_type = tpvOriginal;
			_ptr = src.get();
			src._type = tpvCopy;
			return true;
		} else if(src._type == tpvTransitive) {
			free();
			value<_TYPE>* s;
			for(s = src._ptr2; s->_type == tpvTransitive; s = s->_ptr2)
				;
			s->_print(L"\t\t\t. copyFrom  s&", true);
			if(s->_type == tpvNoTransitive) {
				_type = tpvOriginal;
				_ptr = new _TYPE(s->get());
			} else if(s->_type == tpvInitialVariable) {
				_type = tpvTransitive;
				_ptr2 = s;
			} else {
				_type = tpvOriginal;
				_ptr = s->get();
				s->_type = tpvCopy;
				//src._type = tpvTransitiveCopy;
			}
			return true;
		} else if(src._type == tpvInitialVariable) {
			free();
			_type = src._type;
			return true;
		} else if(force) {
			if(src._type == tpvNoTransitive || src._type == tpvCopy) {
				free();
				_ptr = new _TYPE(src.get());
				_type = tpvOriginal;
			}
		}
		return false;
	}


	_TYPE* copyFrom(value<_TYPE>& lSrc, value<_TYPE>& rSrc) {
		if(copyFrom(lSrc, false))
			return rSrc.get();
		copyFrom(rSrc, true);
		return lSrc.get();
	}


	// for Debug:
	void ___print(bool recursive) const {
		if(_type != tpvNoType && _type != tpvUnknowNodeType && _type != tpvInitialVariable) {
			if(_type == tpvOriginal || _type == tpvCopy || _type == tpvNoTransitive) {
				if(get())
					AfxTrace(L"<0x%08X [%s]{0x%08X = %f}>", this, _getType(), get(), *get());
				else
					AfxTrace(L"<0x%08X [%s]{0x%08X = 'NULL'}>", this, _getType(), NULL);
			} else if(recursive) {
				AfxTrace(L"<0x%08X [%s]{0x%08X -> ", this, _getType(), _ptr2);
				_ptr2->___print(recursive);
				AfxTrace(L"> ");
			} else {
				AfxTrace(L"<0x%08X [%s]{0x%08X ...}> ", this, _getType(), _ptr2);
			}

		}
		else
			AfxTrace(L"<0x%08X [%s]{'NULL'}>", this, _getType());
	}

private:
	tpValue _type;
	union {
		tpValuePtr _ptr;
		value<_TYPE>* _ptr2;
	};
};






template<class _TVALUE>
struct operations
{
	void positive(_TVALUE& result, _TVALUE& value) {
		result = value;
	}

	void negative(_TVALUE& result, _TVALUE& value) {
		result.negative(value);
	}

	void factorial(_TVALUE& result, _TVALUE& value) {
		result.factorial(value);
	}

	void exponentiation(_TVALUE& result, _TVALUE& left, _TVALUE& right) {
		result.exponentiation(left, right);
	}

	void root(_TVALUE& result, _TVALUE& left, _TVALUE& right) {
		result.root(left, right);		
	}

	void multiplication(_TVALUE& result, _TVALUE& left, _TVALUE& right) {
		result.multiplication(left, right);
	}

	void division(_TVALUE& result, _TVALUE& left, _TVALUE& right) {
		result.division(left, right);
	}
	
	void modulo(_TVALUE& result, _TVALUE& left, _TVALUE& right) {
		result.modulo(left, right);
	}

	void addition(_TVALUE& result, _TVALUE& left, _TVALUE& right) {
		result.addition(left, right);		
	}
	
	void substraction(_TVALUE& result, _TVALUE& left, _TVALUE& right) {
		result.substraction(left, right);
	}

	void assignation(_TVALUE& result, _TVALUE& left, _TVALUE& right) {
		result = right;
		left = result;
	}

	void result(_TVALUE& result, _TVALUE& value) {
		
	}
};




template<class _TVALUE>
struct operation
{
	typedef void (operations<_TVALUE>::*tpOprFunc1)(_TVALUE& result, _TVALUE& value);
	typedef void (operations<_TVALUE>::*tpOprFunc2)(_TVALUE& result, _TVALUE& left, _TVALUE& right);

public:
	operation(const char* symbol, int precedence, bool leftToRight, const char* name, const char* description,
						tpOprFunc2 oprFunc2, bool canCallFunction = false)
				: _precedence(precedence), _leftToRight(leftToRight), _binary(true), _oprFunc2(oprFunc2), _canCallFunction(canCallFunction), 
					_canCreateLVariable(false), _canCreateRVariable(false) {
		_symbol = new char[(_len = strlen(symbol)) + 1];
		strcpy(_symbol, symbol);
		_name = new char[strlen(name) + 1];
		strcpy(_name, name);
		_description = new char[strlen(description) + 1];
		strcpy(_description, description);
	}
	operation(const char* symbol, int precedence, bool leftToRight, const char* name, const char* description,
						tpOprFunc2 oprFunc2, bool canCreateLVariable, bool canCreateRVariable)
				: _precedence(precedence), _leftToRight(leftToRight), _binary(true), _oprFunc2(oprFunc2), _canCallFunction(false),
					_canCreateLVariable(canCreateLVariable), _canCreateRVariable(canCreateRVariable) {
		_symbol = new char[(_len = strlen(symbol)) + 1];
		strcpy(_symbol, symbol);
		_name = new char[strlen(name) + 1];
		strcpy(_name, name);
		_description = new char[strlen(description) + 1];
		strcpy(_description, description);
	}
	operation(const char* symbol, int precedence, bool leftToRight, const char* name, const char* description,
						tpOprFunc1 oprFunc1)
				: _precedence(precedence), _leftToRight(leftToRight), _binary(false), _oprFunc1(oprFunc1), _canCreateLVariable(false), _canCreateRVariable(false),
					 _canCallFunction(false) {
		_symbol = new char[(_len = strlen(symbol)) + 1];
		strcpy(_symbol, symbol);
		_name = new char[strlen(name) + 1];
		strcpy(_name, name);
		_description = new char[strlen(description) + 1];
		strcpy(_description, description);
	}

	~operation() {
		delete [] _symbol;
		if(_name)
			delete [] _name;
		if(_description)
			delete [] _description;
	}


	bool compare(const char* expr, int len) const {
		return _len == len && !strncmp(_symbol, expr, len);
	}

	bool operator<(const operation<_TVALUE>& opr) const {
		return _precedence < opr._precedence;
	}

	bool isFirstLeft() const {
		return _binary && _leftToRight || !_binary && !_leftToRight;
	}

	bool isFirstRight() const {
		return _binary && !_leftToRight || !_binary && _leftToRight;
	}

	bool canCallFunction() const {
		return _canCallFunction;
	}


	const char* getSymbol() const {
		return _symbol;
	}
	const char* getName() const {
		return _name;
	}
	const char* getDescription() const {
		return _description;
	}
	bool isLeftToRight() const {
		return _leftToRight;
	}
	bool isBinary() const {
		return _binary;
	}
	bool canCreateLVariable() const {
		return _canCreateLVariable;
	}
	bool canCreateRVariable() const {
		return _canCreateRVariable;
	}
	int getPrecedence() const {
		return _precedence;
	}

	unsigned int getFunctor1() const {
		return (int)(void*&)_oprFunc1;
	}
	unsigned int getFunctor2() const {
		return (int)(void*&)_oprFunc2;
	}

	void operator()(_TVALUE& result, _TVALUE& value) const {
		operations<_TVALUE> opr;
		(opr.*_oprFunc1)(result, value);
	}

	void operator()(_TVALUE& result, _TVALUE& left, _TVALUE& right) const {
		operations<_TVALUE> opr;
		(opr.*_oprFunc2)(result, left, right);
	}

private:
	tpOprFunc1 _oprFunc1;
	tpOprFunc2 _oprFunc2;
	char*	_symbol;
	int		_len;
	int		_precedence;
	bool	_leftToRight;
	bool	_binary;
	bool	_canCallFunction;
	bool	_canCreateLVariable;
	bool	_canCreateRVariable;
	char*	_name;
	char*	_description;
};



template<class _TVALUE>
class operation_table 
{
public:

	const operation<_TVALUE>* find(const node<_TVALUE>* nd, const char* expr) const;

	const operation<_TVALUE>* get(int i) const;
	int size() const;

protected:
	static const operation<_TVALUE> _opr[];
	static const int _oprSize;
};

}
}
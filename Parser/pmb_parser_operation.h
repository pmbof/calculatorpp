#pragma once


#include <math.h>

namespace pmb
{
namespace parser
{

template<class _TYPE>
struct number
{
public:
	typedef number<_TYPE> _MyT;

public:
	explicit number(const _TYPE& src) : _number(src) { }
	explicit number(const _MyT* src) : _number(src->_number) { }

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

	void positive(const _MyT& right) {
		_number = +right._number;
	}

	void negative(const _MyT& right) {
		_number = -right._number;
	}

	void factorial(const _MyT& right) {
		long res = 1;
		for(int i = 2; i <= right._number; ++i)
			res *= i;
		_number = res;
	}

	void multiplication(const _MyT& left, const _MyT& right) {
		_number = left._number * right._number;
	}

	void division(const _MyT& left, const _MyT& right) {
		_number = left._number / right._number;
	}

	void modulo(const _MyT& left, const _MyT& right) {
		_number = (long int)left._number % (long int)right._number;
	}

	void addition(const _MyT& left, const _MyT& right) {
		_number = left._number + right._number;
	}

	void substraction(const _MyT& left, const _MyT& right) {
		_number = left._number - right._number;
	}

	void exponentiation(const _MyT& left, const _MyT& right) {
		_number = pow(left._number, right._number);
	}

	void root(const _MyT& left, const _MyT& right) {
		_number = pow(left._number, 1 / right._number);
	}

	void assignation(const _MyT& left, const _MyT& right) {
		_number = right._number;
	}

	void result(const _MyT& right) {
	}

	number<_TYPE>* place(bool leftPriority, _MyT* left, _MyT* right)
	{
		return leftPriority && left || !leftPriority && !right ? left: right;
	}

	number<_TYPE>* assignation_place(bool leftPriority, _MyT* left, _MyT* right)
	{
		return right;
	}

	_TYPE _number;
};





template<class _TVALUE>
struct operation
{
	typedef typename _TVALUE::VALUE VALUE;
	typedef VALUE* (VALUE::*tpOprPlace)(bool leftPriority, VALUE* left, VALUE* right);
	typedef void (VALUE::*tpOprFunc1)(const VALUE& value);
	typedef void (VALUE::*tpOprFunc2)(const VALUE& left, const VALUE& right);

public:
	operation(const char* symbol, int precedence, bool leftToRight, const char* name, const char* description, tpOprFunc2 oprFunc2, tpOprPlace oprPlace, bool canCallFunction = false);
	operation(const char* symbol, int precedence, bool leftToRight, const char* name, const char* description, tpOprFunc2 oprFunc2, tpOprPlace oprPlace, bool canCreateLVariable, 
		bool canCreateRVariable);
	operation(const char* symbol, int precedence, bool leftToRight, const char* name, const char* description, tpOprFunc1 oprFunc1, tpOprPlace oprPlace);

	~operation();

	bool compare(const char* expr, int len) const;

	bool operator<(const operation<_TVALUE>& opr) const;

	bool operator<(int rPrecedence) const;

	bool isFirstLeft() const;

	bool isFirstRight() const;

	bool canCallFunction() const;


	const char* getSymbol() const;
	const char* getName() const;
	const char* getDescription() const;
	bool isLeftToRight() const;
	bool isBinary() const;
	bool canCreateLVariable() const;
	bool canCreateRVariable() const;
	int getPrecedence() const;

	unsigned int getFunctor1() const;
	unsigned int getFunctor2() const;

	void operator()(_TVALUE& result, _TVALUE& value) const;

	void operator()(_TVALUE& result, _TVALUE& left, _TVALUE& right) const;

private:
	tpOprFunc1 _oprFunc1;
	tpOprFunc2 _oprFunc2;
	tpOprPlace _oprPlace;
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

	template<class _TVALUE>
	friend bool operator< (int lPrecedence, const operation<_TVALUE>& opr);
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
#pragma once


#include <math.h>

namespace pmb
{
namespace parser
{




template<class _TVALUE>
struct operation
{
	typename typedef _TVALUE transporter_args;
	typedef typename _TVALUE::tpValue VALUE;
	typedef void(*tpFunc)(transporter_args& args);


	class exception
	{
	public:
		exception(const char* msg) : _msg(msg) {}

		const char* message() const { return _msg.c_str(); }
	protected:
		std::string _msg;
	};
public:
	operation(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction = false, bool canCreateLVariable = false, bool canCreateRVariable = false);

	~operation();

	bool compare(const char* expr, int len) const;

	bool operator<(const operation<_TVALUE>& opr) const;

	bool operator<(int rPrecedence) const;

	bool isFirstLeft() const;

	bool isFirstRight() const;

	bool canCallFunction() const;


	const char* getSymbol() const;
	int getSymbolLen() const;
	const char* getName() const;
	const char* getDescription() const;
	bool isLeftToRight() const;
	bool isBinary() const;
	bool canCreateLVariable() const;
	bool canCreateRVariable() const;
	int getPrecedence() const;

	unsigned int getFunctor1() const;
	unsigned int getFunctor2() const;

	void operator()(_TVALUE& values) const;

private:
	tpFunc _fnc;
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




template<class _OPR, class _NODE>
class operation_table 
{
public:
	typedef typename _OPR operation;
	typedef unsigned short size_t;

public:
	operation_table(const operation* opr, size_t size);

	const operation* find(const _NODE* nd, const char* expr) const;

	const operation* get(size_t i) const;
	size_t size() const;

protected:
	const operation* _opr;
	size_t _oprSize;
};



}
}
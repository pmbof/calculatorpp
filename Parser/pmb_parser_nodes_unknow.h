#pragma once

#include "pmb_parser_nodes_calc.h"
#include "pmb_parser_operation.h"
#include "pmb_parser_function.h"


namespace pmb
{
namespace parser
{
namespace nodes
{


template<class _TVALUE>
class unknow: public calc<_TVALUE>, protected _TVALUE
{
public:
	typedef typename _TVALUE tvalue_type;

public:
	unknow(int ini, int end);
	~unknow();

	const operation<_TVALUE>* getOperation() const;
	const function<_TVALUE>* getFunction() const;

	int getPrecedence() const;

	void setOperation(const operation<_TVALUE>* opr, const function<_TVALUE>* fnc);

	const _TVALUE& getValue() const;
	_TVALUE& getValue();

	const _TVALUE* getArguments() const;

protected:
	const operation<_TVALUE>* _opr;
	const function<_TVALUE>* _fnc;
};




}
}
}
#pragma once

#include "pmb_parser_nodes_calc.h"
#include "pmb_parser_operation.h"



namespace pmb
{
namespace parser
{
namespace nodes
{


template<class _TVALUE>
class unknow: public calc<_TVALUE>
{
public:
	unknow(int ini, int end);
	~unknow();

	const operation<_TVALUE>* getOperation() const;

	void setOperation(const operation<_TVALUE>* opr);

	const _TVALUE& getValue() const;
	_TVALUE& getValue();

protected:
	const operation<_TVALUE>* _opr;
	_TVALUE _value;
};




}
}
}
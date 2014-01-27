#pragma once

#include "pmb_parser_node.h"

namespace pmb
{
namespace parser
{
namespace nodes
{



template<class _TVALUE>
class calc: public node<_TVALUE>
{
protected:
	calc(ndtype type, int ini, int end);

public:
	~calc();

	bool isCalculated() const;
	bool isVariableDependent() const;

	void setCalculated(bool bCalculated = true);
	void setVariableDependent(bool isVariableDependet = true);

	static bool isNotCalculated(const node* nd);

	calc<_TVALUE>* nextCalc();

protected:
	bool _bCalculated;
	bool _isVariableDependent;
};



}
}
}
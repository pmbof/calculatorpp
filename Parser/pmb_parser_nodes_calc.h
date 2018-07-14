#pragma once

#include "pmb_parser_node.h"

namespace pmb
{
namespace parser
{
namespace nodes
{



template<class _ITEM, class _NDTYPE>
class calc: public node<_ITEM, _NDTYPE>
{
public:
	typedef typename node<_ITEM, _NDTYPE> cnode;

protected:
	calc(typename cnode::cNdType type, int ini, int end);

public:
	~calc();

	const calc* firstCalc() const;
	const calc* nextCalc() const;
};



}
}
}
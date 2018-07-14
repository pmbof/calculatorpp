#pragma once

#include "pmb_parser_node.h"

namespace pmb
{
namespace parser
{
namespace nodes
{



template<class _ITEM, class _NDTYPE>
class number: public node<_ITEM, _NDTYPE>
{
public:
	typedef typename node<_ITEM, _NDTYPE> cnode;

public:
	number(int ini, int end);
	~number();

protected:
};



}
}
}
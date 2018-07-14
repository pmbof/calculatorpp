#include "StdAfx.h"

#include "pmb_parser_nodes_number.h"

namespace pmb
{
namespace parser
{
namespace nodes
{



template<class _ITEM, class _NDTYPE>
number<_ITEM, _NDTYPE>::number(int ini, int end)
	: node(ndNumber, ini, end)
{
	//*static_cast<_TVALUE*>(this) = getNewValue(expr + ini, end - ini);
};

template<class _ITEM, class _NDTYPE>
number<_ITEM, _NDTYPE>::~number()
{
};


}
}
}
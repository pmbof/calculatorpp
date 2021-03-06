#pragma once
#include "pmb_parser_nodes_calc.h"

namespace pmb
{
namespace parser
{
namespace nodes
{


template<class _ITEM, class _NDTYPE>
class parentheses : public calc<_ITEM, _NDTYPE>
{
public:
	parentheses(int ini, int end, char type, int opened);
	~parentheses();

	inline short getOpened() const;

	inline bool isThisType(const cnode* nd) const;

	char getClass() const;

	int countListChildNodes() const;

	parentheses* split(unsigned short count);
	parentheses* split_right(unsigned short count);

protected:
	char _prtype;
	short _opened;
};


}
}
}
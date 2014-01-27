#pragma once
#include "pmb_parser_nodes_calc.h"

namespace pmb
{
namespace parser
{
namespace nodes
{


template<class _TVALUE>
class parentheses: public calc<_TVALUE>
{
public:
	parentheses(int ini, int end, char type, int opened);
	~parentheses();

	inline int getOpened() const {
		return _opened;
	}

	inline bool isThisType(const node* nd) const {
		const parentheses* pr = static_cast<const parentheses*>(nd);
		return nd && pr->_type == ndParentheses && pr->_prtype == _prtype;
	}

	inline char getClass() const {
		return _prtype;
	}

	int countListChildNodes() const;


	const _TVALUE& getValue() const;
	_TVALUE& getValue();

protected:
	char _prtype;
	int _opened;

	_TVALUE _value;
};


}
}
}
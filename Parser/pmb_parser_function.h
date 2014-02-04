#pragma once

#include "pmb_parser_node.h"


namespace pmb
{
namespace parser
{







template<class _TVALUE>
class function
{
	typedef void (functions<_TVALUE>::*tpFunc)(_TVALUE&, const node<_TVALUE>*);

public:
	function(const char* name, const char* description, tpFunc func, int nArgs);
	~function();

	bool compare(const char* expr, int len) const;


private:
	char* _name;
	int _len;
	char* _symbol;
	tpFunc _func;
	int _nArgs;
};



}
}
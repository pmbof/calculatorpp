#pragma once

#include "pmb_parser_node.h"

namespace pmb
{
namespace parser
{



template<class _TVALUE>
class tree
{
public:
	tree();
	~tree();

	void insert(node<_TVALUE>* newNode);


	const node<_TVALUE>* getRootNode() const;

	node<_TVALUE>* getRootNode();

protected:
	node<_TVALUE>* _root;
	node<_TVALUE>* _cursor;
};


}
}
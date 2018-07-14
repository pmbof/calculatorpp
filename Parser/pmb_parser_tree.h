#pragma once

#include "pmb_parser_node.h"
#include "pmb_parser_nodes_unknow.h"


namespace pmb
{
namespace parser
{




template <class _ITEM, class _NDTYPE>
class tree
{
public:
	typedef typename _ITEM cItem;
	typedef typename _NDTYPE cNdType;
	typedef typename node<cItem, cNdType> cnode;

public:
	tree();
	~tree();

	void insert(cnode* newNode);


	const cnode* getRootNode() const;

	cnode* getRootNode();

	void check() const;

#ifdef DEBUG
	void trace(const char* expr) const;
#endif // DEBUG

protected:
	cnode* _root;
	cnode* _cursor;
};


}
}
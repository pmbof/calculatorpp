
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::node_space::node_space(bnode* parent, const tnode* nd)
	: node(parent, nd)
{
}




inline CParserView::line::bnodetypes CParserView::line::node_space::type() const
{
	return bndSpace;
}


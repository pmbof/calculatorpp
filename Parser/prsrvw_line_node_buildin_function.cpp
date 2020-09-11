
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::node_buildin_function::node_buildin_function(bnode* parent, const tnode* nd)
	: node_alpha(parent, nd)
{
}



inline CParserView::line::bnodetypes CParserView::line::node_buildin_function::type() const
{
	return bndBIFunction;
}

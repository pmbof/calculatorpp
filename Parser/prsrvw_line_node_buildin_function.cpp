
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::node_buildin_function::node_buildin_function(const tnode* nd, bnode* parent)
	: node_alpha(nd, parent)
{
}



inline CParserView::line::bnodetypes CParserView::line::node_buildin_function::type() const
{
	return bndBIFunction;
}

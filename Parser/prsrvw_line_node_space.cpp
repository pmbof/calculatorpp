
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::node_space::node_space(const tnode* nd, bnode* parent)
	: node(nd, parent)
{
}




inline CParserView::line::bnodetypes CParserView::line::node_space::type() const
{
	return bndSpace;
}



#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::node_space::node_space(const tnode* nd)
	: node(nd)
{
}




CParserView::line::bnodetypes CParserView::line::node_space::type() const
{
	return bndSpace;
}


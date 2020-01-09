
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::node_number::node_number(bnode* parent, const tnode* nd)
	: node(parent, nd)
{
}




CParserView::line::bnodetypes CParserView::line::node_number::type() const
{
	return bndNumber;
}


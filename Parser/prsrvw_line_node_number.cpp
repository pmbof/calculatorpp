
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::node_number::node_number(const tnode* nd)
	: node(nd)
{
}




CParserView::line::bnodetypes CParserView::line::node_number::type() const
{
	return bndNumber;
}


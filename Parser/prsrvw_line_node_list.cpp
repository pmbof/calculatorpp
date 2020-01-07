
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::node_list::node_list(const tnode* nd)
	: node(nd)
{
}




CParserView::line::bnodetypes CParserView::line::node_list::type() const
{
	return bndList;
}
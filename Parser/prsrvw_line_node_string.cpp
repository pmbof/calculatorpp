
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::node_string::node_string(const tnode* nd)
	: node(nd)
{
}





CParserView::line::bnodetypes CParserView::line::node_string::type() const
{
	return bndString;
}



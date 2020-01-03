
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


void CParserView::line::sdraw::begin_expr(const bnode* pnd)
{
	pline->_swith_expr_begin(pnd, this);
}




void CParserView::line::sdraw::end_expr(const bnode* pnd)
{
	pline->_swith_expr_end(pnd, this);
}

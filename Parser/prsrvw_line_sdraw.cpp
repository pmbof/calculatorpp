
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes




CParserView::line::sbase::sbase(line* pl, CDC* pdc, const char* pExpr, bool bediting)
	: pline(pl), pDC(pdc), pstr(pExpr), index(0), bEditing(bediting)
{
}






CParserView::line::sset::sset(line* pl, const tnode* pnd, CDC* pdc, const char* pExpr, bool bediting)
	: sbase(pl, pdc, pExpr, bediting), nd(pnd)
{
}












CParserView::line::sdraw::sdraw(line* pl, const bnode* pbnd, CDC* pdc, const char* pExpr, bool bediting, const char* pExpr2)
	: sbase(pl, pdc, pExpr, bediting), pnd(pbnd), postr(pExpr2)
{
}




void CParserView::line::sdraw::begin_expr(const bnode* pnd)
{
	pline->_swith_expr_begin(pnd, this);
}




void CParserView::line::sdraw::end_expr(const bnode* pnd)
{
	pline->_swith_expr_end(pnd, this);
}

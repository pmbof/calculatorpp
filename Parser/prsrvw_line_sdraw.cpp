
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes




CParserView::line::sbase::sbase(line* pl, CDC* pdc, const char* pExpr, bool bediting)
	: pline(pl), pDC(pdc), pstr(pExpr), index(0), bEditing(bediting)
{
}






CParserView::line::sset::sset(line* pl, const tnode* ptnd, CDC* pdc, const char* pExpr, bool bediting)
	: sbase(pl, pdc, pExpr, bediting), tnd(ptnd)
{
}












CParserView::line::sdraw::sdraw(line* pl, const bnode* pbnd, CDC* pdc, const char* pExpr, bool bediting, item::SIZETP carpos, const char* pExpr2)
	: sbase(pl, pdc, pExpr, bediting), pnd(pbnd), _carpos(carpos), postr(pExpr2)
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

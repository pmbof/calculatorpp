
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#include "pmb_parser_operation.cpp"
#pragma endregion includes



CParserView::operation::operation(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction, bool canCreateLVariable, bool canCreateRVariable)
	: ::operation(symbol, precedence, leftToRight, binary, name, description, func, canCallFunction, canCreateLVariable, canCreateRVariable) 
{
}


CParserView::operation::operation(const char* symbol, int precedence, bool leftToRight, const char* name, const char* description, tpFunc func, tpFuncCheck funcCheck, bool canCallFunction, bool canCreateLVariable, bool canCreateRVariable)
	: ::operation(symbol, precedence, leftToRight, name, description, func, funcCheck, canCallFunction, canCreateLVariable, canCreateRVariable)
{
}


CParserView::operation::~operation() 
{
}


CParserView::line::node* CParserView::operation::new_instance(line::bnode* parent, const tnode* nd) const
{
	return new line::node_unknown(nd, parent);
}










CParserView::opr_equal::opr_equal(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction, bool canCreateLVariable, bool canCreateRVariable)
	: operation(symbol, precedence, leftToRight, binary, name, description, func, canCallFunction, canCreateLVariable, canCreateRVariable) 
{
}


CParserView::line::node* CParserView::opr_equal::new_instance(line::bnode* parent, const tnode* nd) const
{
	return new line::node_operator_equal(nd, parent);
}










CParserView::opr_result::opr_result(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction, bool canCreateLVariable, bool canCreateRVariable)
	: operation(symbol, precedence, leftToRight, binary, name, description, func, canCallFunction, canCreateLVariable, canCreateRVariable) 
{
}



CParserView::line::node* CParserView::opr_result::new_instance(line::bnode* parent, const tnode* nd) const
{
	return new line::node_operator_result(nd, parent);
}








CParserView::opr_product::opr_product(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction, bool canCreateLVariable, bool canCreateRVariable)
	: operation(symbol, precedence, leftToRight, binary, name, description, func, canCallFunction, canCreateLVariable, canCreateRVariable)
{
}


CParserView::line::node* CParserView::opr_product::new_instance(line::bnode* parent, const tnode* nd) const
{
	return new line::node_operator_product(nd, parent);
}










CParserView::opr_plus::opr_plus(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction, bool canCreateLVariable, bool canCreateRVariable)
	: operation(symbol, precedence, leftToRight, binary, name, description, func, canCallFunction, canCreateLVariable, canCreateRVariable)
{
}


CParserView::line::node* CParserView::opr_plus::new_instance(line::bnode* parent, const tnode* nd) const
{
	return new line::node_operator_plus(nd, parent);
}









CParserView::opr_minus::opr_minus(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction, bool canCreateLVariable, bool canCreateRVariable)
	: operation(symbol, precedence, leftToRight, binary, name, description, func, canCallFunction, canCreateLVariable, canCreateRVariable)
{
}


CParserView::line::node* CParserView::opr_minus::new_instance(line::bnode* parent, const tnode* nd) const
{
	return new line::node_operator_minus(nd, parent);
}












CParserView::opr_division_inline::opr_division_inline(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction, bool canCreateLVariable, bool canCreateRVariable)
	: operation(symbol, precedence, leftToRight, binary, name, description, func, canCallFunction, canCreateLVariable, canCreateRVariable)
{
}


CParserView::line::node* CParserView::opr_division_inline::new_instance(line::bnode* parent, const tnode* nd) const
{
	return new line::node_operator_division_inline(nd, parent);
}








CParserView::opr_division::opr_division(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction, bool canCreateLVariable, bool canCreateRVariable)
	: operation(symbol, precedence, leftToRight, binary, name, description, func, canCallFunction, canCreateLVariable, canCreateRVariable)
{
}


CParserView::line::node* CParserView::opr_division::new_instance(line::bnode* parent, const tnode* nd) const
{
	return new line::node_operator_division(nd, parent);
}








CParserView::opr_power::opr_power(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction, bool canCreateLVariable, bool canCreateRVariable)
	: operation(symbol, precedence, leftToRight, binary, name, description, func, canCallFunction, canCreateLVariable, canCreateRVariable)
{
}


CParserView::line::node* CParserView::opr_power::new_instance(line::bnode* parent, const tnode* nd) const
{
	return new line::node_operator_power(nd, parent);
}










CParserView::opr_root::opr_root(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction, bool canCreateLVariable, bool canCreateRVariable)
	: operation(symbol, precedence, leftToRight, binary, name, description, func, canCallFunction, canCreateLVariable, canCreateRVariable)
{
}


CParserView::line::node* CParserView::opr_root::new_instance(line::bnode* parent, const tnode* nd) const
{
	return new line::node_operator_root(nd, parent);
}











CParserView::opr_not::opr_not(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction, bool canCreateLVariable, bool canCreateRVariable)
	: operation(symbol, precedence, leftToRight, binary, name, description, func, canCallFunction, canCreateLVariable, canCreateRVariable)
{
}


CParserView::line::node* CParserView::opr_not::new_instance(line::bnode* parent, const tnode* nd) const
{
	return new line::node_operator_not(nd, parent);
}




















CParserView::opr_and::opr_and(const char* symbol, int precedence, bool leftToRight, const char* name, const char* description, tpFunc func, tpFuncCheck funcCheck, bool canCallFunction, bool canCreateLVariable, bool canCreateRVariable)
	: operation(symbol, precedence, leftToRight, name, description, func, funcCheck, canCallFunction, canCreateLVariable, canCreateRVariable)
{
}



CParserView::line::node* CParserView::opr_and::new_instance(line::bnode* parent, const tnode* nd) const
{
	return new line::node_operator_and(nd, parent);
}


























CParserView::opr_or::opr_or(const char* symbol, int precedence, bool leftToRight, const char* name, const char* description, tpFunc func, tpFuncCheck funcCheck, bool canCallFunction, bool canCreateLVariable, bool canCreateRVariable)
	: operation(symbol, precedence, leftToRight, name, description, func, funcCheck, canCallFunction, canCreateLVariable, canCreateRVariable)
{
}



CParserView::line::node* CParserView::opr_or::new_instance(line::bnode* parent, const tnode* nd) const
{
	return new line::node_operator_or(nd, parent);
}

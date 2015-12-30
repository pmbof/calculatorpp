#pragma once

#include "pmb_parser_item.h"



namespace pmb
{
namespace parser
{

enum ndtype: char
{
	ndSpace			= 'S',
	ndAlpha			= 'A',
	ndNumber		= 'N',
	ndString		= 's',
	ndList			= 'L',
	ndParentheses	= 'P',
	ndUnknow		= 'U'
};




template <class _TVALUE>
class node: public item
{
protected:
	node(ndtype type, int ini, int end);

public:
	~node();

	static node* newNodeSpace(int ini, int end);
	static node* newNodeAlpha(int ini, int end);
	static node* newNodeNumber(int ini, int end);
	static node* newNodeString(int ini, int end);
	static node* newNodeParentheses(int ini, int end, char type, int opened);
	static node* newNodeList(int end);
	static node* newNodeUnknow(int ini, int end);

	node* insert(node* newNode);

public:
	const node* getRootNode() const;
	const node* getFirstLeftNode() const;
	const node* getFirstNode() const;
	const node* getNextNode() const;

	const node* getFirstUnknowNode() const;
	const node* getNextUnknowNode() const;

	const node* getParent() const;
	const node* getLeft() const;
	const node* getRight() const;
	const node* getChild(bool bLeft) const;

	char getType() const;
	bool isCalcType() const;

	node* getLeft();
	node* getRight();

	node* getRootNode();
	node* getFirstUnknowNode();
	node* getNextUnknowNode();
	node* getFirstNode();
	node* getFirstLeftNode();
	node* getNextNode();

	int nArguments(bool toRight) const;

	// For Debug:
	int getMetricsNodes(int& size) const;
	int getMetricsNodes(ndtype type, int& size) const;
	static int getSizeofNode(ndtype type);

protected:
	static node* newNodeUnknowEmpty(const node* parent, const node* newNode);

	void insertToRight(node* newNode);
	void insertToLeft(node* newNode);
	void insertInThisRight(node* newNode);
	void insertInThisNode(node* newNode);
	node* insertEmptyUnknow(const node* newNode);

	node* deleteThisNode();
	void replaceThisNode(node* newNode);
	void upLeftToThisNode();
	void insertWithLowPriority(node* newNode);
	node* insertUnknowListInParentheses(node* newUnknow);

	node* foundOpenParentheses();

	void switchToUpNode();

private:
	node* insert_space(node* newNode);
	node* insert_alpha(node* newNode);
	node* insert_number(node* newNode);
	node* insert_parentheses(node* newNode);
	node* insert_list(node* newNode);
	node* insert_unknow(node* newNode);
	node* insert_string(node* newNode);


protected:
	node* _left;
	node* _parent;
	node* _right;

	ndtype _type;

	template<class _TVALUE>
	friend class tree;

#ifdef _DEBUG
	template<class _TVALUE>
	friend void TRACE_NODE(const char* text, const pmb::parser::node<_TVALUE>* nd, bool isBaseClass, bool recursive);
#endif
};



#ifdef _DEBUG
template<class _TVALUE>
void TRACE_NODE(const char* text, const pmb::parser::node<_TVALUE>* nd, bool isBaseClass = false, bool recursive = true)
{
	if(!isBaseClass && nd->isCalcType())
	{
		const _TVALUE& val =	nd->getType() == pmb::parser::ndUnknow ? static_cast<const pmb::parser::nodes::unknow<_TVALUE>*>(nd)->getValue():
								nd->getType() == pmb::parser::ndParentheses ? static_cast<const pmb::parser::nodes::parentheses<_TVALUE>*>(nd)->getValue():
										*static_cast<const pmb::parser::nodes::list<_TVALUE>*>(nd)->getRValue();
		if(nd->getType() != pmb::parser::ndUnknow)
			AfxTrace(_T("%s node %s: 0x%08X %c[%d, %d]{l0x%08X, r0x%08X}"), (LPCTSTR)CString(text),
					nd->getType() == pmb::parser::ndList ? L"list": L"parentheses",
				nd, nd->getType(), nd->_ini, nd->_end, 
			nd->getLeft(), nd->getRight());
		else {
			const pmb::parser::nodes::unknow<_TVALUE>* uk = static_cast<const pmb::parser::nodes::unknow<_TVALUE>*>(nd);
			if(uk->getOperation())
				AfxTrace(_T("%s node unknow: 0x%08X %c'%s'[%c,%c,%d][%d, %d]{l0x%08X, r0x%08X}"), (LPCTSTR)CString(text),
					nd, nd->getType(), 
					(LPCTSTR)CString(uk->getOperation()->getSymbol()), uk->getOperation()->isBinary() ? 'b': 'u',
					uk->getOperation()->isLeftToRight() ? 'L': 'R', uk->getOperation()->getPrecedence(),
					nd->_ini, nd->_end, 
					nd->getLeft(), nd->getRight());
			else
				AfxTrace(_T("%s node unknow: 0x%08X %c'Operation not found'[%d, %d]{l0x%08X, r0x%08X}"), (LPCTSTR)CString(text),
					nd, nd->getType(), nd->_ini, nd->_end, 
					nd->getLeft(), nd->getRight());
		}

		val._print(nd->getType() != pmb::parser::ndList ? L" value": L" rValue", false);
		if(nd->getType() == pmb::parser::ndList)
			static_cast<const pmb::parser::nodes::list<_TVALUE>*>(nd)->getLValue()._print(L" | lValue", false);
		AfxTrace(_T("\n"));
	}
	else
		AfxTrace(_T("%s node: 0x%08X %c[%d, %d]{l0x%08X, r0x%08X}\n"), (LPCTSTR)CString(text), nd, (nd)->getType(), (nd)->_ini, (nd)->_end, (nd)->getLeft(), (nd)->getRight());
}
#else
#define TRACE_NODE(text, nd)	
#endif


}
}

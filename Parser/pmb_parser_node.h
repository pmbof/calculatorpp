#pragma once

#include "pmb_log.h"

#include "pmb_parser_item.cpp"

// Only for trace node:
#include <iomanip>


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
	ndUnknown		= 'U'
};




template <class _ITEM, typename _NDTYPE>
class node: public _ITEM
{
public:
	typedef typename _ITEM cItem;
	typedef typename _ITEM::SIZETP ISIZE;
	typedef typename _ITEM::_CPTRCHAR CPTRCHAR;

	typedef typename _NDTYPE cNdType;
	typedef typename node<cItem, cNdType> cnode;

protected:
	node(cNdType type, ISIZE ini, ISIZE end);

public:
	~node();

	static node* newNodeSpace(ISIZE ini, ISIZE end);
	static node* newNodeAlpha(ISIZE ini, ISIZE end);
	static node* newNodeNumber(ISIZE ini, ISIZE end);
	static node* newNodeString(ISIZE ini, ISIZE end);
	static node* newNodeParentheses(ISIZE ini, ISIZE end, char type, int opened);
	static node* newNodeList(ISIZE end);
	static node* newNodeUnknown(ISIZE ini, ISIZE end);

	node* insert(node* newNode);

public:
	const node* getRootNode() const;
	const node* getFirstLeftNode() const;
	const node* getFirstNode() const;
	const node* getNextNode() const;

	const node* getFirstUnknownNode() const;
	const node* getNextUnknownNode() const;

	const node* getParent() const;
	const node* getLeft() const;
	const node* getRight() const;
	const node* getChild(bool bLeft) const;

	char getType() const;
	bool isCalcType() const;

	node* getLeft();
	node* getRight();

	node* getRootNode();
	node* getFirstUnknownNode();
	node* getNextUnknownNode();
	node* getFirstNode();
	node* getFirstLeftNode();
	node* getNextNode();

	int nArguments(bool toRight) const;

	// For Debug:
	int getMetricsNodes(int& size) const;
	int getMetricsNodes(cNdType type, int& size) const;
	static int getSizeofNode(cNdType type);
	const char* getTypeL() const;

	bool checkParentheses() const;

	bool check() const;

protected:
	static node* newNodeUnknownEmpty(const cnode* parent, const cnode* newNode);

	void insertInRight(cnode* newnode);
	void insertToRight(cnode* newNode);
	void insertToLeft(node* newNode);
	void insertInThisRight(node* newNode);
	void insertInThisNode(node* newNode);
	node* insertEmptyUnknown(const node* newNode);

	node* deleteThisNode();
	void replaceThisNode(node* newNode);
	void upLeftToThisNode();
	void insertWithLowPriority(node* newNode);
	node* insertUnknownListInParentheses(node* newUnknown);

	node* foundOpenParentheses(short& closed, node*& lastParentOpened);

	void switchToUpNode();

private:
	node* insert_space(node* newNode);
	node* insert_alpha(node* newNode);
	node* insert_number(node* newNode);
	node* insert_parentheses(node* newNode);
	node* insert_list(node* newNode);
	node* insert_unknown(node* newNode);
	node* insert_string(node* newNode);


protected:
	node* _left;
	node* _parent;
	node* _right;

	cNdType _type;

	template<class cItem, class cNdType>
	friend class tree;

#ifdef _DEBUG
	template<class _ITEM, typename _NDTYPE>
	friend void TRACE_NODE(pmb::log_type logType, typename _ITEM::_CPTRCHAR text, typename _ITEM::_CPTRCHAR expr, const pmb::parser::node<_ITEM, _NDTYPE>* nd, bool isBaseClass, bool recursive);
#endif
};



#ifdef _DEBUG
template<class _ITEM, typename _NDTYPE>
void TRACE_NODE(pmb::log_type logType, typename _ITEM::_CPTRCHAR text, typename _ITEM::_CPTRCHAR expr, const pmb::parser::node<_ITEM, _NDTYPE>* nd, bool isBaseClass = false, bool recursive = true)
{
	log* plg = log::instance(false);
	*plg << text << " node " << nd->getTypeL() << ": 0x" << std::hex << std::setw(8) << std::setfill('0') << nd << " " << nd->getType();
	std::stringstream sexpr;
	sexpr << "[" << std::dec << nd->_ini << ", " << nd->_end;
	if (expr)
		sexpr << ", '" << std::string(expr + nd->_ini, nd->_end - nd->_ini) << "'";
	if (!isBaseClass && nd->isCalcType())
	{
		const pmb::parser::nodes::calc<_ITEM, _NDTYPE>* uc = static_cast<const pmb::parser::nodes::calc<_ITEM, _NDTYPE>*>(nd);
//		const _TVALUE& val = nd->getType() == pmb::parser::ndUnknown ? static_cast<const pmb::parser::nodes::unknown<_TVALUE, _ITEM, _NDTYPE>*>(nd)->getValue() :
//			nd->getType() == pmb::parser::ndParentheses ? static_cast<const pmb::parser::nodes::parentheses<_TVALUE, _ITEM, _NDTYPE>*>(nd)->getValue() :
//			*static_cast<const pmb::parser::nodes::list<_TVALUE, _ITEM, _NDTYPE>*>(nd)->getRValue();

		if (nd->getType() != pmb::parser::ndUnknown)
			*plg << sexpr.str() << "]{0x" << std::hex << nd->getLeft() << ", 0x" << nd->getRight() << "}  ->  " << std::dec;
		else {
			const pmb::parser::nodes::unknown<_ITEM, _NDTYPE>* uk = static_cast<const pmb::parser::nodes::unknown<_ITEM, _NDTYPE>*>(nd);
			if (uk->isValid())
				*plg << " " << (uk->isCallBuildInFunction() ? "Call build in function" : uk->isCallUserDefFunction() ? "Call user defined function" : "Operation") << ": [" << (uk->isBinary() ? 'b' : 'u') << "," << (uk->isLeftToRight() ? 'L' : 'R')
					<< "," << std::dec << uk->precedence() << "]"
					<< sexpr.str() << "]{0x" << std::hex << nd->getLeft() << ", 0x" << nd->getRight() << "}  ->  " << std::dec;
			else
				*plg << " 'Operation not found' "
					<< sexpr.str() << "]{0x" << std::hex << nd->getLeft() << ", 0x" << nd->getRight() << "}  ->  " << std::dec;
		}

//		val._print(logType, nd->getType() != pmb::parser::ndList ? " value": " rValue", false);
//		if(nd->getType() == pmb::parser::ndList)
//			static_cast<const pmb::parser::nodes::list<_ITEM, _NDTYPE>*>(nd)->getLValue()._print(logType, " | lValue", false);
		*plg << "\n";
	}
	else
		*plg << sexpr.str() << "]{0x" << std::hex << nd->getLeft() << ", 0x" << nd->getRight() << "}\n" << std::dec;
}
#else
#define TRACE_NODE(logType, text, nd) 
#endif


}
}

#pragma once


#include "pmb_parser_calc_build_in_function.hpp"
#include "pmb_parser_calc_transporter.hpp"
#include <map>


namespace pmb
{
namespace parser
{
namespace calc
{




// Y está clase debe ser tree_iterator (un arbol analizar)
template <class _TVARGS, class _TREE> 
struct iterator
{
	typedef _TREE tptree;
	typedef typename tptree::cnode tnode;
	typedef typename tptree::cItem titem;
	typedef typename tptree::cNdType tndttype;

	typedef _TVARGS transporter_args;
	typedef typename transporter_args::transporter transporter;
	typedef typename transporter_args::nargs nargs;

	struct inode
	{
		inode* _prev;
		inode* _next;

		bool _isCalculated;
		bool _isVarDependent;
		const tnode* _node;
		const tnode* _nodeLast;
		const tnode* _nodeJoinLast;
		transporter_args _transporter;
	};


// Constructor and destructor
	iterator(tptree* tree);
	~iterator();

// Operations for move cursor:
	bool begin();
	void end();

	bool operator ()() const;
	operator bool() const;
	inode* next();
	inode* prev();
	inode* operator++();
	const tnode* node() const;
	const tnode* lastNode() const;
	const tnode* lastJoinNode() const;
	void calculated(bool bCalculated);
	bool calculated() const;
	bool calculating() const;
	void varDependent(bool bVarDependent = true);
	bool is_varDependent() const;

//  Get root node of tree:
	const tnode* rootNode() const;

//  Over arguments:
	nargs getNArgs() const;
	transporter_args& getValues();
	bool hasValue(nargs argN) const;
	bool hasPlace(nargs argN) const;

	transporter& operator[](nargs argN);
	transporter& left();
	transporter& right();
	transporter& result();

	void parameters_clear();
	nargs parameters_amount() const;
	void parameters_add(transporter& param);
	void parameters_set(transporter_args& arguments);

	tptree* tree();

	void setBegined();
	const tnode* function() const;
	bool function_definition() const;
	void function(const tnode* breakNode);

	bool stoped() const;

protected:
//	tnode* _nextCalc();
	void clear(inode* root);
	const tnode* next_calc(bool& stopCalculation);

protected:
	bool _begined;
	const tnode* _function;
	transporter_args _parameters;
	tptree* _tree;
	inode* _root;
	inode* _cursor;
	bool _stoped;
	inode* _rootCalc;
	const tnode* _rootNode;

	typedef std::map<const tnode*, inode*> map;
	map _cmap;
};







template <class _BUILD_IN_FUNCTION, class _CITERATOR>
struct pair_function
{
	typedef _CITERATOR iterator;
	typedef _BUILD_IN_FUNCTION tpbin_function;
	typedef typename iterator::nargs nargs;

	pair_function();
	pair_function(iterator* userDef, nargs nArgs);
	pair_function(const tpbin_function* pFnc);

	bool operator!() const;
	operator bool() const;

	nargs _nArgs;
	iterator* _userDef;
	const tpbin_function* _buildin;
};


}
}
}
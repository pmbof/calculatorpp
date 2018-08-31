#pragma once

#include "pmb_parser_calc_iterator.hpp"


namespace pmb
{
namespace parser
{
namespace calc
{













template <class _CITERATOR, class _BIN_FNCTABLE, class _SYMBOL>
class block
{
public:
	typedef _CITERATOR iterator;
	typedef typename iterator::tptree tptree;
	typedef typename tptree::cItem cItem;
	typedef typename tptree::cNdType cNdType;

	typedef typename cItem::string istring;
	typedef typename cItem::CHAR CHAR;
	typedef typename cItem::_CPTRCHAR _CPTRCHAR;
	typedef typename cItem::SIZETP szcItem;

	typedef typename iterator::transporter_args transporter_args;
	typedef typename iterator::transporter transporter;

	typedef typename _SYMBOL symbol;
	typedef typename symbol::_tpMap map_args;

	typedef typename iterator::nargs nargs;
	typedef std::map<nargs, iterator*> map_nargs_iterator;
	typedef util::map<std::string, map_nargs_iterator, istring> mapfnc;
	typedef typename transporter::tpvalue tpvalue;

	typedef typename iterator::tnode tnode;
	typedef typename _BIN_FNCTABLE::build_in_function build_in_function;
	typedef pair_function<typename build_in_function, iterator> pair_function;

public:
	struct stack
	{
		stack(iterator* list);
		stack(tptree* tree);
		~stack();

		void begin(_CPTRCHAR expr);

		bool function_def() const;
		bool function_call() const;
		bool in_function_breaknode() const;

		iterator* release();

	protected:
		_CPTRCHAR _expr;
		iterator* _it_calc;
		bool _delete;

		istring _fncName;
		map_args _margs;

		friend class block;
	};

	typedef std::list<stack*> lststack;


private:
	struct bnode
	{
		bnode(block* root, symbol* variables);
		bnode(bnode* parent);
		~bnode();

		block* _root;
		bnode* _parent;

		bnode* _child;  // pointer to first child
		bnode* _next;

		bool _internal;
		lststack _stack;
		symbol* _variables;
		mapfnc _functions;
	};


protected:
//	block(const _BIN_FNCTABLE* bin_fnc_table);
public:
	block(const _BIN_FNCTABLE* bin_fnc_table, symbol* variables);
	~block();

	void init(tptree* tree);
	
	iterator* begin(_CPTRCHAR expr);
	operator bool () const;
	bool calculate();
	transporter_args& getValues();
	void next();
	typename const transporter& tresult() const;
	typename const tpvalue& result() const;
	typename const tpvalue::_TypeValue& nresult() const;
	const tnode* rootNode() const;

	stack* actualStack();
	bool is_functionDef() const;

	bool insert_function();

	pair_function find_function(const tnode* nd, typename _CPTRCHAR expr, bool toRight, bool firstFindBuildInFunctions) const;

	iterator* call_function(iterator* function, transporter_args& args);

	symbol* variables();

protected:
	void insert_function(const istring& fncName, iterator* clist);

	bool find_variable(const istring& symbol, transporter& value, bool canCreate);

protected:
	bnode* _root;
	bnode* _ndActual;

	iterator* _lastResult;

	const _BIN_FNCTABLE* _fncTable;
};







}
}
}
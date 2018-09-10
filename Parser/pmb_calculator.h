#pragma once

#include "pmb_parser_algorithm.h"


namespace pmb
{



template <class _BLOCK, class _OPRTABLE>
class calculator : public parser::algorithm<_BLOCK, _OPRTABLE>
{
public:
	typedef parser::algorithm<_BLOCK, _OPRTABLE> _base;
	typedef typename _base::symbol symbol;
	typedef typename symbol::tpChar tpChar;
	typedef typename _base::_tdOprTable _tdOprTable;

public:
	calculator(_tdOprTable* operation_table, _BLOCK* pBlock);
	~calculator();

	bool add_unit(const typename tpChar* name, const typename tpChar* expression, const typename tpChar* group = nullptr);
	bool add_unit(const typename tpChar* expression);

	bool add_constant(const typename tpChar* name, const typename tpChar* expression);
};



}
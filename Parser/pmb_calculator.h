#pragma once

#include "pmb_parser_algorithm.h"
#include "pmb_calculate_sunitType.h"



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
	typedef typename _BLOCK::iterator::titem::SIZETP citemsz;
	typedef typename _base::tptree tptree;
public:
	calculator(_tdOprTable* operation_table, _BLOCK* pBlock);
	~calculator();

	void clear();

	bool add_unit(const typename tpChar* name, const typename tpChar* expression, const typename tpChar* group = nullptr, pmb::calculate::units::sunitType automatic = pmb::calculate::units::sutAutomaticPostfix);
	bool add_unit(const typename tpChar* expression, pmb::calculate::units::sunitType automatic = pmb::calculate::units::sutAutomaticPostfix);

	bool add_constant(const typename tpChar* name, const typename tpChar* expression);

	bool calculate(const tpChar* expr);
	bool parser_result(const tpChar* res);

	const tptree* getResultNode() const;

protected:
	tptree* _result;
	const tpChar* _rexpr;
};



}
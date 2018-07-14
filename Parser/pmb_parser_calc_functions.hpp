#include "stdafx.h"
#include "pmb_parser_calc_functions.h"

#pragma once

namespace pmb
{
namespace parser
{
namespace calc
{


template<class _TVALUE, class _ITEM, typename _NDTYPE>
inline functions<_TVALUE, _ITEM, _NDTYPE>::~functions()
{
	for (mapfnc::const_iterator f = _functions.begin(); f != _functions.end(); ++f)
		for (map_nargs_pair_arg_tree::const_iterator fa = f->second.begin(); fa != f->second.end(); ++fa)
			delete fa->second.second;
}



template<class _TVALUE, class _ITEM, typename _NDTYPE>
inline functions<_TVALUE, _ITEM, _NDTYPE>::tptree* functions<_TVALUE, _ITEM, _NDTYPE>::find(const node<_TVALUE, _ITEM, _NDTYPE>* nd, const char * expr, bool toRight) const
{
	const node<_TVALUE, _ITEM, _NDTYPE>* chFnc = toRight ? nd->getLeft() : nd->getRight();
	if (chFnc)
	{
		int args = nd->nArguments(toRight);
		mapfnc::const_iterator cif = _functions.find(chFnc->getString(expr));
		if (cif != _functions.end())
		{
			map_nargs_pair_arg_tree::const_iterator ciff = cif->second.find(args);
			if (ciff != cif->second.end())
				return pair_ffnc(ciff->second.second);
		}
	}
	return pair_ffnc();
}


template<class _TVALUE, class _ITEM, typename _NDTYPE>
inline void functions<_TVALUE, _ITEM, _NDTYPE>::insert(const sitem & fncName, const argmap & args, tptree * tree)
{
	_functions[fncName.getString()][args.size()] = pair_arg_tree(args, tree);
}






}
}
}
#include "StdAfx.h"
#include "pmb_parser_nodes_calc.h"


namespace pmb
{
namespace parser
{
namespace nodes
{



template<class _TVALUE>
calc<_TVALUE>::calc(ndtype type, int ini, int end)
	: node<_TVALUE>(type, ini, end), _bCalculated(false), _isVariableDependent(false)
{
}


template<class _TVALUE>
calc<_TVALUE>::~calc()
{
}


template<class _TVALUE>
void calc<_TVALUE>::setCalculated(bool bCalculated)
{
	_bCalculated = bCalculated;
}


template<class _TVALUE>
void calc<_TVALUE>::setVariableDependent(bool isVariableDependet)
{
	_isVariableDependent = isVariableDependet;
}


template<class _TVALUE>
bool calc<_TVALUE>::isCalculated() const
{
	return _bCalculated;
}


template<class _TVALUE>
bool calc<_TVALUE>::isVariableDependent() const
{
	return _isVariableDependent;
}


template<class _TVALUE>
bool calc<_TVALUE>::isNotCalculated(const node<_TVALUE>* nd)
{
	return nd && nd->isCalcType() && !static_cast<const calc*>(nd)->_bCalculated;
}




template<class _TVALUE>
calc<_TVALUE>* calc<_TVALUE>::nextCalc()
{
	AfxTrace(_T("calc<_TVALUE>::nextCalc()\n"));
	calc<_TVALUE>* c,
					* nc;
	for(c = this, nc = this; c && nc; )
	{
		nc = NULL;
		if(c->_type == ndUnknow)
		{
			unknow<_TVALUE>* uk = static_cast<unknow<_TVALUE>*>(c);
			if(uk->getOperation())
			{
				if(uk->getOperation()->isFirstLeft())
				{
					if(isNotCalculated(uk->_left))
						nc = static_cast<calc<_TVALUE>*>(uk->_left);
					if(!nc && isNotCalculated(uk->_right))
						nc = static_cast<calc<_TVALUE>*>(uk->_right);
				}
				if(!nc && uk->getOperation()->isFirstRight())
				{
					if(isNotCalculated(uk->_right))
						nc = static_cast<calc<_TVALUE>*>(uk->_right);
					if(!nc && isNotCalculated(uk->_left))
						nc = static_cast<calc<_TVALUE>*>(uk->_left);
				}
				if(!nc)
				{
					if(uk->_bCalculated)
						nc = static_cast<calc<_TVALUE>*>(uk->_parent);
				}
				else if(!nc)
					TRACE_NODE("ERROR: Condition not match", uk);
			}
		}
		else //if(c->_type == ndParentheses || c->_type == ndList)
		{
			if(isNotCalculated(c->_left))
				nc = static_cast<calc<_TVALUE>*>(c->_left);
			if(!nc && isNotCalculated(c->_right))
				nc = static_cast<calc<_TVALUE>*>(c->_right);
			if(!nc)
			{
//				if(!c->_bCalculated)
//					c->_bCalculated = true;
				if(c->_bCalculated)
					nc = static_cast<calc<_TVALUE>*>(c->_parent);
			}
		}
		if(nc)
			c = nc;
	}
	return !nc && c ? c->_bCalculated ? NULL: c: nc;
}




}
}
}
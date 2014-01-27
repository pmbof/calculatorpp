#pragma once

#include "pmb_parser_algorithm.h"


namespace pmb
{
namespace parser
{
namespace debug
{


struct iterator: parser::iterator
{
	int _step0;
	int _i0;
	int _subStep0;
	int _subStep1;

	iterator& operator= (const char* str)
	{
		_expr = str;
		_i0 = _i = 0;
		_step0 = -1;
		_subStep0 = 0;
		_subStep1 = 0;
		return *this;
	}
	
	inline operator bool() const
	{
		return _expr[_i] || _step0 < 6;
	}

	void incrStep()
	{
		++_step0;
		_subStep0 = 0;
	}
};

template <int i, class _IT, class _TVALUE>
struct auto_iterator
{
	auto_iterator(_IT& expr): _it(expr), _next(expr), _space(expr) { }

	node<_TVALUE>* operator()()
	{
		node<_TVALUE>* nd = NULL;
		switch(_it._step0) {
		case -1:
			_next.initLoop();
			break;
		case 0:
			_it._i0 = _it._i;
			nd = _space();
			break;
		default:
			nd = _next();
			break;
		}
		return nd;
	}


	node<_TVALUE>* release()
	{
		return _next.release();
	}

	bool space()
	{
		return _space.release();
	}


	const node<_TVALUE>* getUnknow() const
	{
		return _next.getUnknow();
	}


	const char* getClassName() const
	{
		return _it._step0 == -1 ? "initLoop": _it._step0 == 0 ? "space": _next.getClassName();
	}

protected:
	_IT& _it;
	auto_iterator<1, _IT, _TVALUE> _next;
	iterators::space<_IT, wrapper, _TVALUE> _space;
};


template <class _IT, class _TVALUE>
struct auto_iterator<6, _IT, _TVALUE>
{
	auto_iterator(_IT& expr): _it(expr), _unknow(expr) { }

	node<_TVALUE>* operator()()
	{
		node<_TVALUE>* nd = _it._step0 == 6 ? _unknow(): NULL;
		if(6 < _it._step0)
			_it._step0 = -2;
		return nd;
	}


	void initLoop()
	{
		_unknow.initLoop();
	}


	node<_TVALUE>* release()
	{
		return _unknow.release();
	}


	const char* getClassName() const
	{
		return _it._step0 == 6 ? "unknow": NULL;
	}

protected:
	_IT& _it;
	iterators::unknow<_IT, wrapper, _TVALUE> _unknow;
};




template <class _IT, class _TVALUE>
struct auto_iterator<5, _IT, _TVALUE>
{
	auto_iterator(_IT& expr): _it(expr), _next(expr), _list(expr) { }

	node<_TVALUE>* operator()()
	{
		return _it._step0 == 5 ? _list(): _next();
	}


	void initLoop()
	{
		_next.initLoop();
	}


	node<_TVALUE>* release()
	{
		return _next.release();
	}


	const char* getClassName() const
	{
		return _it._step0 == 5 ? "list": _next.getClassName();
	}

protected:
	_IT& _it;
	auto_iterator<6, _IT, _TVALUE> _next;
	iterators::list<_IT, wrapper, _TVALUE> _list;
};




template <class _IT, class _TVALUE>
struct auto_iterator<4, _IT, _TVALUE>
{
	auto_iterator(_IT& expr): _it(expr), _next(expr), _parentheses(expr) { }

	node<_TVALUE>* operator()()
	{
		return _it._step0 == 4 ? _parentheses(): _next();
	}


	void initLoop()
	{
		_next.initLoop();
	}


	node<_TVALUE>* release()
	{
		return _next.release();
	}


	const char* getClassName() const
	{
		return _it._step0 == 4 ? "parentheses": _next.getClassName();
	}

protected:
	_IT& _it;
	auto_iterator<5, _IT, _TVALUE> _next;
	iterators::parentheses<_IT, wrapper, _TVALUE> _parentheses;
};



template <class _IT, class _TVALUE>
struct auto_iterator<3, _IT, _TVALUE>
{
	auto_iterator(_IT& expr): _it(expr), _next(expr), _string(expr) { }

	node<_TVALUE>* operator()()
	{
		return _it._step0 == 3 ? _string(): _next();
	}


	void initLoop()
	{
		_next.initLoop();
	}


	node<_TVALUE>* release()
	{
		return _next.release();
	}

	const char* getClassName() const
	{
		return _it._step0 == 3 ? "string": _next.getClassName();
	}

protected:
	_IT& _it;
	auto_iterator<4, _IT, _TVALUE> _next;
	iterators::string<_IT, '\\', wrapper, _TVALUE> _string;
};



template <class _IT, class _TVALUE>
struct auto_iterator<2, _IT, _TVALUE>
{
	auto_iterator(_IT& expr): _it(expr),  _next(expr), _number(expr) { }

	node<_TVALUE>* operator()()
	{
		return _it._step0 == 2 ? _number(): _next();
	}


	void initLoop()
	{
		_next.initLoop();
	}


	node<_TVALUE>* release()
	{
		return _next.release();
	}


	const char* getClassName() const
	{
		return _it._step0 == 2 ? "number": _next.getClassName();
	}

protected:
	_IT& _it;
	auto_iterator<3, _IT, _TVALUE> _next;
	iterators::number<_IT, wrapper, _TVALUE> _number;
};



template <class _IT, class _TVALUE>
struct auto_iterator<1, _IT, _TVALUE>
{
	auto_iterator(_IT& expr): _it(expr), _next(expr), _alpha(expr) { }

	node<_TVALUE>* operator()()
	{
		return _it._step0 == 1 ? _alpha(): _next();
	}


	void initLoop()
	{
		_next.initLoop();
	}


	node<_TVALUE>* release()
	{
		return _next.release();
	}


	const char* getClassName() const
	{
		return _it._step0 == 1 ? "alpha": _next.getClassName();
	}

protected:
	_IT& _it;
	auto_iterator<2, _IT, _TVALUE> _next;
	iterators::alpha<_IT, wrapper, _TVALUE> _alpha;
};





// Class For Debug:
class algorithm: public parser::algorithm<value<number<double> > , iterator>
{
public:
	typedef value<number<double> > value;
	typedef parser::algorithm<value, iterator> _base;

public:
	algorithm(_base::_tdSymbols& symbols);
	~algorithm();

	const auto_iterator<0, iterator, value>* getIterators() const;
	const iterator* getIterator() const;
	const tree<value>* getTree() const;

	const node<value>* getNewNode() const;
	const node<value>* getNewNodeUnknow() const;

	bool nextStep();

protected:
	void populate();
	void mapUnknow();

	auto_iterator<0, iterator, value> _ai;

	node<value>* _newNode;
	node<value>* _newNodeUnknow;
};



}
}
}
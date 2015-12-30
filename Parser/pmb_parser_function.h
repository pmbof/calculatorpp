#pragma once

#include "pmb_parser_node.h"


namespace pmb
{
namespace parser
{



template<class _TVALUE>
struct functions
{
	void abs(_TVALUE& result, int nArgs, const _TVALUE* value) {
//		result = value;
	}

	void sgn(_TVALUE& result, int nArgs, const _TVALUE* value) {
//		result = value;
	}

	void rnd(_TVALUE& result, int nArgs, const _TVALUE* value) {
//		result = value;
	}

	void ceil(_TVALUE& result, int nArgs, const _TVALUE* value) {
//		result = value;
	}

	void floor(_TVALUE& result, int nArgs, const _TVALUE* value) {
//		result = value;
	}

	void round(_TVALUE& result, int nArgs, const _TVALUE* value) {
//		result = value;
	}

	void lg(_TVALUE& result, int nArgs, const _TVALUE* value) {
//		result = value;
	}

	void ln(_TVALUE& result, int nArgs, const _TVALUE* value) {
//		result = value;
	}

	void log(_TVALUE& result, int nArgs, const _TVALUE* value) {
//		result = value;
	}

	void exp(_TVALUE& result, int nArgs, const _TVALUE* value) {
//		result = value;
	}

	void sin(_TVALUE& result, int nArgs, const _TVALUE* value) {
		result->_number = ::sin(value[0]->_number);
	}

	void cos(_TVALUE& result, int nArgs, const _TVALUE* value) {
//		result.negative(value);
	}

	void tg(_TVALUE& result, int nArgs, const _TVALUE* value) {
//		result = value;
	}

	void sec(_TVALUE& result, int nArgs, const _TVALUE* value) {
//		result = value;
	}

	void cosec(_TVALUE& result, int nArgs, const _TVALUE* value) {
//		result = value;
	}

	void cotg(_TVALUE& result, int nArgs, const _TVALUE* value) {
//		result = value;
	}

	void asin(_TVALUE& result, int nArgs, const _TVALUE* value) {
		result->_number = ::asin((*value)[0]->_number);
	}

	void asin2(_TVALUE& result, int nArgs, const _TVALUE* value) {
		result = ::asin((*value)[0]->_number / (*value)[1]->_number) + ((*value)[0]->_number < 0 ? atan(1.0) * 2.0: 0);
	}

	void acos(_TVALUE& result, int nArgs, const _TVALUE* value) {
//		result = value;
	}

	void atg(_TVALUE& result, int nArgs, const _TVALUE* value) {
//		result = value;
	}

	void asec(_TVALUE& result, int nArgs, const _TVALUE* value) {
//		result = value;
	}

	void acosec(_TVALUE& result, int nArgs, const _TVALUE* value) {
//		result = value;
	}

	void acotg(_TVALUE& result, int nArgs, const _TVALUE* value) {
//		result = value;
	}

	void _if(_TVALUE& result, int nArgs, const _TVALUE* value) {
//		result = value;
	}

};



template<class _TVALUE>
class function
{
	typedef void (functions<_TVALUE>::*tpFunc)(_TVALUE&, int nArgs, const _TVALUE* args);

public:
	function(const char* name, const char* description, tpFunc func, int nArgs = 1);
	~function();

	bool compare(const char* expr, int len, int nArgs) const;


	const char* getName() const;
	const char* getDescription() const;
	int getNArgs() const;

	void operator()(_TVALUE& result, int nArgs, const _TVALUE* vals) const;

private:
	char* _name;
	int _len;
	char* _description;
	tpFunc _func;
	int _nArgs;
};




template<class _TVALUE>
class function_table 
{
public:
	static int getPrecedence();

public:
	const function<_TVALUE>* find(const node<_TVALUE>* nd, const char* expr, bool toRight) const;

	const function<_TVALUE>* get(int i) const;
	int size() const;


protected:
	static const function<_TVALUE> _fnc[];
	static const int _fncSize;
};


}
}
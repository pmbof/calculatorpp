#pragma once

#include "pmb_parser_node.h"
#include "pmb_util_map.h"

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


template<class _TVALUE, class _ITEM, typename _NDTYPE>
struct pair_ffnc
{
	typedef tree<_ITEM, _NDTYPE> tptree;

	pair_ffnc();
	pair_ffnc(tptree* pTree);
	pair_ffnc(const function<_TVALUE>* pFnc);

	bool operator!() const;
	operator bool() const;

	tptree* ptree;
	const function<_TVALUE>* pFunc;
};


template<class _TVALUE, class _ITEM, typename _NDTYPE>
class function_table 
{
	typedef typename _ITEM::string sitem;
	typedef tree<_ITEM, _NDTYPE> tptree;
	typedef util::map<std::string, _TVALUE, sitem> argmap;
	typedef std::pair <argmap, tptree*> pair_arg_tree;

	typedef std::map<int, pair_arg_tree> map_nargs_pair_arg_tree;
	typedef util::map<std::string, map_nargs_pair_arg_tree, sitem> mapfnc;
public:
	typedef pair_ffnc<_TVALUE, _ITEM, _NDTYPE> pair_ffnc;

public:
	static int getPrecedence();

public:
	~function_table();

	pair_ffnc find(const node<_ITEM, _NDTYPE>* nd, const char* expr, bool toRight) const;

	const function<_TVALUE>* get(int i) const;
	int size() const;

	void insert(const sitem& fncName, const argmap& args, tptree* tree);

protected:
	static const function<_TVALUE> _fnc[];
	static const int _fncSize;

	mapfnc _functions;
};


}
}
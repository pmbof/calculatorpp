#pragma once




namespace pmb
{
namespace parser
{
namespace calc
{


template<class _TVALUE, class _ITEM, typename _NDTYPE>
class functions
{
	typedef typename _ITEM::string sitem;
	typedef tree<_TVALUE, _ITEM, _NDTYPE> tptree;
	typedef util::map<std::string, _TVALUE, sitem> argmap;
	typedef std::pair <argmap, tptree*> pair_arg_tree;

	typedef std::map<int, pair_arg_tree> map_nargs_pair_arg_tree;
	typedef util::map<std::string, map_nargs_pair_arg_tree, sitem> mapfnc;

public:
	~functions();

	tptree* find(const node<_TVALUE, _ITEM, _NDTYPE>* nd, const char* expr, bool toRight) const;

	void insert(const sitem& fncName, const argmap& args, tptree* tree);

protected:
	mapfnc _functions;
};




}
}
}
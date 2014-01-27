#include "StdAfx.h"
#include "pmb_parser_item.h"


namespace pmb
{
namespace parser
{



item::item(int ini, int end)
	: _ini(ini), _end(end)
{
}


int item::len() const
{
	return _end - _ini;
}


item::string item::getString(const char* expr) const
{
	return string(expr + _ini, len());
}

const char* item::getCharPtr(const char* expr) const
{
	return expr + _ini;
}

int item::getIni() const
{
	return _ini;
}

int item::getEnd() const
{
	return _end;
}


bool operator <(const item::string& left, const std::string& right) {
	unsigned int i;
	for(i = 0; i < right.length() && i < left._size && left._str[i] == right[i]; ++i)
		;
	bool bRet = i < left._size && i < right.length() && left._str[i] < right[i] || i == left._size && i < right.length();

//	std::string l(left._str, left._size);
//	AfxTrace(_T("1: \"%s\" < \"%s\"\t==\t%s[%s]: %s\n"), l.c_str(), right.c_str(), bRet ? L"TRUE": L"FALSE", l < right ? L"true": L"false", 
//		bRet == l < right ? L"Ok!": L"BAD!!!");
	return bRet;
}


bool operator <(const std::string& left, const item::string& right) {
	unsigned int i;
	for(i = 0; i < left.length() && i < right._size && left[i] == right._str[i]; ++i)
		;
	bool bRet = i < left.length() && i < right._size && left[i] < right._str[i] || i == left.length() && i < right._size;

//	std::string r(right._str, right._size);
//	AfxTrace(_T("2: \"%s\" < \"%s\"\t==\t%s[%s]: %s\n"), left.c_str(), r.c_str(), bRet ? L"TRUE": L"FALSE", left < r ? L"true": L"false", 
//		bRet == left < r ? L"Ok!": L"BAD!!!");
	return bRet;
}



}
}
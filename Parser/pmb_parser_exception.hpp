#include "pmb_parser_exception.h"
#pragma once



namespace pmb
{
namespace parser
{


template<class _ITEM>
inline exception<_ITEM>::exception()
	: _item(nullptr)
{
}


template<class _ITEM>
exception<_ITEM>::exception(const char* message)
	: _message(message), _item(nullptr)
{
}


template<class _ITEM>
exception<_ITEM>::exception(const _ITEM* pItem, const char* message, bool deletable)
	: _message(message), _item(pItem), _deletable(deletable)
{
}


template<class _ITEM>
exception<_ITEM>::~exception()
{
	if (_item && _deletable)
		delete _item;
}



template<class _ITEM>
inline void exception<_ITEM>::reset()
{
	if (_item && _deletable)
		delete _item;
	_item = nullptr;
	_message.clear();
}


template<class _ITEM>
inline bool exception<_ITEM>::empty()
{
	return !_item;
}




template<class _ITEM>
inline std::string exception<_ITEM>::message(typename _ITEM::_CPTRCHAR expr) const
{
	const char* replace = "%item";
	size_t tssz = strlen(replace);
	std::string replaceStr;
	if (_item && expr)
		replaceStr = _item->getString(expr).getString();
	size_t rplcsize = replaceStr.size();
	if (!rplcsize)
		rplcsize = 1;

	std::string vret = _message;
	for (size_t pos = vret.find(replace); pos != std::string::npos; pos = vret.find(replace, pos + rplcsize))
	{
		if (!pos || vret[pos - 1] != '%')
		{
			vret.replace(pos, tssz, replaceStr);
			if (!(rplcsize = replaceStr.size()))
				rplcsize = 1;
		}
		else
			rplcsize = tssz;
	}

	return vret;
}



template<class _ITEM>
inline const _ITEM* exception<_ITEM>::item() const
{
	return _item;
}





}
}
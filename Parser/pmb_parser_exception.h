#pragma once


namespace pmb
{
namespace parser
{



template<class _ITEM>
class exception
{
public:
	exception();
	exception(const char* message);
	exception(const _ITEM* pItem, const char* message, bool deletable = false);

	~exception();

	std::string message(typename _ITEM::_CPTRCHAR expr) const;
	const _ITEM* item() const;

protected:
	std::string _message;
	const _ITEM* _item;
	bool _deletable;
};


}
}

#include "pmb_parser_exception.hpp"
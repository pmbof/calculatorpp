#pragma once

#include "pmb_parser_nodes_calc.h"
#include "pmb_parser_operation.h"
#include "pmb_parser_function.h"


namespace pmb
{
namespace parser
{
namespace nodes
{



template<class _ITEM, class _NDTYPE>
class unknown : public calc<_ITEM, _NDTYPE>
{
public:
	unknown(ISIZE ini, ISIZE end);
	~unknown();

	const void* pointer() const;

	void set(const void* opr, bool binary, bool isLeftToRight, int precedence, bool canCreateLVariable, bool canCreateRVariable, const void* buildInFnc, const void* userFnc);
	void set(const void* opr, bool isLeftToRight, int precedence, bool canCreateLVariable, bool canCreateRVariable);

	unknown* add(ISIZE ini);

	bool isValid() const;
	bool operator!() const;

	bool isFirstLeft() const;
	bool isFirstRight() const;
	int precedence() const;

	bool isBinary() const;
	bool isLeftToRight() const;

	bool isCallFunction() const;
	bool isCallBuildInFunction() const;
	bool isCallUserDefFunction() const;
	bool canCreateVariable() const;
	bool canCreateLVariable() const;
	bool canCreateRVariable() const;
	bool checkOperator()const;

protected:
	void switch2UpNode();

protected:
	const void* _pointer;
	unsigned char _flag;
	int _precedence;
};




}
}
}
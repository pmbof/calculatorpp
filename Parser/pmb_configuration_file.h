#pragma once

#include <fstream>
#include <vector>

namespace pmb
{



template <class SYMBOL, class CALC>
class configuration_file : public std::fstream
{
	typedef std::pair<int, int> prpair;
	typedef std::vector<prpair> vprpair;

	struct preprocessor : vprpair
	{
		preprocessor(const std::string& cmd, char separator = '\0');

		bool preprocess();

		bool compare(int nElement, const char* val, int szval, char separator = '\0', bool bMandatory = true) const;

		void clean(int nElement, char separator);
		prpair _clean(int nElement, char separator) const;

		CStringA operator()(int nElement, int nElements = 1, char separator = '\0') const;

		bool is_numeric(int nElement, bool bNegative = false, bool bDecimal = false, char separator = '\0') const;
		bool is_alpha(int nElement, char separator = '\0') const;
		bool is_alphanumeric(int nElement, char separator = '\0') const;
		bool has_separator(int nElement, char separator) const;

		const std::string& _cmd;
	};


public:
	void open(const char* filename);

	bool process(SYMBOL& symbols, CALC& calculator);

protected:
	bool process(SYMBOL& symbols, CALC& calculator, std::string& line, log* lg);
	bool calculate(SYMBOL& symbols, CALC& calculator, const preprocessor& prpr, const std::string& line, bool bDefault = false);

protected:
	int _nline;
	bool _bcomment;
	short _defines;
	bool _bUnit;
	CStringA _dstr[3];
	CStringA _serror;
	typename symbol::prefix_base* _prefix;
};



}


#include "pmb_configuration_file.hpp"

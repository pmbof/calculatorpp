#pragma once

#include <fstream>
#include <vector>
#include <set>


namespace pmb
{



template <class SYMBOL, class CALC>
class configuration_file : public std::fstream
{
	typedef std::pair<int, int> prpair;
	typedef std::vector<prpair> vprpair;
	typedef std::set<CStringA> mimport;

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



	configuration_file(mimport* mimport);
public:
	configuration_file();
	bool open(const char* filename);

	bool process(SYMBOL& symbols, CALC& calculator);

protected:
	bool process(SYMBOL& symbols, CALC& calculator, std::string& line, log* lg);
	bool calculate(SYMBOL& symbols, CALC& calculator, const preprocessor& prpr, const std::string& line, bool bDefault = false);

protected:
	mimport* _pmimport;
	mimport _mimport;
	std::string _filename;
	int _nline;
	bool _bmlcomment;	// Multi line comments:  /* ....... */
	bool _bcomment;		// Line comment: // ......... \n
	short _defines;
	bool _bUnit;
	CStringA _dstr[3];
	CStringA _serror;
	typename symbol::prefix_base* _prefix;
};



}


#include "pmb_configuration_file.hpp"

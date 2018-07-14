#pragma once

#include <fstream>
#include <vector>
#include <map>
#include <sstream>


namespace pmb
{

enum log_type : char
{
	logError = '\x01',
	logWarning,
	logInf,
	logDebug,
	logNone,
	logWTrace
};


class log : public std::ofstream
{
	typedef std::vector<const char*> vstr;
	typedef std::map<DWORD, vstr> mthrid_vstr;

public:
	static log* instance(log_type logLevel, const char* filename, bool bColored = false, bool bLevelFunction = false);
	static log* instance(bool bprintDate = true);
	static log* beginFunction(log_type type, const char* functionName, bool bprintDate = true);
	static log* endFunction(log_type type, const char* functionName = NULL, bool bprintDate = true);
	static log* endFunction();

private:
	static log* _instance;

	static CCriticalSection _cs;

private:
	log(const char* filename, bool bColored, bool bLevelFunction);

public:
	~log();

	log& trace(log_type type, LPCSTR lpszFormat, ...);
	log& traceN(LPCSTR str, int n, LPCSTR str2);
//	log& trace(log_type type, const char* sFormat, ...);
	log& trace(log_type type, const std::stringstream& ss);
	log& trace(log_type type, const std::string& str);

	const std::string& filename() const;
	bool colored() const;
	const char* functionName() const;
	log_type logLevel() const;

protected:
	void getThreadId();
	void printDate();
	void printType(log_type type);

protected:
	bool _bColored;
	bool _bPrintDate;
	log_type _logLevel;

	bool _levelFunction;
	const char* _sfname;
	mthrid_vstr _functionName;

private:
	std::string _filename;
};




}
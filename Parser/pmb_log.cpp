#include "stdafx.h"
#include "pmb_log.h"

#include <iomanip>



namespace pmb
{


log* log::_instance = NULL;

CCriticalSection log::_cs;


log* log::instance(log_type logLevel, const char* filename, bool bColored, bool bLevelFunction)
{
	_cs.Lock();
	if (!_instance)
	{
		AfxTrace(L"Creating instance pmb::log(\"%s\")\n", (LPCTSTR)CString(filename));
		_instance = new log(filename, bColored, bLevelFunction);
		_instance->_logLevel = logLevel;
	}
	_cs.Unlock();
	return _instance;
}


log* log::instance(bool bprintDate)
{
	if (_instance && bprintDate)
	{
		_cs.Lock();
		_instance->_bPrintDate = true;
		_cs.Unlock();
	}
	return _instance;
}


log* log::beginFunction(log_type type, const char* functionName, bool bprintDate)
{
	if (_instance)
	{
		_cs.Lock();
		_instance->_bPrintDate = bprintDate;
		const DWORD thrId = GetThreadId(GetCurrentThread());
		if (_instance->_levelFunction)
			_instance->_functionName[thrId].push_back(functionName);
		else
			_instance->_sfname = functionName;
		if (type <= _instance->_logLevel)
		{
			if (_instance->_bPrintDate)
				_instance->printDate();
			_instance->getThreadId();
			_instance->printType(type);
			*_instance << "Begin function:   [" << thrId;
			if (_instance->_levelFunction)
				*_instance << '.' << _instance->_functionName[thrId].size();
			*_instance << "]\n";
		}
		_cs.Unlock();
	}
	return _instance;
}



log* log::endFunction(log_type type, const char* functionName, bool bprintDate)
{
	if (_instance && _instance->_levelFunction)
	{
		_cs.Lock();
		_instance->_bPrintDate = bprintDate;
		const DWORD thrId = GetThreadId(GetCurrentThread());
		mthrid_vstr::iterator ithr = _instance->_functionName.find(thrId);
		if (type <= _instance->_logLevel)
		{
			if (functionName && (ithr != _instance->_functionName.end() && (!ithr->second.size() || ithr->second.back() != functionName)))
				ithr->second.push_back(functionName);
			else if (functionName && ithr == _instance->_functionName.end())
			{
				_instance->_functionName[thrId].push_back(functionName);
				ithr = _instance->_functionName.find(thrId);
			}
			if (_instance->_bPrintDate)
				_instance->printDate();
			_instance->getThreadId();
			_instance->printType(type);
			if (ithr != _instance->_functionName.end())
				*_instance << "End function.  [" << thrId << '.' << ithr->second.size() << "]\n";
			else
				*_instance << "End function.  [-.0]\n";
		}
		if (ithr != _instance->_functionName.end() && ithr->second.size())
			ithr->second.pop_back();
		_cs.Unlock();
	}
	else if (_instance && _instance->_sfname)
	{
		_cs.Lock();
		_instance->_sfname = NULL;
		_cs.Unlock();
	}
	return _instance;
}



log* log::endFunction()
{
	if (_instance && _instance->_levelFunction)
	{
		_cs.Lock();
		mthrid_vstr::iterator ithr = _instance->_functionName.find(GetThreadId(GetCurrentThread()));
		if (ithr != _instance->_functionName.end() && ithr->second.size())
			ithr->second.pop_back();
		_cs.Unlock();
	}
	else if (_instance && _instance->_levelFunction)
	{
		_cs.Lock();
		_instance->_sfname = NULL;
		_cs.Unlock();
	}
	return _instance;
}




log::log(const char* filename, bool bColored, bool bLevelFunction)
	: std::ofstream(filename, std::ofstream::out | std::ofstream::app), _filename(filename), _bColored(bColored), _levelFunction(bLevelFunction)
{
}


log::~log()
{
	close();
}


const std::string& log::filename() const
{
	return _filename;
}


const char* log::functionName() const
{
	if (!_levelFunction)
		return NULL;
	mthrid_vstr::const_iterator ithr = _functionName.find(GetThreadId(GetCurrentThread()));
	return ithr == _functionName.end() || !ithr->second.size() ? NULL : ithr->second.back();
}


bool log::colored() const
{
	return _bColored;
}


log_type log::logLevel() const
{
	return _logLevel;
}


void log::printDate()
{
	time_t t = time(0);
	tm now;
	localtime_s(&now, &t);

	if (_bColored)
		*this << char(0x1B) << "[0;38;5;33m";
	*this << std::setfill('0') << std::setw(2) << long((now.tm_year + 1900) % 100)
			<< std::setfill('0') << std::setw(2) << (now.tm_mon + 1) 
			<< std::setfill('0') << std::setw(2) << now.tm_mday 
		<< std::setfill('0') << std::setw(2) << now.tm_hour 
			<< std::setfill('0') << std::setw(2) << now.tm_min 
			<< std::setfill('0') << std::setw(2) << now.tm_sec;
	if (_bColored)
		*this << char(0x1B) << "[m";
	*this << ":";
	_bPrintDate = false;
}


void log::printType(log_type type)
{
	bool bRet = true;
	switch (type)
	{
	case logWTrace:
	case logNone:
		*this << ':';
		break;
	case logDebug:
		if (_bColored)
			*this << char(0x1B) << "[1;33m";
		*this << ":D:";
		if (_bColored)
			*this << char(0x1B) << "[m";
		break;
	case logInf:
		if (_bColored)
			*this << char(0x1B) << "[1;32m";
		*this << ":I:";
		if (_bColored)
			*this << char(0x1B) << "[m";
		break;
	case logWarning:
		if (_bColored)
			*this << char(0x1B) << "[1;31m";
		*this << ":W:";
		break;
	case logError:
		if (_bColored)
			*this << char(0x1B) << "[1;38;5;124m";
		*this << ":E:";
		break;
	default:
		break;
	}
	const char* fname;
	if (_levelFunction)
	{
		mthrid_vstr::const_iterator ithr = _instance->_functionName.find(GetThreadId(GetCurrentThread()));
		fname = ithr != _functionName.end() && ithr->second.size() ? ithr->second.back() : NULL;
	}
	else
		fname = _sfname;

	if (fname)
	{
		if (_bColored)
			*this << char(0x1B) << "[1;38;5;30m";
		*this << fname;
		if (_bColored)
			*this << char(0x1B) << "[m";
		*this << ": ";
		if (_bColored && (type == logWarning || type == logError))
			*this << char(0x1B) << (type == logWarning ? "[1;31m" : "[1;38;5;124m");
	}
}


void log::getThreadId()
{
	if (_bColored)
		*this << char(0x1B) << "[1;38;5;202m";
	*this << GetThreadId(GetCurrentThread());
	if (_bColored) 
		*this << char(0x1B) << "[m";
}


log& log::trace(log_type type, LPCSTR lpszFormat, ...)
{
	_cs.Lock();

	if (_logLevel < type)
	{
		_cs.Unlock();
		return *this;
	}

	if (_bPrintDate)
		printDate();
	getThreadId();
	printType(type);

	char buffer[1024 * 16];
	va_list argptr;
	va_start(argptr, lpszFormat);
	vsprintf_s(buffer, lpszFormat, argptr);
	va_end(argptr);
	*this << buffer;
	this->flush();

#ifdef DEBUG
	if (_logLevel == logWTrace)
	{
		if (100 < strlen(buffer))
			buffer[99] = '\0';
		AfxTrace(CString(buffer));
	}
#endif // DEBUG

	_cs.Unlock();
	return *this;
}



log& log::traceN(LPCSTR str, int n, LPCSTR str2)
{
	_cs.Lock();
	if (str)
		*this << str;
	for (int i = 0; i < n; ++i)
		*this << char('0' + char(i % 10));
	if (str2)
		*this << str2;
	_cs.Unlock();
	return *this;
}




log& log::trace(log_type type, const std::stringstream& ss)
{
	_cs.Lock();

	if (_logLevel < type)
	{
		_cs.Unlock();
		return *this;
	}

	if (_bPrintDate)
		printDate();
	printType(type);

	*this << ss.str();

#ifdef DEBUG
	if (_logLevel == logWTrace)
	{
		CString str(ss.str().c_str());
		str.Truncate(160);
		AfxTrace(str);
	}
#endif // DEBUG

	_cs.Unlock();
	return *this;
}


log& log::trace(log_type type, const std::string& str)
{
	_cs.Lock();

	if (_logLevel < type)
	{
		_cs.Unlock();
		return *this;
	}

	if (_bPrintDate)
		printDate();
	printType(type);

	*this << str;

#ifdef DEBUG
	if (_logLevel == logWTrace)
	{
		CString str(str.c_str());
		str.Truncate(160);
		AfxTrace(str);
	}
#endif // DEBUG

	_cs.Unlock();
	return *this;
}



}
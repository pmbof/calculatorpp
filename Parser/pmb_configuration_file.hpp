#include "stdafx.h"
#pragma once

#include "pmb_configuration_file.h"


namespace pmb
{





template<class SYMBOL, class CALC>
configuration_file<SYMBOL, CALC>::preprocessor::preprocessor(const std::string& cmd, char separator)
	: _cmd(cmd)
{
	for (int i = 0, i0 = 0; i < cmd.size(); ++i)
	{
		if (_cmd[i] == ' ' || _cmd[i] == '\t' || i + 1 == cmd.size())
		{
			if (0 < size() && _cmd[i] == separator)
			{
				back().second = i - back().first;
				for (i0 = ++i; i < cmd.size() && _cmd[i] == ' ' || _cmd[i] == '\t'; ++i)
					;
				if (i0 == i || cmd.size() <= i0)
					continue;
			}
			else if (i + 1 == cmd.size())
				++i;
			
			if (i0 < i && (!i0 && (_cmd[i0] != ' ' && _cmd[i0] != '\t') || 0 < i0))
				push_back(prpair(i0, i - i0));
			
			for (++i; i < cmd.size() && (_cmd[i] == ' ' || _cmd[i] == '\t'); ++i)
				;
			i0 = i;
			if (cmd.size() <= i + 1)
				--i;
		}
	}
}



template<class SYMBOL, class CALC>
inline bool configuration_file<SYMBOL, CALC>::preprocessor::preprocess()
{
	bool bcp = 0 < _cmd.size() && _cmd[0] == '#' && !empty();
	if (bcp)
	{
		prpair& p = at(0);
		p.first = 1;
		--p.second;
	}
	return bcp && 1 < _cmd.size() && (_cmd[1] != ' ' && _cmd[1] != '\t');
}



template<class SYMBOL, class CALC>
bool configuration_file<SYMBOL, CALC>::preprocessor::compare(int nElement, const char* val, int szval, char separator, bool bMandatory) const
{
	if (size() <= nElement)
		return false;
	prpair p = separator != '\0' ? _clean(nElement, separator) : at(nElement);
	if (separator != '\0' && bMandatory && at(nElement).second == p.second)
		return false;
	return p.second == szval && !strncmp(_cmd.c_str() + p.first, val, szval);
}




template<class SYMBOL, class CALC>
void configuration_file<SYMBOL, CALC>::preprocessor::clean(int nElement, char separator)
{
	bool bc = separator == '\0';
	prpair& p = at(nElement);
	int i;
	for (i = p.first + p.second; p.first <= i && (_cmd[i] == ' ' || _cmd[i] == '\t' || !bc && _cmd[i] == separator); --i)
	{
		if (_cmd[i] == separator)
			bc = true;
	}
	p.second = i - p.first;
}



template<class SYMBOL, class CALC>
inline typename configuration_file<SYMBOL, CALC>::prpair configuration_file<SYMBOL, CALC>::preprocessor::_clean(int nElement, char separator) const
{
	bool bc = separator == '\0';
	prpair p = at(nElement);
	int i;
	for (i = p.first + p.second - 1; p.first <= i && (_cmd[i] == ' ' || _cmd[i] == '\t' || !bc && _cmd[i] == separator); --i)
	{
		if (_cmd[i] == separator)
			bc = true;
	}
	p.second = i - p.first + 1;
	return p;
}



template<class SYMBOL, class CALC>
inline CStringA configuration_file<SYMBOL, CALC>::preprocessor::operator()(int nElement, int nElements, char separator) const
{
	prpair p = separator != '\0' && (nElements == 1 || size() - nElement == 1) ? _clean(nElement, separator) : at(nElement);
	CStringA str(_cmd.c_str() + p.first, p.second);
	if (nElements != 1)
	{
		for (int n = nElement + 1; n < size() && (n < nElement + nElements || nElements < 0); ++n)
		{
			p = separator != '\0' && (n + 1 == nElement + nElements || n + 1 == size()) ? _clean(n, separator) : at(n);
			str += ' ';
			str += CStringA(_cmd.c_str() + p.first, p.second);
		}
	}
	return str;
}



template<class SYMBOL, class CALC>
inline bool configuration_file<SYMBOL, CALC>::preprocessor::is_numeric(int nElement, bool bNegative, bool bDecimal, char separator) const
{
	const prpair p = separator != '\0' ? _clean(nElement, separator) : at(nElement);
	bool bd = !bDecimal;
	int i;
	for (i = p.first + (bNegative && _cmd[p.first] == '-' ? 1 : 0); i < p.first + p.second && ('0' <= _cmd[i] && _cmd[i] <= '9' || !bd && _cmd[i] == '.'); ++i)
		;
	return i == p.first + p.second;
}



template<class SYMBOL, class CALC>
inline bool configuration_file<SYMBOL, CALC>::preprocessor::is_alpha(int nElement, char separator) const
{
	const prpair p = separator != '\0' ? _clean(nElement, separator) : at(nElement);
	int i;
	for (i = p.first; i < p.first + p.second && ('A' <= _cmd[i] && _cmd[i] <= 'Z' || 'a' <= _cmd[i] && _cmd[i] <= 'z' || _cmd[i] == '_'); ++i)
		;
	return i == p.first + p.second;
}



template<class SYMBOL, class CALC>
inline bool configuration_file<SYMBOL, CALC>::preprocessor::is_alphanumeric(int nElement, char separator) const
{
	const prpair p = separator != '\0' ? _clean(nElement, separator) : at(nElement);
	int i;
	for (i = p.first; i < p.first + p.second && ('A' <= _cmd[i] && _cmd[i] <= 'Z' || 'a' <= _cmd[i] && _cmd[i] <= 'z' || _cmd[i] == '_' || p.first < i && '0' <= _cmd[i] && _cmd[i] <= '9'); ++i)
		;
	return i == p.first + p.second;
}



template<class SYMBOL, class CALC>
inline bool configuration_file<SYMBOL, CALC>::preprocessor::has_separator(int nElement, char separator) const
{
	const prpair& p = at(nElement);
	return _cmd[p.first + p.second - 1] == separator;
}

















template<class SYMBOL, class CALC>
void configuration_file<SYMBOL, CALC>::open(const char* filename)
{
	std::fstream::open(filename, std::ios_base::in);
}





template<class SYMBOL, class CALC>
bool configuration_file<SYMBOL, CALC>::process(SYMBOL& symbols, CALC& calculator)
{
	if (bad())
		return false;

	pmb::log* lg = pmb::log::beginFunction(pmb::logDebug, __FUNCTION__);

	_prefix = nullptr;
	_nline = 0;
	_defines = 0;
	_bcomment = false;
	bool bOk = true;

	std::string line;
	for (char ch = get(); bOk && good(); ch = get())
	{
		if (ch == '\0' || ch == '\r' || ch != '\n' && _bcomment)
			continue;
		else if (!_bcomment && 0 < line.size() && ch == '/' && line.back() == '/')
		{
			line.pop_back();
			_bcomment = true;
		}
		else if (ch == '\n')
		{
			_bcomment = false;
			++_nline;

			if (bOk = process(symbols, calculator, line, lg))
				line.clear();
		}
		else if (!_bcomment && (ch != ' ' && ch != '\t' || !line.empty()))
			line += ch;
	}
	if (bOk)
		bOk = process(symbols, calculator, line, lg);

	if (!bOk)
	{
		if (!_defines)
		{
			if (_serror.IsEmpty())
				lg->trace(pmb::logError, "Error in line %d:\n%s\n", _nline, line.c_str());
			else
				lg->trace(pmb::logError, "Error in line %d: %s:\n%s\n", _nline, _serror, line.c_str());
		}
		else
		{
			if (_serror.IsEmpty())
				lg->trace(pmb::logError, "Error in line %d:\n[0]: %s\n[1]: %s\n[2]: %s\n%s\n", _nline, _dstr[0], _dstr[1], _dstr[2], line.c_str());
			else
				lg->trace(pmb::logError, "Error in line %d: %s:\n[0]: %s\n[1]: %s\n[2]: %s\n%s\n", _nline, _dstr[0], _dstr[1], _dstr[2], _serror, line.c_str());
		}
	}


	lg->endFunction(bOk ? pmb::logDebug : pmb::logError);
	return bOk;
}




template<class SYMBOL, class CALC>
bool configuration_file<SYMBOL, CALC>::process(SYMBOL& symbols, CALC& calculator, std::string& line, log* lg)
{
	while (!line.empty() && (line.back() == ' ' || line.back() == '\t'))
		line.pop_back();

	lg->trace(pmb::logError, "process line %d: %s\n", _nline, line.c_str());

	if (line.empty())
		return true;

	preprocessor prpr(line);

	if (prpr.empty())
		return true;

#ifdef _DEBUG
	std::vector<CStringA> vtest;
	for (int i = 0; i < prpr.size(); ++i)
		vtest.push_back(prpr(i));
#endif

	if (prpr.preprocess())
	{
		if (!calculate(symbols, calculator, prpr, line, true))
			return false;

		if (prpr.compare(0, "define", 6))
		{
			_defines = 0;
			if (4 <= prpr.size() && prpr.compare(1, "dimension", 9) && prpr.is_alphanumeric(2, ':') && prpr.is_alphanumeric(3))
			{
				CStringA symbol = prpr(2, 1, ':');
				CStringA name = prpr(3, -1);
				symbols.add_dimension(symbol, name);
			}
			else if (prpr.size() == 6 && prpr.compare(1, "new", 3) && prpr.compare(2, "prefix", 6, ':') && prpr.compare(4, "base", 4, ':') && prpr.is_alphanumeric(3) && prpr.is_numeric(5))
			{
				CStringA name = prpr(3);
				CStringA base = prpr(5);
				_prefix = new prefix_base(atoi(base));
				symbols.add_prefix(name, _prefix);
			}
			else
				return false;
		}
		else if (prpr.compare(0, "set", 3))
		{
			if (3 < prpr.size() && prpr.compare(1, "system", 6) && prpr.compare(2, "constants", 9, ':', false) && 0 < prpr[3].second)
			{
				CStringA systemc = prpr(3, -1);
				_bUnit = false;
				return (_defines = symbols.set_system_constants(systemc) ? 1 : 0) == 1;
			}
			else if (2 < prpr.size() && prpr.compare(1, "system", 6, ':', false) && 0 < prpr[2].second)
			{
				CStringA system = prpr(2, -1);
				_bUnit = true;
				return (_defines = symbols.set_system(system) ? 1 : 0) == 1;
			}
			else
				return false;
		}
		else if (prpr.compare(0, "end", 3))
		{
			if ((2 == prpr.size() || 3 == prpr.size()) && prpr.compare(1, "system", 6))
			{
				if (2 == prpr.size())
					symbols.set_system();
				else if (3 == prpr.size() && prpr.compare(2, "constants", 9))
					symbols.set_system_constants();
				else
					return false;

				_defines = 0;
				for (int i = 0; i < sizeof(_dstr) / sizeof(*_dstr); ++i)
					_dstr[i].Empty();
			}
			else if (1 < prpr.size() && prpr.size() < 3 && prpr.compare(1, "prefix", 6))
				_prefix = nullptr;
			else
				return false;
		}
		else if (2 < prpr.size() && prpr.compare(0, "add", 3) && prpr.compare(1, "system", 6, ':'))
		{
			int sep;
			for (sep = 3; sep < prpr.size() && !prpr.compare(sep, "prefix", 6, ':'); ++sep)
				;
			CStringA system = prpr(2, sep - 2);
			CStringA prefix;
			if (sep + 1 < prpr.size())
				prefix = prpr(sep + 1, -1);
			return symbols.add_system(system, sep + 1 < prpr.size() ? (const char*)prefix : nullptr);
		}
		else
			return false;

	}
	else if (_prefix && prpr.size() == 3 && prpr.has_separator(0, ':') && prpr.has_separator(1, ':') && prpr.is_alpha(1, ':') && prpr.is_numeric(2, true))
	{
		bool bShow;
		if (!prpr.is_alpha(0, ':'))
		{
			prpair& p = prpr[0];
			if (line[p.first] != '-')
				return false;
			++p.first;
			if (!prpr.is_alpha(0, ':'))
				return false;
			bShow = false;
		}
		else
			bShow = true;
		CStringA name = prpr(0, 1, ':');
		CStringA symbol = prpr(1, 1, ':');
		CStringA exp = prpr(2);
		_prefix->insert(symbol, atoi(exp), name, bShow);
	}
	else
		return calculate(symbols, calculator, prpr, line);
	return true;
}



template<class SYMBOL, class CALC>
inline bool configuration_file<SYMBOL, CALC>::calculate(SYMBOL& symbols, CALC& calculator, const preprocessor& prpr, const std::string& line, bool bDefault)
{
	if (0 < _defines)
	{
		bool bend = prpr.has_separator(prpr.size() - 1, ':');

		if (_defines == 1)
		{
			_dstr[0] += prpr(0, -1, ':');
			if (bend)
			{
				++_defines;
				_dstr[1].Empty();
				if (_bUnit && 1 < prpr.size() && prpr.has_separator(prpr.size() - 2, ':'))
				{
					_dstr[0] = prpr(0, prpr.size() - 1, ':');
					_dstr[2] = prpr(prpr.size() - 1, -1, ':');
				}
			}
		}
		else
		{
			if (bend || bDefault)
			{
				bool bOk;
				try
				{
					if (_bUnit)
					{
						bOk = true;
						bool bUnit = _dstr[0][0] != '-';
						if (!bUnit)
							_dstr[0] = _dstr[0].Mid(1);
						calculator.add_unit(_dstr[0], _dstr[1], _dstr[2].IsEmpty() ? nullptr : (LPCSTR)_dstr[2], bUnit);
					}
					else
						bOk = calculator.add_constant(_dstr[0], _dstr[1]);
				}
				catch (pmb::parser::exception<item>& ex)
				{
					_serror = ex.message((LPCSTR)_dstr[1]).c_str();
					bOk = false;
				}

				if (!bOk)
					return false;

				if (bDefault)
					_defines = 0;
				else
				{
					if (_bUnit && 1 < prpr.size() && prpr.has_separator(prpr.size() - 2, ':'))
					{
						_dstr[0] = prpr(0, prpr.size() - 1, ':');
						_dstr[2] = prpr(prpr.size() - 1, -1, ':');
					}
					else
					{
						_dstr[0] = prpr(0, -1, ':');
						_dstr[2].Empty();
					}
					_dstr[1].Empty();
				}
			}
			else
			{
				if (!_dstr[1].IsEmpty())
					_dstr[1] += ' ';
				_dstr[1] += line.c_str();
			}
		}
		bDefault = true;
	}
	return bDefault;
}



}
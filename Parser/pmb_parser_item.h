#pragma once


#include <string>


namespace pmb
{
namespace parser
{



template <typename _CHAR, typename _SIZE>
class item
{
public:
	typedef typename _CHAR CHAR;
	typedef const _CHAR* _CPTRCHAR;
	typedef typename _SIZE SIZETP;

public:
	class string
	{
	public:
		typedef typename _CHAR tpChar;
		typedef typename SIZETP tpSize;

	public:
		string();
		string(_CPTRCHAR str, SIZETP size);

		_CHAR operator[] (SIZETP i) const;
		operator const _CHAR*() const;
		operator _SIZE() const;
		string operator+(SIZETP right) const;

		std::string getString() const;

		bool operator <(const std::string& right) const;
		bool operator >(const std::string& right) const;

		void clear();
		bool operator!() const;
		operator bool() const;

		_CPTRCHAR str() const;
		SIZETP size() const;
	private:
		_CPTRCHAR _str;
		SIZETP _size;

		template <typename _CHAR, typename _SIZE>
		friend inline bool operator <(const std::string& left, typename const item<_CHAR, _SIZE>::string& right);
	};


public:
	item(_SIZE ini, _SIZE end);

	_SIZE len() const;

	string getString(_CPTRCHAR expr) const;
	_CPTRCHAR getCharPtr(_CPTRCHAR expr) const;

	_SIZE getIni() const;
	_SIZE getEnd() const;

protected:
	_SIZE _ini;
	_SIZE _end;
};




}
}
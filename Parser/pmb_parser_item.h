#pragma once


#include <string>


namespace pmb
{
namespace parser
{




class item
{
public:
	class string
	{
	public:
		string() : _str(NULL) { }
		string(const char* str, unsigned int size) : _str(str), _size(size) { }

		char operator[] (unsigned int i) const {
			return i < _size ? _str[i]: '\0';
		}
		operator const char*() const {
			return _str;
		}
		operator int() const {
			return _size;
		}
		string operator +(unsigned int right) const {
			return string(_str + right, _size - right);
		}

		std::string getString() const {
			return std::string(_str, _size);
		}
	private:
		const char* _str;
		unsigned int _size;

		friend bool operator <(const string& left, const std::string& right);
		friend bool operator <(const std::string& left, const string& right);
	};


public:
	item(int ini, int end);

	int len() const;

	string getString(const char* expr) const;
	const char* getCharPtr(const char* expr) const;

	int getIni() const;
	int getEnd() const;

protected:
	int _ini;
	int _end;
};



}
}
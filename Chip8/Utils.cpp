#include "Utils.hpp"

bool is_hex_number(const std::string str)
{
	if (str[0] == '0' && str[1] == 'x')
	{
		if (str.find_first_not_of("x0123456789abcdefABCDEF") == std::string::npos)
			return true;
		return false;
	}
	return false;
}

bool is_dec_number(const std::string str)
{
	if (str.find_first_not_of("0123456789") == std::string::npos)
		return true;
	return false;
}

bool is_register_read(const std::string str)
{
	return (str[0] == 'V' && isxdigit(str[1]));
}

bool is_memory_addr(const std::string str)
{
	if (str[0] != '[' || str[str.size() - 1] != ']')
		return false;
	// TODO: Think about this
	printf("\"%s\"", str.substr(1, str.size() - 2).c_str());
	if (is_hex_number(str.substr(1, str.size() - 2)) && str.size() <= 10)
		return true;

	return false;
}

bool is_byte(const std::string str)
{
	if (str[0] == '0' && str[1] == 'x')
		return is_hex_number(str) && str.size() < 5;
	else return false;
}

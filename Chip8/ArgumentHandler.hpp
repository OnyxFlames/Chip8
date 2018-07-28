#pragma once

#include <string>
#include <vector>

struct Flag_Struct
{
	bool execute = false;
	bool compile = false;

	std::vector<std::string> file_list;
};

class ArgumentHandler
{
private:
	std::string prog_name = "";
	std::vector<std::string> arguments;
public:
	ArgumentHandler(int argc, char* argv[]);
	~ArgumentHandler();
};


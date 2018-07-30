#pragma once

#include <string>
#include <vector>

struct Flag_Struct
{
	bool execute = false;
	bool compile = false;

	std::string output_name = "output";
	std::vector<std::string> file_list;
};

class ArgumentHandler
{
private:
	std::string prog_name = "";
	std::vector<std::string> arguments;
	Flag_Struct _flags;
public:
	ArgumentHandler(int argc, char* argv[]);
	~ArgumentHandler();
	void eval();
	Flag_Struct& flags() { return _flags; }
};


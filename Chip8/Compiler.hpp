#pragma once

#include <string>
#include <vector>

#include "ArgumentHandler.hpp"



class Compiler
{
private:
	Flag_Struct _flags;
	std::vector<std::string> tokens;
	std::string output_filename;
	std::vector<char> data;
	void write(std::vector<int> _data);
public:
	Compiler(Flag_Struct& flags);
	~Compiler();
	void lex();
	void compile();
	void emit();
	template<typename T>
	std::vector<char> read(T t);
};

template<typename T>
inline std::vector<char> Compiler::read(T t)
{
	std::vector<char> ret;
	for (size_t i = 0; i < sizeof(T) - 1; i++)
	{
		ret.push_back(t & ((0xFF * sizeof(T) - (char)i)));
	}
	return ret;
}
#include "Compiler.hpp"

#include <fstream>
#include <iostream>

#include "Utils.hpp"

void Compiler::write(std::vector<int> _data)
{
	for (const auto& d : _data)
		data.push_back((char)d);
}

Compiler::Compiler(Flag_Struct & flags)
	:
	_flags(flags)
{
	output_filename = flags.output_name;
}

Compiler::~Compiler()
{
}

void Compiler::lex()
{
	std::ifstream input(_flags.file_list[0]);
	if (!input.is_open())
	{
		printf("Failed to open file '%s'\n", _flags.file_list[0].c_str());
		std::exit(1);
	}
	std::string buffer;
	while (std::cin >> buffer)
	{
		tokens.push_back(buffer);
	}
	
}

void Compiler::compile()
{
	// Give us some speed with compiling files that end up 3x larger than the binary(a rough estimate)
	data.reserve(tokens.size() % 3);
	for (size_t i = 0; i < tokens.size(); i++)
	{
		std::string s = tokens[i];
		if (s == "SYS")
		{
			write
			( 
				{ 0x00, 0xE0 }
			);
		}
		else if (s == "RET")
		{
			write
			(
				{ 0x00, 0xEE }
			);
		}
		else if (s == "JP")
		{
			s = tokens[++i];
			if (is_hex_number(s))
			{
				uint16_t val = 0x1000;
				val += std::stoi(s) & 0x00000FFF;
				write
				(
					{ (val & 0xFF00) >> 8, val & 0x00FF }
				);
			}
		}
		else if (s == "CALL")
		{
			s = tokens[++i];
			if (is_hex_number(s))
			{
				uint16_t val = 0x2000;
				val += std::stoi(s) & 0x00000FFF;
				write
				(
					{ (val & 0xFF00) >> 8, val & 0x00FF }
				);
			}
		}
		else if (s == "SE")
		{
			std::string s1 = tokens[++i].substr(s1.find(','));
			std::string s2 = tokens[++i];

			if (is_register_read(s1) && is_byte(s2))
			{

			}

		}

	}
}

void Compiler::emit()
{
}

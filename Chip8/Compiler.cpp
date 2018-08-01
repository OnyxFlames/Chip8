#include "Compiler.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "Utils.hpp"

uint8_t decode_register(uint8_t r);


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
	while (input >> buffer)
	{
		tokens.push_back(buffer);
	}
	
}

void Compiler::compile()
{
	// Give us some speed with compiling files that end up 3x larger than the binary(a rough estimate)
	data.reserve(tokens.size() % 3);
	std::cout << std::hex << std::setw(5) << std::setfill('0');
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
			else
			{
				std::cerr << "CALL opcode does not support operands other than a hexidecimal number in the implementation.\n";
				return;
			}
		}
		else if (s == "SE")
		{
			std::string s1 = tokens[++i].find(',') != std::string::npos 
				? tokens[i].substr(0, tokens[i].find(',')) 
				: tokens[i];
			std::string s2 = tokens[++i];

			if (is_register_read(s1) && is_byte(s2))
			{
				uint8_t r = decode_register(s1[1]);

				uint8_t b1 = 0x30 + (r & 0x0F) ;
				uint8_t b2 = (std::stoi(s2.substr(2), nullptr, 16));
				std::cout << "SE Vx, 0xXX: " << std::hex << std::setw(2) << std::setfill('0') << (short)b1 << (short)b2 << "\n";
				write
				(
					{ b1, b2 }
				);
			}
			else if (is_register_read(s1) && is_register_read(s1))
			{
				//TODO: Fix
				uint8_t b1 = 0x50 + (decode_register(s1[1]) & 0x0F);
				uint8_t b2 = (decode_register(s2[1]) << 4);
				std::cout << "SE Vx, Vx: " << std::hex << std::setw(2) << std::setfill('0') << ((short)b1) << (short)b2 << "\n";
				write
				(
					{ b1, b2 }
				);
			}
			else
			{
				printf("Invalid SE opcode format.\n");
				return;
			}
		}
	}
	return /* exit early*/;
	std::cout << "Binary dump: \n";
	for (size_t i = 0; i < data.size(); i+=2)
	{
		std::cout << std::hex << ((short)data[i] & 0x00FF) << ((short)data[i + 1] & 0x00FF) << "\n";
	}
}

void Compiler::emit()
{
}

uint8_t decode_register(uint8_t r)
{
	std::string buff{ (char)r, '\0' };
	return std::stoi(buff, nullptr, 16) & 0x000F;
}
#include <iostream>
#include <fstream>

#include "ArgumentHandler.hpp"
#include "Chip8.hpp"
#include "Compiler.hpp"
#include "Utils.hpp"

//#define print_func_call(x) printf("Function %s\n", x)
//#define print_func_call(x)


int main(int argc, char* argv[])
{
	ArgumentHandler ah(argc, argv);
	ah.eval();
	Flag_Struct fs = ah.flags();
	std::unique_ptr<Chip8> chip;// ("Chip8 Emulator (Alpha)", 64, 32);
	if (fs.compile)
	{
		Compiler c(fs);
	}
	else if (fs.execute)
	{
		chip = std::make_unique<Chip8>("Chip8 Emulator (Alpha)", 64, 32);
		if (fs.file_list.size() < 1)
		{
			printf("Need file to execute.\n");
			return 1;
		}
		chip->load_rom(fs.file_list[0]);
		chip->run();
	}
	return 0;
}
#include <iostream>
#include <fstream>

#include "Chip8.hpp"

//#define print_func_call(x) printf("Function %s\n", x)
#define print_func_call(x)


int main(int argc, char* argv[])
{
	print_func_call(__FUNCTION__);
	Chip8 chip("Chip8 Emulator (Alpha)", 64, 32);
	if (argc > 1)
	{
		chip.load_font();
		if (chip.load_rom(argv[1]))
			chip.run();
	}
	return 0;
}
#include "Opcodes.hpp"

#include "Chip8.hpp"

/*

#define print_func_call(x) printf("\t%s()\n", x)

void _cls(Chip8* chip)
{
	print_func_call(__FUNCTION__);
}

void _ret(Chip8* chip)
{
	print_func_call(__FUNCTION__);
	if (chip->get_stack().size() == 0)
	{
		std::cerr << "[Runtime Error]: Attempting to return from subroutine with empty stack.\n";
		std::exit(1);
	}
	else
	{
		chip->get_pc() = chip->get_sp()--;
	}
}

void _jp1(Chip8 * chip)
{
	print_func_call(__FUNCTION__);
	//printf("Jumping to %.4X\n", (chip->get_instruction() & 0x0FFF));
	chip->get_pc() = (chip->get_instruction() & 0x0FFF);
}

void _call(Chip8 * chip)
{
	print_func_call(__FUNCTION__);
	chip->get_stack()[++(chip->get_sp())];
	chip->get_pc() = chip->get_instruction() & 0x0FFF;
}

void _se1(Chip8 * chip)
{
	print_func_call(__FUNCTION__);
	uint16_t inst = chip->get_instruction();
	uint16_t x = (inst & 0x0F00) >> 0x8;
	uint16_t byte = inst & 0x00FF;

	if (chip->get_registers()[x] == byte)
		chip->get_pc() += 2;
	chip->get_pc() += 2;
}

void _sne1(Chip8 * chip)
{
	print_func_call(__FUNCTION__);

}

void _se2(Chip8 * chip)
{
	print_func_call(__FUNCTION__);
}

void _ld1(Chip8 * chip)
{
}

void _add1(Chip8 * chip)
{
}

void _ld2(Chip8 * chip)
{
}

void _or(Chip8 * chip)
{
}

void _and(Chip8 * chip)
{
}

void _xor(Chip8 * chip)
{
}

void _add2(Chip8 * chip)
{
}

void _sub(Chip8 * chip)
{
}

void _shr(Chip8 * chip)
{
}

void _subn(Chip8 * chip)
{
}

void _shl(Chip8 * chip)
{
}

void _sne2(Chip8 * chip)
{
}

void _ld3(Chip8 * chip)
{
}

void _jp2(Chip8 * chip)
{
}

*/
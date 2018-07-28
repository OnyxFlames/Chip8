#include "Chip8.hpp"

#include <filesystem>

//#define print_func_call(x) printf("Function %s\n", x)/#define print_func_call(x)
#define print_func_call(x)

long handle_fps(sf::Time dt);

namespace fs = std::experimental::filesystem;

static std::array<uint8_t, 80> default_char =
{
	0xF0 ,0x90, 0x90, 0x90, 0xF0, //0
	0x20, 0x60, 0x20, 0x20, 0x70, //1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
	0x90, 0x90, 0xF0, 0x10, 0x10, //4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
	0xF0, 0x10, 0x20, 0x40, 0x40, //7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
	0xF0, 0x90, 0xF0, 0x90, 0x90, //A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
	0xF0, 0x80, 0x80, 0x80, 0xF0, //C
	0xE0, 0x90, 0x90, 0x90, 0xE0, //D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
	0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

Chip8::Chip8(const std::string title, unsigned width, unsigned height)
	:	
	window(sf::VideoMode(pixel_size * width, pixel_size * height), title)
{
	print_func_call(__FUNCTION__);
	debugfont.loadFromFile("../resources/fonts/consola.ttf");
	debugtext.setFont(debugfont);
	debugtext.setString("");
}

Chip8::Chip8()
	: 
	window(sf::VideoMode(pixel_size * 64, pixel_size * 32), "Chip8 Emulator(Alpha)")
{
	print_func_call(__FUNCTION__);
	debugfont.loadFromFile("../resources/fonts/consola.ttf");
	debugtext.setFont(debugfont);
	debugtext.setString("");
}


Chip8::~Chip8()
{
}

bool Chip8::load_rom(const std::string romfile)
{
	print_func_call(__FUNCTION__);
	std::ifstream input(
		romfile, 
		std::ios::binary | 
		std::ios::ate);
	if (!input.is_open())
	{
		std::cerr << "Failed to open ROM '" << fs::canonical(fs::path(romfile)) << "'.\n";
		return false;
	}
	std::cout << "Loading \"" << fs::canonical(fs::path(romfile)) << "\"\n";
	std::ifstream::pos_type pos = input.tellg();// +(std::streampos)0x200;
	std::vector<char> buffer((unsigned)pos);
	input.seekg(0, std::ios::beg);
	input.read(&buffer[0], (unsigned)pos);
	ROM = buffer;
	std::cout << "Loaded " << ROM.size() << " bytes.\n";
	return true;
}

bool Chip8::load_font()
{
	for (size_t i = 0; i < default_char.size(); i++)
		memory[i] = default_char[i];
	//printf("Loaded Chip8 font (%u bytes)\n", default_char.size());
	return true;
}

void Chip8::run()
{
	print_func_call(__FUNCTION__);
	sf::Clock clock;
	while (window.isOpen())
	{
		dispatch();
		update(clock.restart());
		render();
	}
}

void Chip8::dispatch()
{
	print_func_call(__FUNCTION__);
	sf::Event e;
	while (window.pollEvent(e))
	{
		switch (e.type)
		{
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::KeyPressed:
			switch (e.key.code)
			{
			case sf::Keyboard::Escape:
				window.close();
				break;
			}
		}
	}
}

void Chip8::update(sf::Time dt)
{
	print_func_call(__FUNCTION__);
	handle_fps(dt);
	debugtext.setString(std::to_string(handle_fps(dt)));

	fetch();
	execute();
}

void Chip8::fetch()
{
	print_func_call(__FUNCTION__);
	if (pc == ROM.size())
		window.close();
	instruction = ROM[pc++];
	instruction <<= 8;
	instruction += ROM[pc++];
}

void Chip8::execute()
{
	print_func_call(__FUNCTION__);
	//printf("Instruction: %.4X [Sig: %.4X] \n", instruction, (instruction & 0xF000));
	switch (instruction & 0xF000)
	{
	case 0x0000:
	{
		//printf("Branch [%.2X]\n", instruction & 0xFF00 >> 16);
		switch (instruction & 0x00FF)
		{
			// CLS - 00E0
		case 0xE0:
			for (auto& it : graphics)
			{
				it = false;
			}
			break;
			// RET - 00EE
		case 0xEE:
			if (stack_pointer == 0)
			{
				std::cerr << "[Runtime Error]: Attempting to return from subroutine with empty stack.\n";
				std::exit(1);
			}
			else
			{
				pc = stack[stack_pointer--];
			}
				break;
		}
	}
		break;
	case 0x1000:
		pc = (instruction & 0x0FFF);
		break;
	case 0x2000:
		stack[++stack_pointer] = pc;
		pc = instruction & 0x0FFF;
		break;
	case 0x3000:
	{
		int8_t byte = instruction & 0x00FF;
		int8_t x = (instruction >> 8) & 0x000F;
		if (x == byte)
			pc += 2;
		pc += 2;
		break;
	}
	case 0x4000:
	{
		int8_t byte = instruction & 0x00FF;
		int8_t x = (instruction >> 8) & 0x000F;
		if (x != byte)
			pc += 2;
		pc += 2;
		break;
	}
	case 0x5000:
	{
		if ((instruction & 0x000F) == 0)
		{
			int8_t byte = instruction & 0x00FF;
			int8_t x = (instruction >> 8) & 0x000F;
			if (x != byte)
				pc += 2;
			pc += 2;
		}
		else
		{
			printf("Opcode 0x%.4X is not supported!\n", instruction);
		}
		break;
	}
	default:
		printf("\tIgnoring instruction 0x%.4X - Not implemented.\n", instruction);
		break;
	}
}

void Chip8::render()
{
	print_func_call(__FUNCTION__);
	window.clear();
	window.draw(debugtext);
	window.display();
}

long handle_fps(sf::Time dt)
{
	print_func_call(__FUNCTION__);
	static long fps = 0, ret = 0;
	static sf::Time duration;
	duration += dt;
	if (duration >= sf::seconds(1))
	{
		std::cout << "FPS: " << fps << "\n";
		ret = fps;
		fps = 0;
		duration = sf::seconds(0);
	}
	fps++;
	return ret;
}
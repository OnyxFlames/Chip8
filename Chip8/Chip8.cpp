#include "Chip8.hpp"

#include <filesystem>






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

uint8_t sfmltochip(sf::Keyboard::Key key)
{

	return 0;
}


sf::Keyboard::Key chiptosfml(uint8_t key)
{
	switch (key)
	{
	case 0x1: return sf::Keyboard::Num1;
	case 0x2: return sf::Keyboard::Num2;
	case 0x3: return sf::Keyboard::Num3;
	case 0xc: return sf::Keyboard::Num4;
	case 0x4: return sf::Keyboard::Q;
	case 0x5: return sf::Keyboard::W;
	case 0x6: return sf::Keyboard::E;
	case 0xD: return sf::Keyboard::R;
	case 0x7: return sf::Keyboard::A;
	case 0x8: return sf::Keyboard::S;
	case 0x9: return sf::Keyboard::D;
	case 0xE: return sf::Keyboard::F;
	case 0xA: return sf::Keyboard::Z;
	case 0x0: return sf::Keyboard::X;
	case 0xB: return sf::Keyboard::C;
	case 0xF: return sf::Keyboard::V;
	}
	return sf::Keyboard::Escape;
}

Chip8::Chip8(const std::string title, unsigned width, unsigned height)
	:	
	window(sf::VideoMode(pixel_size * width, pixel_size * height), title),
	base_title(title.c_str())
{
	print_func_call(__FUNCTION__);
	init();
}

Chip8::Chip8()
	: 
	window(sf::VideoMode(pixel_size * 64, pixel_size * 32), "Chip8 Emulator (Alpha)"),
	base_title("Chip8 Emulator (Alpha)")
{
	print_func_call(__FUNCTION__);
	init();
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
	std::ifstream::pos_type pos = input.tellg();
	std::vector<char> buffer((unsigned)pos);
	input.seekg(0, std::ios::beg);
	input.read(&buffer[0], (unsigned)pos);
	ROM = buffer;
	std::cout << "Loaded " << ROM.size() << " bytes.\n";
	window.setTitle(base_title + " - " + fs::path(romfile).filename().string());
	return true;
}

bool Chip8::load_font()
{
	print_func_call(__FUNCTION__);
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
	//handle_fps(dt);
	fpstext.setString(std::to_string(handle_fps(dt)));
#ifdef DEBUG
	// Doesn't work.
	/*static size_t list_count = 0;
	std::stringstream ss;
	if (list_count++ > 15)
	{
		ss << std::hex << instruction << "\n";
		list_count = 0;
		opcodetext.setString(ss.str());
	}
	else
		opcodetext.setString(ss.str());*/
#endif
	fetch();
	execute();
}

void Chip8::fetch()
{
	print_func_call(__FUNCTION__);
	if (pc >= ROM.size())
		window.close();
	instruction = ROM[pc++];
	instruction <<= 8;
	instruction += ROM[pc++];
}

void Chip8::execute()
{
	print_func_call(__FUNCTION__);
	switch (instruction & 0xF000)
	{
		case 0x0000:
		{
			//printf("Branch [%.2X]\n", instruction & 0xFF00 >> 8);
			switch (instruction & 0x00FF)
			{
				// CLS - 00E0
			case 0x00E0:
				OP("CLS", instruction);
				for (auto& it : graphics)
				{
					it = false;
				}
				break;
				// RET - 00EE
			case 0x00EE:
				OP("RET", instruction);
#ifdef DEBUG
				if (stack_pointer == 0)
				{
					std::cerr << "[Runtime Error]: Attempting to return from subroutine with empty stack.\n";
					std::exit(1);
				}
				else
				{
					pc = stack[stack_pointer--];
				}
#else
				pc = stack[stack_pointer--];
#endif
					break;
			}
		}
			break;
		case 0x1000:
		{
			OP("JP", instruction);
			pc = (instruction & 0x0FFF);
			break; 
		}
		case 0x2000:
		{
			OP("CALL", instruction);
			stack[stack_pointer++] = pc;
#ifdef DEBUG
			if ((size_t)(instruction & 0x0FFF) > ROM.size())
			{
				std::cerr << "[Runtime Error]: Calling subroutine at invalid address 0x0" << std::hex << std::uppercase << (instruction & 0x0FFF)
					<< "\n(Offset: " << std::dec << (unsigned short)(instruction & 0x0FFF) - ROM.size() << ")\n";
				std::exit(1);
			}
#endif
			pc = (instruction & 0x0FFF);
			break; 
		}
		case 0x3000:
		{
			OP("SE", instruction);
			int8_t byte = instruction & 0x00FF;
			int8_t x = (instruction >> 8) & 0x000F;
			if (x == byte)
				pc += 2;
			break;
		}
		case 0x4000:
		{
			OP("SNE", instruction);
			int8_t byte = instruction & 0x00FF;
			int8_t x = (instruction >> 8) & 0x000F;
			if (x != byte)
				pc += 2;
			break;
		}
		case 0x5000:
		{
			if ((instruction & 0x000F) == 0)
			{
				OP("SE", instruction);
				int8_t byte = instruction & 0x00FF;
				int8_t x = (instruction >> 8) & 0x000F;
				if (x != byte)
					pc += 2;
			}
			else
			{
				NoSupp(instruction);
			}
			break;
		}
		case 0x6000:
		{
			OP("LD", instruction);
			uint8_t x = (instruction & 0x0F00) >> 8;
			uint8_t byte = (instruction & 0x00FF);
			registers[x] = byte;
			break;
		}
		case 0x7000:
		{
			OP("ADD", instruction);
			uint8_t x = (instruction & 0x0F00) >> 8;
			uint8_t byte = (instruction & 0x00FF);
			registers[x] += byte;
			break;
		}
		case 0x8000:
		{
			switch (instruction & 0x000F)
			{
				case 0x0:
				{
					OP("LD", instruction);
					uint8_t x = (instruction & 0x0F00) >> 8;
					uint8_t y = (instruction & 0x00F0) >> 4;
					registers[x] = registers[y];
					break;
				}
				case 0x1:
				{
					OP("OR", instruction);
					uint8_t x = (instruction & 0x0F00) >> 8;
					uint8_t y = (instruction & 0x00F0) >> 4;
					registers[x] |= registers[y];
					break;
				}
				case 0x2:
				{
					OP("AND", instruction);
					uint8_t x = (instruction & 0x0F00) >> 8;
					uint8_t y = (instruction & 0x00F0) >> 4;
					registers[x] &= registers[y];
					break;
				}
				case 0x3:
				{
					OP("XOR", instruction);
					uint8_t x = (instruction & 0x0F00) >> 8;
					uint8_t y = (instruction & 0x00F0) >> 4;
					registers[x] ^= registers[y];
					break;
				}
				case 0x4:
				{
					OP("ADD", instruction);
					uint8_t x = (instruction & 0x0F00) >> 8;
					uint8_t y = (instruction & 0x00F0) >> 4;
					registers[0xF] = ((registers[x] + registers[y]) > 0xFF);
					registers[x] = registers[x] + registers[y];
					break;
				}
				case 0x5:
				{
					OP("SUB", instruction);
					uint8_t x = (instruction & 0x0F00) >> 8;
					uint8_t y = (instruction & 0x00F0) >> 4;
					registers[0xF] = (registers[x] > registers[y]);
					registers[x] = registers[x] - registers[y];
					break;
				}
				case 0x6:
				{
					OP("SHR", instruction);
					uint8_t x = (instruction & 0x0F00) >> 8;
					uint8_t y = (instruction & 0x00F0) >> 4;
					registers[0xF] = (registers[x] & 0x1);
					registers[x] >>= 1;
					break;
				}
				case 0x7:
				{
					OP("SUBN", instruction);
					uint8_t x = (instruction & 0x0F00) >> 8;
					uint8_t y = (instruction & 0x00F0) >> 4;
					registers[0xF] = (registers[y] > registers[x]);
					registers[x] = registers[y] - registers[x];
					break;
				}
				case 0xE:
				{
					OP("SHL", instruction);
					uint8_t x = (instruction & 0x0F00) >> 8;
					uint8_t y = (instruction & 0x00F0) >> 4;
					registers[0xF] = (registers[x] & 0x8);
					registers[x] <<= 1;
					break;
				}
				default:
					NoSupp(instruction);
					break;
			}
			break;
		}
		case 0x9000:
		{
			if ((instruction & 0x000F) == 0)
			{
				OP("SNE", instruction);
				uint8_t x = (instruction & 0x0F00) >> 8;
				uint8_t y = (instruction & 0x00F0) >> 4;
				if (registers[x] != registers[y])
					pc += 2;
			}
			else
				NoSupp(instruction);
			break;
		}
		case 0xA000:
		{
			OP("LD", instruction);
			memory_pointer = instruction & 0x0FFF;
			break;
		}
		case 0xB000:
		{
			OP("LD", instruction);
			pc = registers[0] + (instruction & 0x0FFF);
			break;
		}
		case 0xC000:
		{
			OP("RND", instruction);
			uint8_t x = (instruction & 0x0F00) >> 8;
			uint8_t byte = (instruction & 0x00FF);
			registers[x] = (rand() & byte);
			break;
		}
		case 0xD000:
		{
			// Thanks: https://github.com/Nakrez/chip8/blob/master/src/chip8.cc
			OP("DRW", instruction);
			uint8_t x = registers[(instruction & 0x0F00) >> 8];
			uint8_t y = registers[(instruction & 0x00F0) >> 4];
			uint8_t n = (instruction & 0x000F);
			uint16_t& I = memory_pointer;
			uint16_t p;

			for (size_t i = 0; i < n; ++i)
			{
				p = memory[I + i];
				for (size_t j = 0; j < 8; ++j)
				{
					if ((p & (0x80 >> j)) != 0)
					{
						if (graphics[x + j + ((y + i) * 64)] != 0)
						{
							registers[0xF] = 1;
						}
						graphics[x + j + ((y + i) * 64)] ^= 1;
					}
				}
			}

			
			update_draw = true;
			break;
		}
		case 0xE000:
		{
			if ((instruction & 0xFF) == 0x9E)
			{
				OP("SKP", instruction);
				uint8_t x = (instruction & 0x0F00) >> 8;
				if (sf::Keyboard::isKeyPressed(chiptosfml(registers[x])))
					pc += 2;
			}
			else if ((instruction & 0xFF) == 0xA1)
			{
				OP("SKNP", instruction);
				uint8_t x = (instruction & 0x0F00) >> 8;
				if (!sf::Keyboard::isKeyPressed(chiptosfml(registers[x])))
					pc += 2;
			}
			else
				NoSupp(instruction);
			break;
		}
		case 0xF000:
		{
			switch (instruction & 0x00FF)
			{
				case 0x0007:
				{
					OP("LD", instruction);
					break;
				}
				case 0x000A:
				{
					OP("LD", instruction);
					break;
				}
				case 0x0015:
				{
					OP("LD", instruction);
					break;
				}
				case 0x0018:
				{
					OP("LD", instruction);
					break;
				}
				case 0x001E:
				{
					OP("ADD", instruction);
					break;
				}
				case 0x0029:
				{
					OP("LD", instruction);
					break;
				}
				case 0x0033:
				{
					OP("LD", instruction);
					break;
				}
				case 0x0055:
				{
					OP("LD", instruction);
					break;
				}
				case 0x0065:
				{
					OP("LD", instruction);
					break;
				}
				default:
					NoSupp(instruction);
					break;
			}
			break;
		}
		default:
			NoSupp(instruction);
			break;
	}
}

void Chip8::render()
{

	print_func_call(__FUNCTION__);
	window.clear();
	/*if (update_draw)
	{
		for (size_t i = 0; i < 32; i++)
		{
			for (size_t j = 0; j < 64; j++)
			{
				if (graphics[i * j])
					;
			}
		}
		update_draw = false;
	}*/
	window.draw(fpstext);
	window.draw(opcodetext);
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
		//std::cout << "FPS: " << fps << "\n";
		ret = fps;
		fps = 0;
		duration = sf::seconds(0);
	}
	fps++;
	return ret;
}
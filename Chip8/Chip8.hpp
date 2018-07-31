#pragma once

#include "Definitions.hpp"

#include <SFML/Graphics.hpp>

#include <map>
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>
#include "Opcodes.hpp"


#ifdef DEBUG
#define print_func_call(x) //printf("Function %s\n", x)
#define OP(opstr, addr) printf("%s\t0x%.4X\n", opstr, addr);
#define NoSupp(x) //printf("Opcode 0x%.4X is not supported!\n", x);
#else
#define print_func_call(x)
#define OP(x, a)
#define NoSupp(x)
#endif

class Chip8
{
private:
	const std::string base_title;
	const size_t pixel_size = 16u;
	sf::RenderWindow window;
	std::array<bool, 64 * 32> graphics =	{ 0 };
	std::array<uint8_t, 0x1000> memory =	{ 0 };
	std::array<uint16_t, 16> stack =		{ 0 };
	std::array<int8_t, 0x10> registers = { 0 };
	std::vector<char> ROM;
	uint16_t pc = 0x0000;
	uint16_t memory_pointer = 0x200;
	uint8_t stack_pointer = 0;
	uint16_t instruction = 0;
	sf::Text fpstext, opcodetext;
	sf::Font debugfont;
	sf::Event e;
	void init()
	{
		print_func_call(__FUNCTION__);
		debugfont.loadFromFile("../resources/fonts/consola.ttf");
		fpstext.setFont(debugfont);
		fpstext.setString("");
		fpstext.setPosition(5.f, 5.f);
		//opcodetext = fpstext;
		opcodetext.setFont(debugfont);
		opcodetext.setString("");
		opcodetext.setPosition(window.getSize().x - opcodetext.getGlobalBounds().width, 0);
		srand((unsigned)time(0));
	}
	bool update_draw = true;
public:
	Chip8(const std::string title, unsigned width, unsigned height);
	Chip8();
	~Chip8();
	bool load_rom(const std::string romfile);
	bool load_font();
	void run();
	void dispatch();
	void update(sf::Time dt);
	void fetch();
	void execute();
	void render();
};

#pragma once

#include <SFML/Graphics.hpp>

#include <map>
#include <array>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <functional>

#include "Opcodes.hpp"

//typedef std::function<void(Chip8*)> opcode_t;

class Chip8
{
private:
	const size_t pixel_size = 16u;
	sf::RenderWindow window;
	std::array<bool, 64 * 32> graphics =	{ 0 };
	std::array<uint8_t, 0x1000> memory =	{ 0 };
	std::array<uint16_t, 16> stack =		{ 0 };
	std::array<int8_t, 0xF> registers = { 0 };
	std::vector<char> ROM;
	uint16_t pc = 0x200;
	uint8_t stack_pointer = std::numeric_limits<uint8_t>::max();
	//std::array<opcode_t, 35> opcodes;
	uint16_t instruction = 0;
	sf::Text debugtext;
	sf::Font debugfont;
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
	/*std::array<bool, 64 * 32>& get_graphics() { return graphics; }
	std::array<uint8_t, 0x1000>& get_memory() { return memory; }
	std::array<uint16_t, 16>& get_stack() { return stack; }
	sf::RenderWindow& get_window() { return window; }
	uint16_t& get_instruction() { return instruction; }
	uint16_t& get_pc() { return pc; }
	uint8_t& get_sp() { return stack_pointer; }
	std::array<int8_t, 0xF>& get_registers() { return registers; }
	int8_t& get_register(size_t i) { return registers[i % registers.size()]; }*/
};

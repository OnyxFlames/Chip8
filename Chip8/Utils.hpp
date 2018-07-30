#pragma once

#include <string>

bool is_hex_number(const std::string str);
bool is_dec_number(const std::string str);
bool is_register_read(const std::string str);
bool is_memory_addr(const std::string str);
bool is_byte(const std::string str);
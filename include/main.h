#pragma once
#include <string>

constexpr int INT = 0x8A;
constexpr int BOOL = 0x8B;
constexpr int STR = 0x8C;
constexpr int STR_START = 0x01;
constexpr int STR_END = 0x00;

constexpr int ADD = 0x8D;
constexpr int SUB = 0x8F;

constexpr int PRT = 0x9A;
constexpr int ADD_RANDOM = 0x9F;

struct Variable
{
	unsigned char type;
	unsigned char name;
	const char* string;
	unsigned char value;
};

const char* get_string(int pos, const std::vector<unsigned char>& buffer);

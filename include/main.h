#pragma once
#include <string>

#define INT 0x8A
#define BOOL 0x8B
#define STR 0x8C
#define STR_START 0x01
#define STR_END 0x00

#define ADD 0x8D
#define SUB 0x8F

#define PRT 0x9A
#define ADD_RANDOM 0x9F

struct Variable
{
	unsigned char Type;
	unsigned char Name;
	unsigned char Value;
};

struct String
{
	unsigned char Type = STR;
	unsigned char Name;
	std::string Value;
};

std::string get_string(int pos, const std::vector<unsigned char>& buffer);

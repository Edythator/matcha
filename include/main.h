#pragma once
#include <vector>

enum Opcode
{
	INT,
	BOOL,
	STR,
	ADD,
	SUB,
	AI,
	PRT,
	PRT_STR,
	ADD_RANDOM
};

struct Variable
{
	Opcode type;
	unsigned char name;
	const char* string;
	unsigned char value;
};

const char* get_string(int pos, const std::vector<unsigned char>& buffer);

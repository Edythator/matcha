#include <stdio.h>
#include <vector>
#include "../include/main.h"

const char* get_string(int pos, const std::vector<unsigned char>& buffer)
{
	int length = 0;
	size_t size = buffer.size();

	for (size_t j = pos + STR_START; j < size; j++)
		if (buffer[j] != STR_END)
			length++;

	return (const char*)(buffer.data() + pos + 1);
}

int main(int argc, char* argv[])
{
	std::vector<Variable> variables;
	//std::vector<unsigned char> buffer = { 0x13, 0x37, ADD_RANDOM, 0x31, PRT_STR, 0x74, 0x6A, 0x6F, 0x20, 0x67, 0x72, 0x61, 0x62, 0x62, 0x61, 0x72, STR_END, ADD_RANDOM, 0x1A };
	std::vector<unsigned char> buffer = {
		0x13, 0x37,
		INT, 0x10, 0x23,
		BOOL, 0x11, 0x1,
		STR, 0x12, STR_START, 0x6A, 0x20, 0x72, STR_END,
		PRT, STR_START, 0x74, 0x6A, 0x6F, 0x20, 0x67, 0x72, 0x61, 0x62, 0x62, 0x61, 0x72, STR_END,
		PRT, 0x10
	};


	// checksum
	if (buffer[0] != 0x13 || buffer[1] != 0x37)
		return 0;

	for (size_t i = 2; i < buffer.size(); i++)
	{
		switch (buffer[i])
		{
		case INT:
			variables.push_back(Variable{ INT, buffer[i + 1], "", buffer[i + 2]});
			i += 2;
			break;
		case BOOL:
			variables.push_back(Variable{ BOOL, buffer[i + 1], "", buffer[i + 2]});
			i += 2;
			break;
		case STR:

			variables.push_back(Variable{ STR, buffer[i + 1], get_string(i + 2, buffer) }); 
			break;

		case ADD: // ADD INT variable, INT value
		{
			int name = buffer[i + 1];
			int value = buffer[i + 2];

			for (size_t i = 0; i < variables.size(); i++)
			{
				if (variables[i].name == name)
				{
					variables[i].value += value;
					break;
				}
			}
			i += 2;
			break;
		}

		case SUB: // SUB INT variable, INT value
		{
			int name = buffer[i + 1];
			int value = buffer[i + 2];

			for (size_t i = 0; i < variables.size(); i++)
			{
				if (variables[i].name == name)
				{
					variables[i].value -= value;
					break;
				}
			}
			i += 2;
			break;
		}

		case PRT: // PRT VAR variable
		{
			if (buffer[i + 1] == STR_START)
			{
				std::string str = get_string(i + 1, buffer);
				printf("%s\n", str.c_str());
				i += str.size();
			}
			else
			{
				printf("%x\n", buffer[i + 1]);
				i += 2;
			}
			break;
		}

		case ADD_RANDOM: // ADD_RANDOM INT variable
		{
			int val = buffer[i + 1] + (rand() % INT_MAX);
			i++;
			break;
		}

		}
	}
	return 0;
}
#include "../include/main.h"
#include <stdio.h>

const char* get_string(int pos, const std::vector<unsigned char>& buffer)
{
	int length = 0;
	size_t size = buffer.size();

	for (size_t j = pos; j < size; j++)
		if (buffer[j] != 0x00)
			length++;

	return (const char*)(buffer.data() + pos + 1);
}

int main(int argc, char* argv[])
{
	std::vector<Variable> variables;
	std::vector<unsigned char> buffer = {
		0x13, 0x37,
		INT, 0x10, 0x23,
		BOOL, 0x11, 0x1,
		STR, 0x12, 0x6A, 0x20, 0x72, 0x00,
		PRT_STR, 0x74, 0x6A, 0x6F, 0x20, 0x67, 0x72, 0x61, 0x62, 0x62, 0x61, 0x72, 0x00,
		PRT, 0x10
	};

	// magic
	if (buffer[0] != 0x13 || buffer[1] != 0x37)
		return 0;

	for (size_t i = 2; i < buffer.size(); )
	{
		switch (buffer[i])
		{
		case INT:
		{
			variables.push_back(Variable{ INT, buffer[i + 1], NULL, buffer[i + 2] });
			i += 3;
			break;
		}

		case BOOL:
		{
			variables.push_back(Variable{ BOOL, buffer[i + 1], NULL, buffer[i + 2] });
			i += 3;
			break;
		}

		case STR:
		{
			const char* str = get_string(i + 1, buffer);
			variables.push_back(Variable{ STR, buffer[i + 1], str });
			i += strlen(str) + 3;
			break;
		}

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
			i += 3;
			break;
		}

		case PRT: // PRT VAR variable
		{
			printf("%x\n", buffer[i + 1]);
			i += 3;
			break;
		}

		case PRT_STR: // PRT_STR STR string
		{
			const char* str = get_string(i, buffer);
			printf("%s\n", str);
			i += strlen(str) + 2;
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
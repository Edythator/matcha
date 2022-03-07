#include "../include/main.h"
#include <stdio.h>
#include <string.h>

#define ARR_SIZE(x) sizeof(x)/sizeof(x[0])

void handle_opcode(int* reg, char* buffer, std::stack<size_t>& stack, size_t& i)
{
    switch (buffer[i])
    {
        case push:
        {
            size_t arg = buffer[i + 1];
            stack.push(arg);
            i += 2;
        }

        case pop:
        {
            size_t arg = buffer[i+1];
            reg[arg] = stack.top();
            stack.pop();
            i += 2;
        }

        case mov:
        {
            size_t arg_1 = buffer[i + 1];
            size_t arg_2 = buffer[i + 2];
            reg[arg_1] = reg[arg_2];
            i += 3;
        }

        case add:
        {
            size_t arg_1 = buffer[i + 1];
            size_t arg_2 = buffer[i + 2];
            reg[arg_1] += arg_2;
            i += 3;
        }

        case sub:
        {
            size_t arg_1 = buffer[i + 1];
            size_t arg_2 = buffer[i + 2];
            reg[arg_1] -= arg_2;
            i += 3;
        }

        case inc:
        {
            size_t arg = buffer[i + 1];
            reg[arg]++;
            i += 2;
        }

        case dec:
        {
            size_t arg = buffer[i + 1];
            reg[arg]--;
            i += 2;
        }

        case call:
        {
            size_t idx = i + 1;
        }

        case cmp:
        {
            size_t arg_1 = buffer[i + 1];
            size_t arg_2 = buffer[i + 2];
            if (arg_1 == arg_2)
                handle_opcode(reg, buffer, stack, *(&i+3));
            i += 3;
        }

        case je:
        {

        }
    }
}

int main(int argc, char* argv[])
{
	int reg[count] {0};
	std::stack<size_t> stack;

	char buffer[] =
	{
		0x13, 0x37,
		mov, r0, r1,
		add, r1, 0x10,
        cmp, r1, 0x10,
        je, 0x9, 'h', 'e', 'y', ' ', 'g', 'u', 'y', 's', '\0'
	};

	if (buffer[0] != 0x13 && buffer[1] != 0x37)
		return 1;

	for (size_t i = 2; i < ARR_SIZE(buffer); i++)
        handle_opcode(reg, buffer, stack, i);

	return 0;
}

/*
 main:
 push l.str.0
 call scanf r0

 cmp r0 0
 je l1

 cmp r0 1
 je l2

 cmp r0 2
 je l3

 l1:
 print "a"

 l2:
 print "b"

 l3:
 print "c"

 l.str.0
 "%d"
 */
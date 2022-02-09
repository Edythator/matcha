#include "../include/main.h"
#include <stdio.h>
#include <stack>

#define ARR_SIZE(x) sizeof(x)/sizeof(x[0])

int main(int argc, char* argv[])
{
	int reg[count] {0};

	std::stack<int> stack;
	int buffer[] =
	{
		0x13, 0x37,
		mov, r0, r1,
		add, r1, 0x100
	};

	if (buffer[0] != 0x13 && buffer[1] != 0x37)
		return 1;

	for (size_t i = 2; i < ARR_SIZE(buffer); i++)
	{
		switch (buffer[i])
		{
		case push:
		{
			int arg = buffer[i + 1];
			stack.push(arg);
			i += 2;
		}
		
		case pop:
		{
			int arg = buffer[i+1];
			reg[arg] = stack.top();
			stack.pop();
			i += 2;
		}

		case mov:
		{
			int arg_1 = buffer[i + 1];
			int arg_2 = buffer[i + 2];
			reg[arg_1] = reg[arg_2];
			i += 3;
		}

		case add:
		{
			int arg_1 = buffer[i + 1];
			int arg_2 = buffer[i + 2];
			reg[arg_1] += arg_2;
			i += 3;
		}

		case sub:
		{
			int arg_1 = buffer[i + 1];
			int arg_2 = buffer[i + 2];
			reg[arg_1] -= arg_2;
			i += 3;
		}

		case inc:
		{
			int arg = buffer[i + 1];
			reg[arg]++;
			i += 2;
		}

		case dec:
		{
			int arg = buffer[i + 1];
			reg[arg]--;
			i += 2;
		}

		}
	}
	return 0;
}

/*
// mov r0, [r1] move the value of r1 to r0
// mov r1, [r0] move the value of r0 back to r1
// mov r0, 0 move 0 to r0
// add r0, 1337 add 1337 to r0

.main
inc r7
mov r0, [r7]
cmp r7, 0x10
jl outcome1

.outcome1


.outcome2


.data
hello HELLO
constant 239
*/
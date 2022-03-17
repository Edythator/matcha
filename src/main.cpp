#include "../include/main.h"
#include <stdio.h>
#include <string.h>
#include <string>
#include <unordered_map>
#include <vector>

#define ARR_SIZE(x) sizeof(x)/sizeof(x[0])

std::unordered_map<std::string, char*> labels; // for the love of god don't make this a global also please don't use std::string

void handle_opcode(size_t* reg, const char* buffer, std::stack<size_t>& stack, size_t& i)
{
    switch (buffer[i])
    {
        case push:
        {
            char arg = buffer[i + 1];
            stack.push(arg);
            i += 2;
        }

        case pop:
        {
            char arg = buffer[i+1];
            reg[arg] = stack.top();
            stack.pop();
            i += 2;
        }

        case mov:
        {
            char arg_1 = buffer[i + 1];
            char arg_2 = buffer[i + 2];
            reg[arg_1] = reg[arg_2];
            i += 3;
        }

        case add:
        {
            char arg_1 = buffer[i + 1];
            char arg_2 = buffer[i + 2];
            reg[arg_1] += arg_2;
            i += 3;
        }

        case sub:
        {
            char arg_1 = buffer[i + 1];
            char arg_2 = buffer[i + 2];
            reg[arg_1] -= arg_2;
            i += 3;
        }

        case inc:
        {
            char arg = buffer[i + 1];
            reg[arg]++;
            i += 2;
        }

        case dec:
        {
            char arg = buffer[i + 1];
            reg[arg]--;
            i += 2;
        }

        case call:
        {
            size_t idx = i + 1;
        }

        case je:
        {
            const char* label_name = buffer + i + 1;
            char& label = *labels[label_name];

            for (size_t j = 0; j < 30; j++)
              handle_opcode(reg, &label, stack, j); //TODO: fix recursion crash
        }

        case cmp:
        {
            // wtf is this?
            //TODO: fix so it's proper
            /*char arg_1 = buffer[i + 1];
            char arg_2 = buffer[i + 2];
            if (arg_1 == arg_2)
                handle_opcode(reg, buffer, stack, *(&i+3));
            i += 3;*/
        }
    }
}

int main(int argc, char* argv[])
{
	size_t reg[count] {0};
	std::stack<size_t> stack;
    std::vector<const char*> strings;

	char buffer[] =
	{
		0x13, 0x37,
		mov, r0, r1,
		add, r1, 0x10,
        je,'l','1', '\0',

        ':', 'l', '1', '\0',
        add, r1, 0x10,
        add, r2, 0x20,
        add, r3, 0x30,
        ret
	};
    constexpr size_t buffer_size = ARR_SIZE(buffer);

	if (buffer[0] != 0x13 && buffer[1] != 0x37)
		return 1;

    // parse labels
    for (size_t i = 2; i < buffer_size; i++)
    {
        if (buffer[i] != ':')
            continue;

        if (buffer[i+1] == 's' && buffer[i+2] == 't' && buffer[i+3] == 'r')
        {
            i += 5; // s + t + r + n + \0

            const char* string = buffer + i;
            char* shit = (char*)malloc(strlen(string));
            strcpy(shit, string);

            strings.push_back(shit);
            continue;
        }

        const char* label_name = buffer + i + 1;
        i += strlen(label_name) + 1;

        size_t label_size = 0;
        for (size_t j = i; j < buffer_size; j++)
        {
            if (buffer[j] == ret)
            {
                label_size = j;
                break;
            }
        }

        char* label_content = (char*)malloc(label_size + 1);
        memcpy(label_content, buffer + i + 1, label_size);
        labels[label_name] = label_content;

        i += label_size;
    }

    // handle code
	for (size_t i = 2; i < buffer_size; i++)
        handle_opcode(reg, buffer, stack, i);

	return 0;
}

/*
 :main
 push str.0
 call scanf r0

 cmp r0 0
 je l1

 cmp r0 1
 je l2

 cmp r0 2
 je l3

 :l1
 push str.1
 call printf r0
 ret

 :l2
 push str.2
 pop r0
 call printf r0
 ret

 :l3
 push l.str.3
 pop r0
 call printf r0
 ret

 :str.0
 "%d"

 :str.1
 "a"

 :str.2
 "b"

 :str.3
 "c"
 */
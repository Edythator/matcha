#include "../include/main.h"
#include <stdio.h>
#include <string.h>
#include <string>
#include <unordered_map>
#include <vector>

#define ARR_SIZE(x) sizeof(x)/sizeof(x[0])

std::unordered_map<std::string, size_t> labels; // for the love of god don't make this a global also please don't use std::string

void handle_opcode(Register* reg, const char* buffer, std::stack<size_t>& stack)
{
    switch (buffer[reg->ri])
    {
        case push:
        {
            char arg = buffer[reg->ri + 1];
            stack.push(arg);
            reg->ri += 2;
            break;
        }

        case pop:
        {
            char arg = buffer[reg->ri + 1];
            reg->reg[arg] = stack.top();
            stack.pop();
            reg->ri += 2;
            break;
        }

        case mov:
        {
            char arg_1 = buffer[reg->ri + 1];
            char arg_2 = buffer[reg->ri + 2];
            reg->reg[arg_1] = reg->reg[arg_2];
            reg->ri += 3;
            break;
        }

        case add:
        {
            char arg_1 = buffer[reg->ri + 1];
            char arg_2 = buffer[reg->ri + 2];
            reg->reg[arg_1] += arg_2;
            reg->ri += 3;
            break;
        }

        case sub:
        {
            char arg_1 = buffer[reg->ri + 1];
            char arg_2 = buffer[reg->ri + 2];
            reg->reg[arg_1] -= arg_2;
            reg->ri += 3;
            break;
        }

        case inc:
        {
            char arg = buffer[reg->ri + 1];
            reg->reg[arg]++;
            reg->ri += 2;
            break;
        }

        case dec:
        {
            char arg = buffer[reg->ri + 1];
            reg->reg[arg]--;
            reg->ri += 2;
            break;
        }

        case call:
        {
            size_t idx = reg->ri + 1;
        }

        case ret:
        {
            reg->ri = reg->rb + 4; // for label name
            reg->rb = 0;
            break;
        }

        case je:
        {
            const char* label_name = buffer + reg->ri + 1;
            size_t label_pos = labels.at(label_name);

            reg->rb = reg->ri;
            reg->ri = label_pos + 1;
            break;
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

        case endp:
        {
            reg->ri = -1;
        }
    }
}

int main(int argc, char* argv[])
{
    Register registers{};
    std::stack<size_t> stack;
    std::vector<const char*> strings;

	char buffer[] =
	{
            0x13, 0x37,
            add, R0, 0x40,
            mov, R1, R0,add, R1, 0x10,
            je,'l','1', '\0',
            endp,

            ':', 'l', '1', '\0',
            add, R2, 0x10,
            add, R3, 0x20,
            add, R4, 0x30,
            ret
	};
    size_t buffer_size = ARR_SIZE(buffer);

    if (buffer[0] != 0x13 && buffer[1] != 0x37)
        return 1;
    registers.ri = 2;

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

        labels.emplace(label_name, i);

        for (size_t j = i; j < buffer_size; j++)
        {
            if (buffer[j] == ret)
            {
                i += j;
                break;
            }
        }
    }

    // handle code
    size_t& buffer_offset = registers.ri;
    while (buffer_offset < buffer_size || buffer_offset != -1)
        handle_opcode(&registers, buffer, stack);

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
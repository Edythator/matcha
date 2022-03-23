#include "../include/main.h"
#include <stdio.h>
#include <string.h>

#define ARR_SIZE(x) sizeof(x)/sizeof(x[0])

void handle_opcode(Register* reg, const char* buffer, std::stack<size_t>& stack, const std::unordered_map<std::string_view, size_t>& labels, const std::vector<const char*>& strings)
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
            // call 0x1 printf r0
            char string_pos = buffer[reg->ri + 1];
            const char* function = buffer + reg->ri + 2;
            reg->ri += strlen(function) + 1;
            char in = buffer[reg->ri];
            reg->ri++;

            // TODO: come up with a way to reference registers
            if (strcmp(function, "printf") != 0)
                printf(strings.at(string_pos));
            else if (strcmp(function, "scanf") != 0)
                scanf(strings.at(string_pos), in);

            break;
        }

        case ret:
        {
            // TODO: fix proper returning from jumps
            reg->ri = reg->rb + 4; // for label name
            reg->rb = 0;
            break;
        }

        case jmp:
        {
            const char* label_name = buffer + reg->ri + 1;
            size_t label_pos = labels.at(label_name);

            reg->rb = reg->ri;
            reg->ri = label_pos + 1;
            break;
        }

        case jnz:
        {
            size_t result = stack.top();
            stack.pop();
            if (!result)
                break;

            const char* label_name = buffer + reg->ri + 1;
            size_t label_pos = labels.at(label_name);

            reg->rb = reg->ri;
            reg->ri = label_pos + 1;
            break;
        }

        case cmp:
        {
            char arg_1 = buffer[reg->ri + 1];
            char arg_2 = buffer[reg->ri + 2];
            if (reg->reg[arg_1] == arg_2)
                stack.push(true);
            else stack.push(false);
            reg->ri += 3;
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
    std::unordered_map<std::string_view, size_t> labels;
    std::vector<const char*> strings;

    char buffer[] =
    {
            0x13, 0x37,
            add, R0, 0x40,
            mov, R1, R0,add, R1, 0x10,
            jmp,'l','1', '\0',
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
        handle_opcode(&registers, buffer, stack, labels, strings);

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
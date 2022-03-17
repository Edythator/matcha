#include "../include/main.h"
#include <stdio.h>
#include <string.h>
#include <unordered_map>
#include <vector>

#define ARR_SIZE(x) sizeof(x)/sizeof(x[0])

std::unordered_map<const char*, size_t*> labels; // for the love of god don't make this a global

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
            size_t size = 0;
            for (size_t j = i; j < size; j++)
            {
                if (buffer[j] == '\0')
                {
                    size = j - i;
                    break;
                }
            }
            const char* label_name = buffer + i;

            for (size_t j = 0; j < size; i++)
                handle_opcode(reg, (char*)labels[label_name], stack, j);
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
    std::vector<size_t*> strings;

	char buffer[] =
	{
		0x13, 0x37,
		mov, r0, r1,
		add, r1, 0x10,
        je,'l','1', '\0',

        ':', 'l', '1', '\0',
        add, r1, 0x10,
        ret
	};

	if (buffer[0] != 0x13 && buffer[1] != 0x37)
		return 1;

    // parse labels
    for (size_t i = 2; i < ARR_SIZE(buffer); i++)
    {
        if (buffer[i] != ':')
            continue;

        if (buffer[i+1] == 's' && buffer[i+2] == 't' && buffer[i+3] == 'r')
        {
            size_t size = i;
            i += 5; // s + t + r + \n + \0
            for (; i < ARR_SIZE(buffer); i++)
            {
                if (buffer[i] == '\0')
                    break;
            }

            size_t* variable = (size_t*)malloc(i - size + 1);
            memcpy(variable, buffer + size, i - size);
            strings.push_back(variable);
            continue;
        }

        const char* label_name = buffer + i + 1;
        size_t label_size = 0;

        for (size_t j = i; j < ARR_SIZE(buffer); j++)
        {
            if (buffer[j] == ret)
            {
                label_size = j;
                break;
            }
        }

        size_t* label_content = (size_t*)malloc(label_size + 1);
        memcpy(label_content, buffer + i + strlen(label_name), label_size);
        labels[label_name] = label_content;

        printf("0x%x ", (char*)labels[label_name]);
        // 0x4 0x0 0x10 0x11

        i += label_size;
    }

    // handle code
	for (size_t i = 2; i < ARR_SIZE(buffer); i++)
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
 push r0
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
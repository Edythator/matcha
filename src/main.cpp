#include "../include/main.h"
#include <stdio.h>
#include <string.h>
#include <unordered_map>
#include <string_view>
#include <vector>

std::unordered_map<std::string_view, size_t> labels;
std::vector<const char*> strings;

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
            bool skip = false;
            const char* function = buffer + reg->ri + 1;
            size_t function_len = strlen(function) + 1;
            char string_pos = buffer[reg->ri + 1 + function_len];

            reg->ri += function_len + 2; // call + func + string_pos
            char in = buffer[reg->ri];

            if (strcmp(function, "print") == 0)
                printf("%s", strings.at(string_pos));
            else if (strcmp(function, "printf") == 0)
            {
                skip = true;
                printf(strings.at(string_pos), reg->reg[in]);
            }
            else if (strcmp(function, "scanf") == 0)
            {
                skip = true;
                scanf(strings.at(string_pos), &reg->reg[in]);
            }

            if (skip)
                reg->ri++;
            break;
        }

        case ret:
        {
            reg->ri = reg->rb;
            reg->rb = 0;
            break;
        }

        case jmp:
        {
            const char* label_name = buffer + reg->ri + 1;
            size_t label_pos = labels.at(label_name);

            reg->rb = reg->ri + strlen(label_name) + 2;
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

            reg->rb = reg->ri + strlen(label_name) + 2;
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

    FILE* file = fopen("main.cha", "rb");
    if (!file)
        return -1;
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(file_size + 1);
    if (!buffer)
        return -1;

    fread(buffer, file_size, 1, file);
    fclose(file);
    buffer[file_size] = 0;

    if (buffer[0] != 0x13 && buffer[1] != 0x37)
        return 1;
    registers.ri = 2;

    // parse labels
    for (size_t i = 2; i < file_size; i++)
    {
        if (buffer[i] != ':')
            continue;

        if (buffer[i+1] == 's' && buffer[i+2] == 't' && buffer[i+3] == 'r')
        {
            i += 6; // s + t + r + n + \0

            const char* string = buffer + i;
            size_t string_len = strlen(string);
            char* shit = (char*)malloc(string_len);
            strcpy(shit, string);

            strings.emplace_back(shit);
            i += string_len;
            continue;
        }

        const char* label_name = buffer + i + 1;
        i += strlen(label_name) + 1;

        labels.emplace(label_name, i);

        for (size_t j = i; j < file_size; j++)
        {
            if (buffer[j] == ret)
            {
                i = j;
                break;
            }
        }
    }

    // handle code
    size_t& buffer_offset = registers.ri;
    while (buffer_offset < file_size || buffer_offset != -1)
        handle_opcode(&registers, buffer, stack);

    return 0;
}
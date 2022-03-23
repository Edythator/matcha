#pragma once
#include <stack>
#include <unordered_map>
#include <string_view>
#include <vector>

enum Opcode
{
    mov = 1,
    push,
    pop,
    add,
    sub,
    inc,
    dec,
    call,
    cmp,
    jmp,
    jnz,
    ret,
    endp
};

enum
{
    R0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    RC,
    RI,
    RB,
    REGISTER_COUNT
};

union Register
{
    struct
    {
        size_t r0;
        size_t r1;
        size_t r2;
        size_t r3;
        size_t r4;
        size_t r5;
        size_t r6;
        size_t r7;
        size_t rb;
        size_t rc;
        size_t ri;
    };
    size_t reg[REGISTER_COUNT];
};

void handle_opcode(Register* reg, const char* buffer, std::stack<size_t>& stack, const std::unordered_map<std::string_view, size_t>& labels, const std::vector<const char*>& strings);

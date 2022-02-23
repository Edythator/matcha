#pragma once
#include <stack>

enum Opcode
{
	mov = 1,
	push,
	pop,
	add,
	sub,
	inc,
	dec,
	exor,
    call,
    cmp,
    je
};

enum Register
{
	r0,
	r1,
	r2,
	r3,
	r4,
	r5,
	r6,
	r7,
	r8, // counter
	count
};

void handle_opcode(int* reg, int* buffer, std::stack<int>& stack, size_t& i);
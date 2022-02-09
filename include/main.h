#pragma once

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
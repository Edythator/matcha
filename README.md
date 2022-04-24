# matcha
This is my first attempt at making a virtual machine. It's very basic and done as my diploma project for Swedish high school.

The actual architecture of the VM is inspired by the instructions that x86 implements. Following is a list that contains all of the opcodes that are implemented as well as an explaination as to what they do.
```
push - pushes the value of a register on the stack
pop - pops the value on top of the stack into a register
mov - moves a value from one register to the other
add - adds a specific value to a register
sub - subtracts a specific value from a register
inc - increments the value of a register by one
dec - decrements the value of a register by one
call - used to call functions from the C stl, note that only scanf and printf are supported
ret - used in labels to mark the end of them
jmp - unconditional jump to a label
jnz - jumps to a label if the top of the stack is true (1)
cmp - compares the value of a register to another value
endp - marks the end of a program
```

Usage:
```./matcha file.cha```


.section .text.start
.global _start

.extern main

#magic
.word 0xE85250D6
#abi
.byte 1
#isa
.byte 0
#abi major version
.byte 0
#abi minor version
.byte 0
#initial sp
.word _stack_top


_start: 
    jal x0, main

loop:
    jal x0, loop




// Mark the 'main' label as visible to linker
// meaning other program can call this function
.global main

// the main function
// assign register a0 with val 1
// assign register a1 with val 2
// add a0 and a1 and assign value to register a2
// assign register a0 with val 0 
//   (set main return val to 0 so 'make' won't complain)
// jump to address ra + 0, and save PC + 4 to register x0
// return to address in register 'ra'
main:
    li a0, 0xECE00362
    li a1, 0x09012023
    li a2, 0xDEADBEEF

question_1:
    add a3, a0, a1

question_2:
    and a4, a0, a2
    xor a4, a4, a1

question_3:
    not a5, a0
    or a5, a5, a1
    and a5, a5, a2
    add a5, a5, a0

     ret
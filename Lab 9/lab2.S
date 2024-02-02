// Lab 2 template file
.data
.global lowercase_string
lowercase_string:
    // Specify a string ending with '\0'
    .asciz "ece 362 is awesome!"

.global login_name
login_name:
    .asciz "wu1795"

.global puid
puid:
    .asciz "0033553770"

// Align to 4bytes boundary as we have a string above
.balign 4

.text
.global asm_strlen
asm_strlen:
    /* uint32_t asm_strlen(char *str)
     * Take in a pointer to a string and return
     * its length up to the null terminating
     * character `\0`
    **/   

    /* Enter your code after this comment */
    //a0 = x10
    //a1 = x11
    li x11, 0                       //Length variable
    li x12, 0                       //Character index in array
    beq x10, x0, Q1_exit            //Check if pointer is null
    Q1_loop:                        //Loop until string reaches end
        lb x12, 0(x10)
        beq x12, x0, Q1_exit    //If string reaches null terminator, go to end
        addi x10, x10, 1
        addi x11, x11, 1
        beq x12, x12, Q1_loop
    Q1_exit:                        //Load 0 value and return the length
        li x10, 0
        or x10, x10, x11    
    /* Enter your code above this comment */
    ret

.global asm_fib
asm_fib:
    /* void asm_fib(int *arr, uint32_t length)
     * Accept a pointer to an array of integer and
     * the length of the array, then generate fibonacci
     * series starting at `0` and store the result
     * back to the array `arr` at corresponding indices.
     **/

    /* Enter your code after this comment */
    li x12, 0                   //Access integer form of i
    li x13, 4                   //Constant to become integer array
    li x14, 0                   //Previous = 0
    li x15, 1                   //Current = 1
    li x16, 1                   //1 for comparison
    li x17, 0                   //Loop variable
    li x18, 0                   //Temp variable
    li x19, 0                   //Access location of array
    beq x10, x0, Q2_exit        //Go to end if array is null or length is 0 
    beq x11, x0, Q2_exit
    Q2_loop:                    //Loop until loop variable equals length
        mul x12, x17, x13
        add x19, x10, x12
        bge x17, x11, Q2_exit   //When loop variable is equal to the length, go to end
        beq x17, x0, case0      //If loop variable is 0, go to case 0
        beq x17, x16, case1     //If loop variable is 1, go to case 1
        mv x18, x15
        add x15, x15, x14
        mv x14, x18
        sw x15, 0(x19)
        addi x17, x17, 1
        beq x14, x14, Q2_loop
    case0:                      //Make current index equal to previous value
        sw x14, 0(x19)
        addi x17, x17, 1
        beq x14, x14, Q2_loop
    case1:                      //Make current index equal to current value
        sw x15, 0(x19)
        addi x17, x17, 1
        beq x15, x15, Q2_loop
    Q2_exit:
    /* Enter your code above this comment */
    ret

.global asm_toUppercase
asm_toUppercase:
    /* void asm_toUppercase(char *str)
     * loop through an entire string and only convert
     * the lowercase letter to uppercase, leaving the
     * rest of the string intact
         **/

    /* Enter your code after this comment */
    li x11, 0                       //Current character in array
    li x12, 0                       //Loop variable
    li x13, 97                      //'a'
    li x14, 123                     //'{'
    li x15, 0                       //Access array locations
    beq x10, x0, Q3_exit
    Q3_loop:                        //Loop until string reaches end
        add x15, x10, x12
        lb x11, 0(x15)
        beq x11, x0, Q3_exit        //When loop reaches end of string, go to end
        bge x11, x13, testlowercase //If character is greater than 'a', go to testlowercase
        addi x12, x12, 1
        beq x12, x12, Q3_loop
    testlowercase:                  //If character is an uppercase letter, go to uppercase, otherwise, store, and reenter loop
        blt x11, x14, uppercase
        addi x12, x12, 1
        beq x12, x12, Q3_loop
    uppercase:                      //Change letter to lowercase and reenter loop
        addi x11, x11, -32
        sb x11, 0(x15)
        addi x12, x12, 1
        beq x12, x12, Q3_loop
    Q3_exit:
    /* Enter your code above this comment */
    ret
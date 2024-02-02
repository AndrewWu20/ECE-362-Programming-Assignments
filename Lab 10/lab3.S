// Lab 3 template file
.data

.global login_name
login_name:
    .asciz "wu1795"

.global puid
puid:
    .asciz "0033553770"

.balign 4

.equ TEST_SORT,         0x1
.equ TEST_STRCONCAT,    0x2
.equ TEST_FIB,          0x4
.equ TEST_BSEARCH,      0x8
.equ TEST_ALL,          (TEST_SORT | TEST_STRCONCAT | TEST_FIB | TEST_BSEARCH)

// Replace the TEST_ALL with the tests you want to run
// OR expression also support like the TEST_ALL above
// i.e. replace the TEST_ALL below with TEST_SORT | TEST_STRCONCAT
//      will test on the sort and strconcat
.global test
test:
    .word TEST_ALL


// Align to 4bytes boundary as we have a string above
.balign 4

.text
.global asm_sort_int
asm_sort_int:
    /* void asm_sort_int(int32_t* arr, uint32_t n)
     * relies on `qsort` in C standard library to 
     * sort in ascending order
     **/

    /* Enter your code after this comment */
    addi x2, x2, -8                 //Make space on stack
    sw x1, 4(x2)
    li x12, 4                       //Set third argument to 4 for size
    la x13, asm_cmp
    jal x1, qsort
    lw x1, 4(x2)                    //Return space to stack
    addi x2,x2, 8
    /* Enter your code above this comment */
    ret

.global asm_cmp
asm_cmp:
    // int asm_cmp(const void * a, const void * b)
    // Compare function to int32_t
    // used for qsort
    /* Enter your code after this comment */
    addi x2, x2, -8                 //Make space on stack
    sw x1, 4(x2)
    lw x5, 0(x10)                   //*a
    lw x6, 0(x11)                   //*b
    sub x5, x5, x6                  //Find temp
    blt x5, x0, Lessthan            //If temp less than 0, go to Lessthan
    li x6, 1                        //Return 1 if greater than 0, continue to return
    Return:
        mv x10, x6
        lw x1, 4(x2)                //Return space to stack
        addi x2, x2, 8
        ret
    Lessthan:
        li x6, -1                   //Return -1 and go to Return
        beq x0, x0, Return
    /* Enter your code above this comment */
    ret

.global asm_strconcat
asm_strconcat:
    /* char* asm_strconcat(char * str1, char * str2)
     * First assign memory space with `malloc`, 
     * concatenating `str1` and `str2`, 
     * and return the resulted string.
     **/
    /* Enter your code after this comment */
    addi x2, x2, -28            //Make space on stack
    sw x1, 0(x2)                //Assign stack space to x1
    sw x10, 4(x2)               //Save *str1 & *str2 to stack
    sw x11, 8(x2)               
    mv x14, x10                 //Save *str1 as x14 and *str2 as x15
    mv x15, x11                 
    jal x1, strlen              //Strlen *str1
    mv x16, x10                 //Save n1 to x16 and to stack
    sw x16, 12(x2)              
    lw x10, 8(x2)               //Load *str2 from stack
    jal x1, strlen              //Strlen *str2
    sw x10, 16(x2)              //Save n2 to stack
    lw x11, 12(x2)              //Load n1 from stack
    add x10, x10, x11           //Size = n1 + n2
    addi x10, x10, 1            //Size + 1
    sw x10, 20(x2)              //Save size to stack
    jal x1, malloc              //Malloc(sizeof(char) * size)
    sw x10, 24(x2)              //Save *buf to stack
    lw x11, 4(x2)               //Load *str1 and n1 from stack
    lw x12, 12(x2)              
    jal x1, memcpy              //Memcpy(buf, str1, n1)
    lw x10, 24(x2)              //Load *buf and n1 from stack
    lw x11, 12(x2)              
    add x10, x10, x11           //Buf = buf + n1
    lw x11, 8(x2)               //Load *str2 and n2 from stack
    lw x12, 16(x2)              
    jal x1, memcpy              //Memcpy(buf + n1, str2, n2)
    lw x10, 24(x2)              //Load *buf and size from stack
    lw x11, 20(x2)              
    addi x11, x11, -1           //Size - 1
    add x12, x11, x10           //Buff[size-1]
    sb x0, 0(x12)               //Buff[size-1] = '\0'
    lw x1, 0(x2)                //Assign stack space to x1
    addi x2, x2, 28             //Delete stack space
    /* Enter your code above this comment */
    ret

.global asm_fib
asm_fib:
    /* uint32_t asm_fib(uint32_t n)
     * accept an index term `n` and return 
     * the Fn fibonacci term (F0 = 0, F1 = 1)
     **/

    /* Enter your code after this comment */
    addi x2, x2, -12            //Allocate 3 items onto stack
    sw x1, 8(x2)                //Save return address
    sw x10, 4(x2)               //Save n
    li x14, 2                   //Set x14 to 2 to check for base case
    blt x10, x14, Base_Case     //If base case, then skip to base case loop
    addi x10, x10, -1           //Calculate n = n - 1
    jal x1, asm_fib             //Fib(n-1)
    mv x7, x10                  //Store fib(n-1) into x7
    sw x7, 0(x2)                //Save fib(n - 1) into x7 in stack
    lw x10, 4(x2)               //Store original n to x10
    addi x10, x10, -2           //Calculate n = n - 2
    jal x1, asm_fib             //fib(n - 2)
    lw x7, 0(x2)                //Load fib(n - 1) from stack
    add x10, x7, x10            //Fib(n) = Fib(n - 1) + Fib (n - 2)
    Base_Case:  
        lw x1, 8(x2)            //Restore return address
        addi x2, x2, 12         //Deallocate 3 items from stack
    /* Enter your code above this comment */
    ret

.global asm_bsearch
asm_bsearch:
    /* int32_t asm_bsearch(int *arr, int key, 
                    int32_t start,
                    int32_t end)
     * Return the index of the element in the array
     * if the element does not exist, return -1 instead
     * arr: integer array to be searched
     * key: the element we want to search on
     * start: start index of subarray, inclusive
     * end: end index of subarray, inclusiveloop through an entire string and only convert
     **/
    //Future reference, sp = x2
    /* Enter your code after this comment */
    addi x2, x2, -24            //Make space on stack
    sw x1, 0(x2)                //Assign stack space to x1
    sw x10, 4(x2)               //Save *arr, key, start, end to stack
    sw x11, 8(x2)               
    sw x12, 12(x2)
    sw x13, 16(x2)
    bgt x12, x13, Base_case     //If start > end, go to Base_case    
    li x7, 2                    //Calculate mid
    add x5, x12, x13    
    div x5, x5, x7  
    sw x5, 20(x2)               //Save mid to stack
    li x6, 4
    mul x6, x6, x5              //Calculate arr[mid]
    add x10, x10, x6 
    lw x10, 0(x10)
    blt x10, x11, Mid_Higher    //If arr[mid] < key, go to Mid_Higher
    bgt x10, x11, Mid_Lower     //If arr[mid] > key, go to Mid_Lower
    lw x10, 20(x2)              //Load mid from stack
    jal x1, Exit
    Mid_Higher:                 //Run asm_bsearch(arr, key, mid + 1, end) and go to Exit
        lw x10, 4(x2)           //Load *arr, key, mid, end
        lw x11, 8(x2)
        lw x12, 20(x2)
        lw x13, 16(x2)
        addi x12, x12, 1        //Calculate mid + 1
        jal x1, asm_bsearch
        jal x1, Exit
    Mid_Lower:                  //Run asm_bsearch(arr, key, start, mid - 1)
        lw x10, 4(x2)           //Load *arr, key, start, mid
        lw x11, 8(x2)
        lw x12, 12(x2)
        lw x13, 20(x2)
        addi x13, x13, -1       //Calculate mid - 1
        jal x1, asm_bsearch
        jal x1, Exit
    Base_case:                  //Return -1 and go to Exit
        addi x10, x0, -1
    Exit:                       //Restore return address and deallocate from stack
        lw x1, 0(x2)
        addi x2, x2, 24
    /* Enter your code above this comment */
    ret

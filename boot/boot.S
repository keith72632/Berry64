//Arm Cortex-A72 has four cores. Only wanted the master core to run or otherwise hang
//Tell OS to access stack 
//Initialize BSS(area where unitialized varibales are store) to zero.
//Jump to main() in C
.section ".text.boot" // Make sure the linker puts this at the start of image

.global _start // Exectution starts here

_start:
    // Check processor ID is 0 (main core) or else hand
    mrs    x1, mpidr_el1
    and    x1, x1, #3
    cbz    x1, 2f
    //not in main core, so hang
1: wfe
    b    1b

2: // At the main core
    
    //set stack to start below code
    ldr x1, =_start
    mov sp, x1

    //clean BSS section
    ldr    x1, =__bss_start   //start address
    ldr    w2, =__bss_size    //size of section
3:  cbz    w2, 4f             //quit loop if zero
    str    xzr, [x1], #8
    sub    w2, w2, #1
    cbnz   w2, 3b            //loop if non-zero

    //Jump to main()
4:  bl    main

    //incase it does return, halt core
    b     1b

.globl _start
_start:
    mov sp,#0x8000
    bl notmain
hang: b hang

.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

.globl dummy
dummy:
    bx lr

    b core_main1 

.globl wakeup1                  
wakeup1:                        // wake up core 1 (same process for 2 and 3 so those are uncommented) 
    ldr r0, =init_core1         // load the address of init_core1 into r0
    mov r1, #0x40000000         // put the multicore mailbox base address into r1
    str r0,[r1,#0x9C]           // put the address of init_core1 (in r0 currently) into the base mailbox address + the mailbox offset of core 1
    bx lr                       // return

init_core1:                     // this is where execution on core 1 starts
    mov sp, #18000              // set up the stack, which C expects... arbitrary address in this case 
    b core_main1                // branch to the function we want this core to actually run


.globl wakeup2
wakeup2:                        // wake up core 2
    ldr r0, =init_core2
    mov r1, #0x40000000
    str r0,[r1,#0xAC]
    bx lr

init_core2:
    mov sp, #28000
    b core_main2


.globl wakeup3
wakeup3:                        // wake up core 3
    ldr r0, =init_core3
    mov r1, #0x40000000
    str r0,[r1,#0xBC]
    bx lr

init_core3:
    mov sp, #38000
    b core_main3

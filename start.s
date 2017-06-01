
;@-------------------------------------------------------------------------
;@-------------------------------------------------------------------------

.globl _start
_start:
    ldr pc,reset_handler
    ldr pc,undefined_handler
    ldr pc,swi_handler
    ldr pc,prefetch_handler
    ldr pc,data_handler
    ldr pc,hyp_handler
    ldr pc,irq_handler
    ldr pc,fiq_handler
reset_handler:      .word reset
undefined_handler:  .word hang
swi_handler:        .word hang
prefetch_handler:   .word hang
data_handler:       .word hang
hyp_handler:        .word hang
irq_handler:        .word irq
fiq_handler:        .word hang

reset:
    mov r0,#0x8000
    MCR p15, 4, r0, c12, c0, 0

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

.globl GETPC
GETPC:
    mov r0,lr
    bx lr

.globl GETCPSR
GETCPSR:
    mrs r0,cpsr
    bx lr

    b core_main1

.globl wakeup1
wakeup1:                        // wake up core 1 (same process for 2 and 3 so those are uncommented)
    ldr r0, =init_core1         // load the address of init_core1 into r0
    mov r1, #0x40000000         // put the multicore mailbox base address into r1
    str r0,[r1,#0x9C]           // put the address of init_core1 (in r0 currently) into the base mailbox address + the mailbox offset of core 1
    bx lr                       // return

init_core1:                     // this is where execution on core 1 starts
    mov sp, #0x18000              // set up the stack, which C expects... arbitrary address in this case
    b core_main1                // branch to the function we want this core to actually run

.globl wakeup2
wakeup2:                        // wake up core 2
    ldr r0, =init_core2
    mov r1, #0x40000000
    str r0,[r1,#0xAC]
    bx lr

init_core2:
    mov sp, #0x28000
    b core_main2

.globl wakeup3
wakeup3:                        // wake up core 3
    ldr r0, =init_core3
    mov r1, #0x40000000
    str r0,[r1,#0xBC]
    bx lr

init_core3:
    mov sp, #0x38000
    b core_main3

.globl enable_irq
enable_irq:
    mrs r0,cpsr
    bic r0,r0,#0x80
    msr cpsr_c,r0
    bx lr

irq:
    push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    bl c_irq_handler
    pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    ;@subs pc,lr,#4
    eret


;@-------------------------------------------------------------------------
;@-------------------------------------------------------------------------


;@-------------------------------------------------------------------------
;@
;@ Copyright (c) 2012 David Welch dwelch@dwelch.com
;@
;@ Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
;@
;@ The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
;@
;@ THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
;@
;@-------------------------------------------------------------------------

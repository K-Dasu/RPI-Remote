//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
#include "timer.h"
#include "interrupts.h"

#define PBASE 0x3F000000

extern void PUT32(unsigned int, unsigned int);
extern void PUT16(unsigned int, unsigned int);
extern void PUT8(unsigned int, unsigned int);
extern unsigned int GET32(unsigned int);
extern void dummy(unsigned int);

#define ARM_TIMER_LOD (PBASE + 0x0000B400)
#define ARM_TIMER_VAL (PBASE + 0x0000B404)
#define ARM_TIMER_CTL (PBASE + 0x0000B408)
#define ARM_TIMER_RIS (PBASE + 0x0000B410)
#define ARM_TIMER_MIS (PBASE + 0x0000B414)
#define ARM_TIMER_RLD (PBASE + 0x0000B418)
#define ARM_TIMER_CNT (PBASE + 0x0000B420)
#define ARM_TIMER_CLI (PBASE + 0x0000B40C)
#define ARM_TIMER_DIV (PBASE + 0x0000B41C)


#define GPFSEL1 (PBASE + 0x00200004)
#define GPSET0 (PBASE + 0x0020001C)
#define GPCLR0 (PBASE + 0x00200028)
#define GPPUD (PBASE + 0x00200094)
#define GPPUDCLK0 (PBASE + 0x00200098)
#define GPFSEL3 0x3F20000C
#define GPFSEL4 0x3F200010

#define AUX_ENABLES (PBASE + 0x00215004)
#define AUX_MU_IO_REG (PBASE + 0x00215040)
#define AUX_MU_IER_REG (PBASE + 0x00215044)
#define AUX_MU_IIR_REG (PBASE + 0x00215048)
#define AUX_MU_LCR_REG (PBASE + 0x0021504C)
#define AUX_MU_MCR_REG (PBASE + 0x00215050)
#define AUX_MU_LSR_REG (PBASE + 0x00215054)
#define AUX_MU_MSR_REG (PBASE + 0x00215058)
#define AUX_MU_SCRATCH (PBASE + 0x0021505C)
#define AUX_MU_CNTL_REG (PBASE + 0x00215060)
#define AUX_MU_STAT_REG (PBASE + 0x00215064)
#define AUX_MU_BAUD_REG (PBASE + 0x00215068)


#define IRQ_BASIC (PBASE + 0x0000B200)
#define IRQ_PEND1 (PBASE + 0x0000B204)
#define IRQ_PEND2 (PBASE + 0x0000B208)
#define IRQ_FIQ_CONTROL (PBASE + 0x0000B210)
#define IRQ_ENABLE_BASIC (PBASE + 0x0000B218)
#define IRQ_DISABLE_BASIC (PBASE + 0x0000B224)

volatile unsigned int icount;

// GPIO14  TXD0 and TXD1
// GPIO15  RXD0 and RXD1
//------------------------------------------------------------------------
unsigned int uart_lcr(void) { return (GET32(AUX_MU_LSR_REG)); }
//------------------------------------------------------------------------
unsigned int uart_recv(void) {
  while (1) {
    if (GET32(AUX_MU_LSR_REG) & 0x01)
      break;
  }
  return (GET32(AUX_MU_IO_REG) & 0xFF);
}
//------------------------------------------------------------------------
unsigned int uart_check(void) {
  if (GET32(AUX_MU_LSR_REG) & 0x01)
    return (1);
  return (0);
}
//------------------------------------------------------------------------
void uart_send(unsigned int c) {
  while (1) {
    if (GET32(AUX_MU_LSR_REG) & 0x20)
      break;
  }
  PUT32(AUX_MU_IO_REG, c);
}
//------------------------------------------------------------------------
void uart_flush(void) {
  while (1) {
    if ((GET32(AUX_MU_LSR_REG) & 0x100) == 0)
      break;
  }
}
//------------------------------------------------------------------------
void hexstrings(unsigned int d) {
  // unsigned int ra;
  unsigned int rb;
  unsigned int rc;

  rb = 32;
  while (1) {
    rb -= 4;
    rc = (d >> rb) & 0xF;
    if (rc > 9)
      rc += 0x37;
    else
      rc += 0x30;
    uart_send(rc);
    if (rb == 0)
      break;
  }
  uart_send(0x20);
}
//------------------------------------------------------------------------
void hexstring(unsigned int d) {
  hexstrings(d);
  uart_send(0x0D);
  uart_send(0x0A);
}
//------------------------------------------------------------------------
void uart_init(void) {
  unsigned int ra;

  PUT32(AUX_ENABLES, 1);
  PUT32(AUX_MU_IER_REG, 0);
  PUT32(AUX_MU_CNTL_REG, 0);
  PUT32(AUX_MU_LCR_REG, 3);
  PUT32(AUX_MU_MCR_REG, 0);
  PUT32(AUX_MU_IER_REG, 0);
  PUT32(AUX_MU_IIR_REG, 0xC6);
  PUT32(AUX_MU_BAUD_REG, 270);
  ra = GET32(GPFSEL1);
  ra &= ~(7 << 12); // gpio14
  ra |= 2 << 12;    // alt5
  ra &= ~(7 << 15); // gpio15
  ra |= 2 << 15;    // alt5
  PUT32(GPFSEL1, ra);
  PUT32(GPPUD, 0);
  for (ra = 0; ra < 150; ra++)
    dummy(ra);
  PUT32(GPPUDCLK0, (1 << 14) | (1 << 15));
  for (ra = 0; ra < 150; ra++)
    dummy(ra);
  PUT32(GPPUDCLK0, 0);
  PUT32(AUX_MU_CNTL_REG, 3);
}

// expects null-terminated string
void uart_print(const char *string) {
  unsigned int idx = 0;

  while (string[idx] != '\0') {
    uart_send(string[idx++]);
  }
}

void uart_println(const char *string) {
  uart_print(string);

  // carriage return
  uart_send(0x0D);
  uart_send(0x0A);
}

// print registers of interest
void uart_dump_registers() {
  unsigned int current_pc = GETPC();
  uart_print("pc: ");
  hexstring(current_pc);

  unsigned int current_cpsr = GETCPSR();
  uart_print("cpsr: ");
  hexstring(current_cpsr);

  register int sp asm("sp");
  uart_print("sp: ");
  hexstring(sp);

  register int r0 asm("r0");
  uart_print("r0: ");
  hexstring(r0);

  register int r1 asm("r1");
  uart_print("r1: ");
  hexstring(r1);

  register int r2 asm("r2");
  uart_print("r2: ");
  hexstring(r2);

  register int r3 asm("r3");
  uart_print("r3: ");
  hexstring(r3);

  register int r4 asm("r4");
  uart_print("r4: ");
  hexstring(r4);

  register int r5 asm("r5");
  uart_print("r5: ");
  hexstring(r5);

  register int r6 asm("r6");
  uart_print("r6: ");
  hexstring(r6);

  register int r7 asm("r7");
  uart_print("r7: ");
  hexstring(r7);

  register int r8 asm("r8");
  uart_print("r8: ");
  hexstring(r8);

  register int r9 asm("r9");
  uart_print("r9: ");
  hexstring(r9);

  register int r10 asm("r10");
  uart_print("r10: ");
  hexstring(r10);

  register int r11 asm("r11");
  uart_print("r11: ");
  hexstring(r11);

  register int r12 asm("r12");
  uart_print("r12: ");
  hexstring(r12);
}

unsigned int timer_tick(void) { return (GET32(ARM_TIMER_CNT)); }

//------------------------------------------------------------------------

void timer_init(void) {
 /* Enable the timer interrupt IRQ */
 RPI_GetIrqController()->Enable_Basic_IRQs = RPI_BASIC_ARM_TIMER_IRQ;

 /* Setup the system timer interrupt */
 /* Timer frequency = Clk/256 * 0x400 */
 RPI_GetArmTimer()->Load = 0x400;

 /* Setup the ARM Timer */
 RPI_GetArmTimer()->Control =
        RPI_ARMTIMER_CTRL_23BIT |
        RPI_ARMTIMER_CTRL_ENABLE |
        RPI_ARMTIMER_CTRL_INT_ENABLE |
        RPI_ARMTIMER_CTRL_PRESCALE_256;

 /* Enable interrupts! */
 enable_irq();

 /*Finished Initializing Interrupts*/
 uart_println("\n\nTimer initialized");
}
//-------------------------------------------------------------------------

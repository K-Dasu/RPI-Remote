#define PBASE 0x3F000000

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

extern unsigned int uart_lcr(void);
extern unsigned int uart_recv(void);
extern unsigned int uart_check(void);
extern void uart_send(unsigned int c);
extern void uart_flush(void);
extern void hexstrings(unsigned int d);
extern void hexstring(unsigned int d);
extern void uart_init(void);
extern void uart_print(const char *string);
extern void uart_println(const char *string);
extern void uart_dump_registers();
extern void timer_init();

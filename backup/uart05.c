extern void PUT32(unsigned int, unsigned int);
extern void PUT16(unsigned int, unsigned int);
extern void PUT8(unsigned int, unsigned int);
extern unsigned int GET32(unsigned int);
extern unsigned int GETPC(void);
extern void dummy(unsigned int);
extern void enable_irq ( void );
extern void enable_fiq ( void );
extern unsigned int BRANCHTO(unsigned int);

extern unsigned int GETCPSR(void);

extern void uart_init(void);
extern unsigned int uart_lcr(void);
extern void uart_flush(void);
extern void uart_send(unsigned int);
extern unsigned int uart_recv(void);
extern unsigned int uart_check(void);
extern void hexstring(unsigned int);
extern void hexstrings(unsigned int);
extern void timer_init(void);
extern unsigned int timer_tick(void);

extern void timer_init(void);
extern unsigned int timer_tick(void);

//------------------------------------------------------------------------
int main(void) {
  int ra;
  uart_init();
  uart_println("\n\nProgram Start");
  uart_dump_registers();

  // timer_init();

  // initCores();

  while (1) {
    int c = uart_recv();

    uart_send(c);

    if(c == 0x0000000D) {
        uart_send('\n');
    }

    if(c == 0x0000007F) {
        uart_send(0x08);
        uart_send(0x7f);
        uart_send(' ');
        uart_send(0x08);
        uart_send(0x7f);
    }
  }

  return (0);
}

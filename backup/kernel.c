#include "multicore.h"
#include "periph.h"
#include <stddef.h>
#include <stdint.h>

// Memory-Mapped I/O output
static inline void mmio_write(uint32_t reg, uint32_t data) {
  *(volatile uint32_t *)reg = data;
}

// Memory-Mapped I/O input
static inline uint32_t mmio_read(uint32_t reg) {
  return *(volatile uint32_t *)reg;
}

// Loop <delay> times in a way that the compiler won't optimize away
static inline void delay(int32_t count) {
  asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
               : "=r"(count)
               : [count] "0"(count)
               : "cc");
}

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif

    int
    kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
  // Declare as unused
  (void)r0;
  (void)r1;
  (void)atags;

  // spinning up multiple cores
  /*
  // core 0 already awake (running this), just make it wake up the other cores
  wakeup1();
  wakeup2();
  wakeup3();

  // start actually running the code for this core
  core_main0();
  */

  uart_init();
  uart_println("\n\n");
  uart_println("\n\nProgram Start");
  uart_dump_registers();

  timer_init();

  initCores();

  while (1) {
    int c = uart_recv();

    uart_send(c);

    if (c == 0x0000000D) {
      uart_send('\n');
    }

    if (c == 0x0000007F) {
      uart_send(0x08);
      uart_send(0x7f);
      uart_send(' ');
      uart_send(0x08);
      uart_send(0x7f);
    }
  }

  return (0);
}

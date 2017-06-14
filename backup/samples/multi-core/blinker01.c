#include "gpio.h"

extern void PUT32(unsigned int, unsigned int);
extern unsigned int GET32(unsigned int);
extern void dummy(unsigned int);

extern void wakeup1(void);
extern void wakeup2(void);
extern void wakeup3(void);

typedef void (*fn)(void);

void core_main0(void) {
  unsigned int ra;

  ra = GET32(GPFSEL1);
  ra &= ~(0b111 << GPFSEL_REG6);
  ra |= FSEL_OUTP << GPFSEL_REG6;
  PUT32(GPFSEL1, ra);

  while (1) {
    PUT32(GPSET0, GPIO_PIN16);
    for (ra = 0; ra < 0x100000; ra++)
      dummy(ra);
    PUT32(GPCLR0, GPIO_PIN16);
    for (ra = 0; ra < 0x100000; ra++)
      dummy(ra);
  }
}

void core_main1(void) {
  unsigned int ra;

  // spin for a bit, so blinking doesn't start at the same time
  for (ra = 0; ra < 500000; ra++)
    dummy(ra);

  ra = GET32(GPFSEL1);
  ra &= ~(0b111 << GPFSEL_REG7);
  ra |= FSEL_OUTP << GPFSEL_REG7;
  PUT32(GPFSEL1, ra);

  while (1) {
    PUT32(GPSET0, GPIO_PIN17);
    for (ra = 0; ra < 0x100000; ra++)
      dummy(ra);
    PUT32(GPCLR0, GPIO_PIN17);
    for (ra = 0; ra < 0x100000; ra++)
      dummy(ra);
  }
}

void core_main2(void) {
  unsigned int ra;

  // spin for a bit, so blinking doesn't start at the same time
  for (ra = 0; ra < 1000000; ra++)
    dummy(ra);

  ra = GET32(GPFSEL1);
  ra &= ~(0b111 << GPFSEL_REG8);
  ra |= FSEL_OUTP << GPFSEL_REG8;
  PUT32(GPFSEL1, ra);

  while (1) {
    PUT32(GPSET0, GPIO_PIN18);
    for (ra = 0; ra < 0x100000; ra++)
      dummy(ra);
    PUT32(GPCLR0, GPIO_PIN18);
    for (ra = 0; ra < 0x100000; ra++)
      dummy(ra);
  }
}

void core_main3(void) {
  unsigned int ra;

  // spin for a bit, so blinking doesn't start at the same time
  for (ra = 0; ra < 1500000; ra++)
    dummy(ra);

  ra = GET32(GPFSEL1);
  ra &= ~(0b111 << GPFSEL_REG9);
  ra |= FSEL_OUTP << GPFSEL_REG9;
  PUT32(GPFSEL1, ra);

  while (1) {
    PUT32(GPSET0, GPIO_PIN19);
    for (ra = 0; ra < 0x100000; ra++)
      dummy(ra);
    PUT32(GPCLR0, GPIO_PIN19);
    for (ra = 0; ra < 0x100000; ra++)
      dummy(ra);
  }
}

int notmain(void) {
  // core 0 already awake (running this), just make it wake up the other cores
  wakeup1();
  wakeup2();
  wakeup3();

  // start actually running the code for this core
  core_main0();

  return (0);
}

#define GPIO_BASE 0x3F000000

#define FSEL_INPT 0b000
#define FSEL_OUTP 0b001
#define FSEL_ALT0 0b100
#define FSEL_ALT1 0b101
#define FSEL_ALT2 0b110
#define FSEL_ALT3 0b111
#define FSEL_ALT4 0b011
#define FSEL_ALT5 0b010

#define GPFSEL0 (GPIO_BASE | 0x200000)
#define GPFSEL1 (GPIO_BASE | 0x200004)
#define GPFSEL2 (GPIO_BASE | 0x200008)
#define GPFSEL3 (GPIO_BASE | 0x20000C)
#define GPFSEL4 (GPIO_BASE | 0x200010)
#define GPFSEL5 (GPIO_BASE | 0x200014)

#define GPFSEL_REG0 0
#define GPFSEL_REG1 3
#define GPFSEL_REG2 6
#define GPFSEL_REG3 9
#define GPFSEL_REG4 12
#define GPFSEL_REG5 15
#define GPFSEL_REG6 18
#define GPFSEL_REG7 21
#define GPFSEL_REG8 24
#define GPFSEL_REG9 27

#define GPSET0 (GPIO_BASE | 0x20001C)
#define GPSET1 (GPIO_BASE | 0x200020)

#define GPCLR0 (GPIO_BASE | 0x200028)
#define GPCLR1 (GPIO_BASE | 0x20002C)

#define GPIO_PIN0 (1 << 0)
#define GPIO_PIN1 (1 << 1)
#define GPIO_PIN2 (1 << 2)
#define GPIO_PIN3 (1 << 3)
#define GPIO_PIN4 (1 << 4)
#define GPIO_PIN5 (1 << 5)
#define GPIO_PIN6 (1 << 6)
#define GPIO_PIN7 (1 << 7)
#define GPIO_PIN8 (1 << 8)
#define GPIO_PIN9 (1 << 9)
#define GPIO_PIN10 (1 << 10)
#define GPIO_PIN11 (1 << 11)
#define GPIO_PIN12 (1 << 12)
#define GPIO_PIN13 (1 << 13)
#define GPIO_PIN14 (1 << 14)
#define GPIO_PIN15 (1 << 15)
#define GPIO_PIN16 (1 << 16)
#define GPIO_PIN17 (1 << 17)
#define GPIO_PIN18 (1 << 18)
#define GPIO_PIN19 (1 << 19)
#define GPIO_PIN20 (1 << 20)
#define GPIO_PIN21 (1 << 21)
#define GPIO_PIN22 (1 << 22)
#define GPIO_PIN23 (1 << 23)
#define GPIO_PIN24 (1 << 24)
#define GPIO_PIN25 (1 << 25)
#define GPIO_PIN26 (1 << 26)
#define GPIO_PIN27 (1 << 27)
#define GPIO_PIN28 (1 << 28)
#define GPIO_PIN29 (1 << 29)
#define GPIO_PIN30 (1 << 30)
#define GPIO_PIN31 (1 << 31)

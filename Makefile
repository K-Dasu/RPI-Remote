#ARMGNU ?= arm-none-eabi
#ARMGNU ?= arm-linux-gnueabi
ARMGNU ?= ../gcc-arm-none-eabi/bin/arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding

gcc : kernel.bin

all : gcc clang

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc
	rm -f *.clang.s

start.o : start.s
	$(ARMGNU)-as start.s -o start.o

kernel.o : kernel.c
	$(ARMGNU)-gcc $(COPS) -c kernel.c -o kernel.o

periph.o : periph.c
	$(ARMGNU)-gcc $(COPS) -c periph.c -o periph.o

interrupts.o : interrupts.c
	$(ARMGNU)-gcc $(COPS) -c interrupts.c -o interrupts.o

timer.o : timer.c
	$(ARMGNU)-gcc $(COPS) -c timer.c -o timer.o

multicore.o: multicore.c
	$(ARMGNU)-gcc $(COPS) -c multicore.c -o multicore.o

kernel.bin : memmap start.o periph.o timer.o interrupts.o kernel.o multicore.o
	$(ARMGNU)-ld start.o periph.o timer.o interrupts.o kernel.o multicore.o -T memmap -o kernel.elf
	$(ARMGNU)-objdump -D kernel.elf > kernel.list
	$(ARMGNU)-objcopy kernel.elf -O ihex kernel.hex
	$(ARMGNU)-objcopy kernel.elf -O binary kernel.bin

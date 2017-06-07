#ARMGNU ?= arm-none-eabi
#ARMGNU ?= arm-linux-gnueabi
PREFIX ?= ../gcc-arm-none-eabi/bin/arm-none-eabi-
RASPPI	?= 3

CC	= $(PREFIX)gcc
CPP	= $(PREFIX)g++
AS	= $(CC)
LD	= $(PREFIX)ld
AR	= $(PREFIX)ar

ARCH	?= -march=armv8-a -mtune=cortex-a53 -mfpu=neon-fp-armv8 -mfloat-abi=hard
TARGET	?= kernel7

OBJS	= exceptionStub.o interrupts.o multicore.o periph.o timer.o sysinit.o uart05.o
OPTIMIZE ?= -O2
INCLUDE	+= -I includes

AFLAGS	+= $(ARCH) -DRASPPI=$(RASPPI) $(INCLUDE)
COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding

CPPFLAGS+= $(CFLAGS) -fno-exceptions -fno-rtti -std=c++14

%.o: %.S
	@echo Building $<
	$(AS) $(AFLAGS) -c -o $@ $<

%.o: %.c
	@echo Building $<
	$(CC) $(COPS) -c -o $@ $<

%.o: %.cpp
	@echo Building $<
	$(CPP) $(CPPFLAGS) -c -o $@ $<

$(TARGET).img: $(OBJS) $(LIBS) start.o rpi-remote.ld
	$(LD) -o $(TARGET).elf -Map $(TARGET).map -T rpi-remote.ld start.o $(OBJS) $(LIBS)
	$(PREFIX)objdump -d $(TARGET).elf | $(PREFIX)c++filt > $(TARGET).lst
	$(PREFIX)objcopy $(TARGET).elf -O binary $(TARGET).img
	wc -c $(TARGET).img

clean:
	rm -f *.o *.a *.elf *.lst *.img *.cir *.map *~

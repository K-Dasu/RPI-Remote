# Makefile
#

CIRCLEHOME = ./circle
NEWLIBHOME = ./newlib-cygwin
NEWLIB = $(NEWLIBHOME)/build/arm-none-eabi/newlib/
NEWLIB_HEADERS = $(NEWLIBHOME)/newlib/libc/include/

# the define that Rules.mk expects
OTHER_INCLUDES = -I $(NEWLIB_HEADERS)

OBJS    = main.o kernel.o
LIBS    = $(CIRCLEHOME)/lib/libcircle.a $(NEWLIB)/libc.a $(NEWLIB)/libm.a

include $(CIRCLEHOME)/Rules.mk

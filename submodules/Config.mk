# Config.mk for circle
PROJECT_PATH=$(RPI_REMOTE_PATH)
NEWLIB_DIR=$(PROJECT_PATH)/submodules/newlib-cygwin

RASPPI = 3

INCLUDE += -I $(NEWLIB_DIR)/newlib/libc/include/
LIBS += $(NEWLIB_DIR)/build/arm-none-eabi/newlib/libc.a $(NEWLIB_DIR)/build/arm-none-eabi/newlib/libm.a

TARGET=$(PROJECT_PATH)/kernel/

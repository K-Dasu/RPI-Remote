PROJECT_PATH=$(shell pwd)

CFLAGS_FOR_TARGET=-march=armv8-a -mtune=cortex-a53 -mfpu=neon-fp-armv8 -mfloat-abi=hard

CIRCLE_DIR=$(PROJECT_PATH)/submodules/circle
NEWLIB_DIR=$(PROJECT_PATH)/submodules/newlib-cygwin
NEWLIB_BUILD=$(NEWLIB_DIR)/build

RASPPI=3

build-newlib: 
	export CFLAGS_FOR_TARGET='$(CFLAGS_FOR_TARGET)'
	-mkdir $(NEWLIB_BUILD)
	cd $(NEWLIB_BUILD) && $(NEWLIB_DIR)/configure --target arm-none-eabi --disable-newlib-supplied-syscalls --disable-multilib && make -j

build-circle:
	cp $(PROJECT_PATH)/submodules/Config.mk $(CIRCLE_DIR)
	export RPI_REMOTE_PATH=$(PROJECT_PATH) && cd $(CIRCLE_DIR) && ./makeall
	rm $(CIRCLE_DIR)/Config.mk

clear-libs:
	rm -rf $(NEWLIB_BUILD)
	cd $(CIRCLE_DIR) && ./makeall clean

build-libs: build-newlib build-circle

clean-build-libs: clear-libs build-libs

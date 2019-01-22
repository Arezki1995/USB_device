#
#             LUFA Library
#     Copyright (C) Dean Camera, 2017.
#
#  dean [at] fourwalledcubicle [dot] com
#           www.lufa-lib.org
#
# --------------------------------------
#         LUFA Project Makefile.
# --------------------------------------

# Run "make help" for target help.

MCU          = atmega16u2
ARCH         = AVR8
BOARD        = UNO
F_CPU        = 16000000
F_USB        = $(F_CPU)
OPTIMIZATION = s
TARGET       = PAD
SRC          = $(TARGET).c Descriptors.c $(LUFA_SRC_USB) $(LUFA_SRC_USBCLASS)
LUFA_PATH    = ../../LUFA
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -IConfig/
LD_FLAGS     =

# Default target
all:

# Include LUFA-specific DMBS extension modules
DMBS_LUFA_PATH ?= $(LUFA_PATH)/Build/LUFA
include $(DMBS_LUFA_PATH)/lufa-sources.mk
include $(DMBS_LUFA_PATH)/lufa-gcc.mk

# Include common DMBS build system modules
DMBS_PATH      ?= $(LUFA_PATH)/Build/DMBS/DMBS
include $(DMBS_PATH)/core.mk
include $(DMBS_PATH)/cppcheck.mk
include $(DMBS_PATH)/doxygen.mk
include $(DMBS_PATH)/dfu.mk
include $(DMBS_PATH)/gcc.mk
include $(DMBS_PATH)/hid.mk
include $(DMBS_PATH)/avrdude.mk
include $(DMBS_PATH)/atprogram.mk


flash:
	@echo "Short Reset and GND on the ICSP port then press ENTRER"
	@read OK
	@echo "====ERASING OLD ATMEGA16U2 FIRMWARE"
	dfu-programmer atmega16u2 erase
	@echo "====FLASHING NEW FIRMWARE"
	dfu-programmer atmega16u2 flash PAD.hex
	@echo "====RESET"
	dfu-programmer atmega16u2 reset
	@echo "Flash Complete"
	
backup:
	@echo "Short Reset and GND on the ICSP port then press ENTRER"
	@read OK
	@echo "====ERASING OLD ATMEGA16U2 FIRMWARE"
	dfu-programmer atmega16u2 erase
	@echo "====FLASHING NEW FIRMWARE"
	dfu-programmer atmega16u2 flash FTDI.hex
	@echo "====RESET"
	dfu-programmer atmega16u2 reset
	@echo "Restore Complete"

list:
	@sudo lsusb -v | grep "f00f:6666" -A88
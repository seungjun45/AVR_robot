###############################################################################
# Makefile for the project test1
###############################################################################

## General Flags
PROJECT = test1
MCU = at90can128
TARGET = test1.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -Os -std=gnu99 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=test1.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = test1.o myDelay.o sonar_pulse.o judge.o steer_myAngle.o motor_pwm.o usartLib.o myCANLib.o encoder_adapt.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) test1.hex test1.eep test1.lss size

## Compile
test1.o: ../test1.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

myDelay.o: ../myDelay.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

sonar_pulse.o: ../sonar_pulse.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

judge.o: ../judge.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

steer_myAngle.o: ../steer_myAngle.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

motor_pwm.o: ../motor_pwm.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

usartLib.o: ../usartLib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

myCANLib.o: ../myCANLib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

encoder_adapt.o: ../encoder_adapt.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) test1.elf dep/* test1.hex test1.eep test1.lss test1.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)


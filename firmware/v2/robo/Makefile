
DEVICE = 18f2550
PROJECT = robo
INCLUDE_PATH = ../include/
LKR_PATH = ../lkr/
OBJS = main.o usb_descriptors.o $(INCLUDE_PATH)Microchip/USB/HID-Device-Driver/usb_function_hid.o $(INCLUDE_PATH)Microchip/USB/usb_device.o

PATH_C18 = /opt/microchip/mplabc18/v3.40/
PATH_FIRMWARE_ICD2 = /opt/microchip/mplabx/mplab_ide/mdbcore/modules/ICD2/

CC = $(PATH_C18)bin/mcc18
CFLAGS = -I $(PATH_C18)h/ -I . -I $(INCLUDE_PATH) -I $(INCLUDE_PATH)Microchip/Include -p $(DEVICE)

LINK = $(PATH_C18)bin/mplink
LFLAGS = -k $(PATH_C18)bin/LKR/ -l $(PATH_C18)lib/ -p $(DEVICE) $(LKR_PATH)rm18f2550.lkr -o $(PROJECT).cof -m $(PROJECT).map -u _CRUNTIME -i -q

.c.o:
	$(CC) $(CFLAGS) -fo=$*.o $*.c

all: $(PROJECT).cof

$(PROJECT).cof: $(OBJS)
	$(LINK) $(LFLAGS) $(OBJS)

clean:
	rm -f $(OBJS) *.$$$ *.bkx *.cce *.cod *.cof *.err *.hex *.i *.lde *.lst *.obj *.o *.rlf *.sym *.sdb *.wat *.mcs *.mptags *.tagsrc *.map *.elf *~

icd2-program:
	piklab-prog -c program -p icd2 -t usb --target-self-powered false --firmware-dir $(PATH_FIRMWARE_ICD2) -d $(DEVICE) $(PROJECT).hex

icd2-erase:
	piklab-prog -c erase -p icd2 -t usb --target-self-powered false --firmware-dir $(PATH_FIRMWARE_ICD2) -d $(DEVICE)

.PHONY: all cleansub
all:
	mkpsxiso -y isoconfig.xml
	rm -f dialog-boxes.cue dialog-boxes.bin
	rm -f *.map
	rm -f *.elf
	rm -f *.o rm -f *.dep
	rm -f *.ps-exe

cleansub:
	$(MAKE) clean
	rm -f dialog-boxes.cue dialog-boxes.bin

TARGET = dialog-boxes
SRCS = Main.c

include ../../Makefile
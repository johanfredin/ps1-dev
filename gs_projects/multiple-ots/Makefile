.PHONY: all cleansub
all:
	mkpsxiso -y isoconfig.xml
	rm -f multiple-ots.cue multiple-ots.bin
	rm -f *.map
	rm -f *.elf
	rm -f *.o rm -f *.dep
	rm -f *.ps-exe

cleansub:
	$(MAKE) clean
	rm -f multiple-ots.cue multiple-ots.bin

TARGET = multiple-ots
SRCS = Main.c

include ../../Makefile
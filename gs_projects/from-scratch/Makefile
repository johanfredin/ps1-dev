.PHONY: all cleansub
all:
	mkpsxiso -y isoconfig.xml
	rm -f from-scratch.cue from-scratch.bin
	rm -f *.map
	rm -f *.elf
	rm -f *.o rm -f *.dep
	rm -f *.ps-exe

cleansub:
	$(MAKE) clean
	rm -f from-scratch.cue from-scratch.bin

TARGET = from-scratch
SRCS = Main.c Map.c

include ../../Makefile
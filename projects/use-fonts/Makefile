.PHONY: all cleansub
all:
	mkpsxiso -y isoconfig.xml
	rm -f use-fonts.cue use-fonts.bin
	rm -f *.map
	rm -f *.elf
	rm -f *.o rm -f *.dep
	rm -f *.ps-exe

cleansub:
	$(MAKE) clean
	rm -f use-fonts.cue use-fonts.bin

TARGET = use-fonts
SRCS = Main.c

include ../../Makefile
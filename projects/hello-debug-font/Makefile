.PHONY: all cleansub
all:
	mkpsxiso -y isoconfig.xml
	rm -f hello-debug-font hello-debug-font.bin
	rm -f *.map
	rm -f *.elf
	rm -f *.o rm -f *.dep
	rm -f *.ps-exe

cleansub:
	$(MAKE) clean
	rm -f hello-debug-font.cue hello-debug-font.bin

TARGET = hello-debug-font
SRCS = Main.c

include ../../Makefile
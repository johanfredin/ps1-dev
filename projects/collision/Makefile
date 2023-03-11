.PHONY: all cleansub
all:
	mkpsxiso -y isoconfig.xml
	rm -f collision.cue collision.bin
	rm -f *.map
	rm -f *.elf
	rm -f *.o rm -f *.dep
	rm -f *.ps-exe

cleansub:
	$(MAKE) clean
	rm -f collision.cue collision.bin

TARGET = collision
SRCS = Main.c

include ../../Makefile
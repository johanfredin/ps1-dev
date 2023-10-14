.PHONY: all cleansub
all:
	mkpsxiso -y isoconfig.xml
	rm -f camera.cue camera.bin
	rm -f *.map
	rm -f *.elf
	rm -f *.o rm -f *.dep
	rm -f *.ps-exe

cleansub:
	$(MAKE) clean
	rm -f camera.cue camera.bin

TARGET = camera
SRCS = Main.c

include ../../Makefile
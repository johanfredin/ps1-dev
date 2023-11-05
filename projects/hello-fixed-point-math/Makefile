.PHONY: all cleansub
all:
	mkpsxiso -y isoconfig.xml
	rm -f hello-fixed-point-math hello-fixed-point-math.bin
	rm -f *.map
	rm -f *.elf
	rm -f *.o rm -f *.dep
	rm -f *.ps-exe

cleansub:
	$(MAKE) clean
	rm -f hello-fixed-point-math.cue hello-fixed-point-math.bin

TARGET = hello-fixed-point-math
SRCS = Main.c

include ../../Makefile
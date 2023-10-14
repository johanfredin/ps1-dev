.PHONY: all cleansub
all:
	mkpsxiso -y isoconfig.xml
	rm -f hello-flat-shaded-polys hello-flat-shaded-polys.bin
	rm -f *.map
	rm -f *.elf
	rm -f *.o rm -f *.dep
	rm -f *.ps-exe

cleansub:
	$(MAKE) clean
	rm -f hello-flat-shaded-polys.cue hello-flat-shaded-polys.bin

TARGET = hello-flat-shaded-polys
SRCS = Main.c

include ../../Makefile
.PHONY: all cleansub
all:
	mkpsxiso -y isoconfig.xml
	rm -f hello-controllers hello-controllers.bin
	rm -f *.map
	rm -f *.elf
	rm -f *.o rm -f *.dep
	rm -f *.ps-exe

cleansub:
	$(MAKE) clean
	rm -f hello-controllers.cue hello-controllers.bin

TARGET = hello-controllers
SRCS = Main.c

include ../../Makefile
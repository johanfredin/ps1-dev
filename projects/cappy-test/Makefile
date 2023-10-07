.PHONY: all cleansub
all:
	mkpsxiso -y isoconfig.xml
	rm -f cappy-test.cue cappy-test.bin
	rm -f *.map
	rm -f *.elf
	rm -f *.o rm -f *.dep
	rm -f *.ps-exe

cleansub:
	$(MAKE) clean
	rm -f cappy-test.cue cappy-test.bin

TARGET = cappy-test
SRCS = Main.c

include ../../Makefile
.PHONY: all cleansub
all:
	mkpsxiso -y isoconfig.xml
	rm -f tim-loading.cue tim-loading.bin
	rm -f *.map
	rm -f *.elf
	rm -f *.o rm -f *.dep
	rm -f *.ps-exe

cleansub:
	$(MAKE) clean
	rm -f tim-loading.cue tim-loading.bin

TARGET = tim-loading
SRCS = Main.c

include ../../Makefile
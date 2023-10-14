.PHONY: all cleansub
all:
	mkpsxiso -y isoconfig.xml
	rm -f draw-primes-in-ot.cue draw-primes-in-ot.bin
	rm -f *.map
	rm -f *.elf
	rm -f *.o rm -f *.dep
	rm -f *.ps-exe

cleansub:
	$(MAKE) clean
	rm -f draw-primes-in-ot.cue draw-primes-in-ot.bin

TARGET = draw-primes-in-ot
SRCS = Main.c

include ../../Makefile
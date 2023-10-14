.PHONY: all cleansub
all:
	mkpsxiso -y isoconfig.xml
	rm -f play-sounds-spu.cue play-sounds-spu.bin
	rm -f *.map
	rm -f *.elf
	rm -f *.o rm -f *.dep
	rm -f *.ps-exe

cleansub:
	$(MAKE) clean
	rm -f play-sounds-spu.cue play-sounds-spu.bin

TARGET = play-sounds-spu
SRCS = Main.c

include ../../Makefile
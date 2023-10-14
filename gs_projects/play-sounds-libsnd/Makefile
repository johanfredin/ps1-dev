.PHONY: all cleansub
all:
	mkpsxiso -y isoconfig.xml
	rm -f play-sounds-libsnd.cue play-sounds-libsnd.bin
	rm -f *.map
	rm -f *.elf
	rm -f *.o rm -f *.dep
	rm -f *.ps-exe

cleansub:
	$(MAKE) clean
	rm -f play-sounds-libsnd.cue play-sounds-libsnd.bin

TARGET = play-sounds-libsnd
SRCS = Main.c

include ../../Makefile
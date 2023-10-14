.PHONY: all cleansub
all:
	mkpsxiso -y isoconfig.xml
	rm -f play-sounds-cdda.cue play-sounds-cdda.bin
	rm -f *.map
	rm -f *.elf
	rm -f *.o rm -f *.dep
	rm -f *.ps-exe

cleansub:
	$(MAKE) clean
	rm -f play-sounds-cdda.cue play-sounds-cdda.bin

TARGET = play-sounds-cdda
SRCS = Main.c

include ../../Makefile
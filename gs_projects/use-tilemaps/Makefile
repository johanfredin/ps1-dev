.PHONY: all cleansub
all:
	mkpsxiso -y isoconfig.xml
	rm -f use-tilemaps.cue use-tilemaps.bin
	rm -f *.map
	rm -f *.elf
	rm -f *.o rm -f *.dep
	rm -f *.ps-exe

cleansub:
	$(MAKE) clean
	rm -f use-tilemaps.cue use-tilemaps.bin

TARGET = use-tilemaps
SRCS = Main.c Map.c

include ../../Makefile
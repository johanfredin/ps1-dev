.PHONY: all cleansub
all:
	mkpsxiso -y isoconfig.xml
	rm -f hello-textured-sprites hello-textured-sprites.bin
	rm -f *.map
	rm -f *.elf
	rm -f *.o rm -f *.dep
	rm -f *.ps-exe

cleansub:
	$(MAKE) clean
	rm -f hello-textured-sprites.cue hello-textured-sprites.bin

TARGET = hello-textured-sprites
SRCS = Main.c

include ../../Makefile
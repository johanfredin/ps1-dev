.PHONY: all cleansub
all:
	mkpsxiso -y isoconfig.xml
	rm -f $template $template.bin
	rm -f *.map
	rm -f *.elf
	rm -f *.o rm -f *.dep
	rm -f *.ps-exe

cleansub:
	$(MAKE) clean
	rm -f $template.cue $template.bin

TARGET = $template
SRCS = Main.c

include ../../Makefile
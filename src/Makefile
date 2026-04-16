# Makefile for tracex

ifeq ($(notdir $(CURDIR)),src)
$(error do not execute make in src directory)
endif

AS = has060
ASFLAGS = -w3
CC = gcc2
CFLAGS = -Wall -O2 -fall-bsr
U8TOSJ = u8tosj

TARGET = trace.x
SRCS = trace.c argument.c display.c error.c handler.s syscall.c version.c
OBJS = $(foreach f,$(SRCS),$(basename $(f)).o)
DOCS = README.txt CHANGELOG.txt LICENSE

ARCHIVE_ZIP = trx.zip


.PHONY: all archive clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o$@ $^

handler.o : handler.s
	$(AS) $(ASFLAGS) -o$@ $<

trace.o: trace.h
argument.o: trace.h syscall.h
display.o: trace.h error.h
syscall.o: syscall.h
error.o: error.h


ifdef SRC_DIR
archive: $(ARCHIVE_ZIP)
else
archive: ../srcdir.txt
	$(if $(SRC_DIR_IS_DEFINED),$(error SRC_DIR is empty.))
	make $@ SRC_DIR_IS_DEFINED=yes SRC_DIR=$(shell cat ../srcdir.txt)
../srcdir.txt:
	$(error file 'srcdir.txt' created by src2build is required.)
endif

$(ARCHIVE_ZIP): $(DOCS) $(TARGET)
	strip $(TARGET)
	rm -f $@
	zip -9 $@ $^

%.txt: $(SRC_DIR)/../%.md
	rm -f $@
	$(U8TOSJ) < $^ > $@

LICENSE: $(SRC_DIR)/../LICENSE
	rm -f $@
	cp -p $< $@


clean:
	rm -f $(ARCHIVE_ZIP) $(DOCS) $(TARGET) $(OBJS)

# EOF

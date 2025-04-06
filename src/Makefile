# Makefile for trace
# K.Abe

.PHONY:	all lzh ish

CFLAGS=-Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions\
	-fall-bsr -fcombine-regs -fforce-mem -fforce-addr

OBJS=trace.o handler.o argument.o display.o

SRCS=trace.c argument.c display.c handler.s syscall.h error.h

ARCHIVE_STUFF=README.DOC trace.1 trace.x trace_src.Lzh

all: trace.x

lzh: trace.Lzh

trace.Lzh: $(ARCHIVE_STUFF)
	xpack trace.x
	lha u trace.Lzh $^

trace_src.Lzh: $(SRCS) Makefile
	lha u trace_src.Lzh $^

ish: trace.Lzh
	mic -s -o trace.Lzh
	wc -l trace.ish

trace.x: $(OBJS)
	gcc -o trace.x -ldos -liocs $(OBJS)

###
argument.o: trace.h syscall.h
display.o: trace.h error.h
trace.o: trace.h

# Makefile for tracex

AS = has060
ASFLAGS = -w3
CC = gcc2
CFLAGS = -Wall -O2 -fall-bsr


SRCS = trace.c argument.c display.c handler.s syscall.c version.c
OBJS = $(foreach f,$(SRCS),$(basename $(f)).o)
TARGET = trace.x


.PHONY : all

all : $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o$@ $^

handler.o : handler.s
	$(AS) $(ASFLAGS) -o$@ $<

trace.o: trace.h
argument.o: trace.h syscall.h
display.o: trace.h error.h
syscall.o: syscall.h

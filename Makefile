CC = gcc
LDFLAGS = -L/usr/lib -lglut -lGLU -lGL -lm
CFLAGS = -g -Wall

EXENAME = brain

TARGETS = brain.c list.h

brain: brain.c list.o

clean:
	rm *.o brain

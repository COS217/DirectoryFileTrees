#--------------------------------------------------------------------
# Makefile for Assignment 4, Part 3 sample implementation
# Invoke with the command:
# 	make -f Makefile.sampleft
# Author: Christopher Moretti
#--------------------------------------------------------------------

CC=gcc217

all: sampleft

clean:
	rm -f sampleft

clobber: clean
	rm -f ft_client.o *~

sampleft: sampleft.o ft_client.o
	$(CC) sampleft.o ft_client.o -o sampleft

ft_client.o: ft_client.c ft.h a4def.h
	$(CC) -c ft_client.c

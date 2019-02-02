CC=gcc
CFLAGS=-c -Wall
all: numberreader xyztotalizer
numberreader: numberreader.c
	$(CC) numberreader.c -o numberreader
xyztotalizer: xyztotalizer.c
	$(CC) xyztotalizer.c -o xyztotalizer -l pthread
						
clean: rm -rf *o xyztotalizer		

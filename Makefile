# -*- mode: makefile -*-
#
# Time-stamp: <2022-07-27 17:37:14 stefan>
#

CC = /usr/bin/gcc
CFLAGS = -g -Wall -std=c99 -fanalyzer -fdump-analyzer-supergraph

INCLUDES = game_of_life.h game_of_life_file_handler.h
MP9SOURCE_C = mp9.c game_of_life.c game_of_life_file_handler.c
MP9OBJS = $(MP9SOURCE_C:.c=.o)

MP9SUPERGRAPHS = $(MP9OBJS:.o=.c.supergraph.dot)
MP9SUPERGRAPHSEQ = $(MP9SUPERGRAPHS:.o=.c.supergraph-eg.dot)
MP9SUPERGRAPHSPNG = $(MP9SUPERGRAPHS:.dot=.png)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

%.png: %.dot
	/usr/bin/dot -Tpng $< -o $@

all: mp9

mp9: $(MP9OBJS)
	$(CC) $(CFLAGS) $(MP9OBJS) -o $@

clean:
	rm *.o mp9

graphs: $(MP9SUPERGRAPHSPNG)

# mål för test via expect
# semaphore:

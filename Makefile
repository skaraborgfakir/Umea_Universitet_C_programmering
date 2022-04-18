#!/usr/bin/make -f
# Time-stamp: <2022-04-18 14:17:24 stefan>
#

INCLUDES = game_of_life.h game_of_life_file_handler.h
MP9SOURCE_C = mp9.c game_of_life.c game_of_life_file_handler.c
MP9OBJS = $(MP9SOURCE_C:.c=.o)
#MP9OBJS : $(INCLUDES)

CC = /usr/bin/gcc
CFLAGS = -Wall -std=c99

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

all: mp9

mp9: $(MP9OBJS)
	$(CC) $(CFLAGS) $(MP9OBJS) -o $@

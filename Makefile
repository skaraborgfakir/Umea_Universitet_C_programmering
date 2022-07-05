# Time-stamp: <2022-07-04 17:45:34 stefan>
#

CC = /usr/bin/gcc
CFLAGS = -g -Wall -std=c99

mp8: mp8.c
	$(CC) $(CFLAGS) mp8.c -o $@

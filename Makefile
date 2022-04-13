# Time-stamp: <2022-04-01 22:44:52 stefan>
#

CC = /usr/bin/gcc
CFLAGS = -Wall -std=c99

mp8: mp8.c
	$(CC) $(CFLAGS) mp8.c -o $@

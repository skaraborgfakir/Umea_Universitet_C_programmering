# Time-stamp: <2022-04-01 21:35:31 stefan>
#

CC = /usr/bin/gcc
CFLAGS = -Wall -std=c99

mp5:
	$(CC) mp5.c -o $@

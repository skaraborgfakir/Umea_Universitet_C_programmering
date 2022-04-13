# Time-stamp: <2022-04-01 22:44:51 stefan>
#

CC = /usr/bin/gcc
CFLAGS = -Wall -std=c99

mp5: mp5.c
	$(CC) mp5.c -o $@

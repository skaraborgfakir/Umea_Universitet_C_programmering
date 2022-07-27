# Time-stamp: <2022-07-27 17:37:53 stefan>
#

CC = /usr/bin/gcc
CFLAGS = -g -Wall -std=c99 -fanalyzer -fdump-analyzer-supergraph
MP8SOURCE_C = mp8.c
MP8OBJS = $(MP8SOURCE_C:.c=.o)

MP8SUPERGRAPHS = $(MP8OBJS:.o=.c.supergraph.dot)
MP8SUPERGRAPHSEQ = $(MP8SUPERGRAPHS:.o=.c.supergraph-eg.dot)
MP8SUPERGRAPHSPNG = $(MP8SUPERGRAPHS:.dot=.png)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

%.png: %.dot
	/usr/bin/dot -Tpng $< -o $@

all: mp8

mp8: $(MP8OBJS)
	$(CC) $(CFLAGS) $(MP8OBJS) -o $@

clean:
	rm *.o mp8

graphs: $(MP8SUPERGRAPHSPNG)

# mål för test via expect
custom:
	expect -c ./mp8 -f ./custom.expect

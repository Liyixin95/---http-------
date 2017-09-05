CC = gcc
CFLAGS = -Wall -g

test:download.c http.c
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	$(RM) .*.sw? test debug *.o

.PHONY:all clean

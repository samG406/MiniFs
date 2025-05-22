CC = gcc
CFLAGS = -Wall -g

OBJS = main.o disk.o cache.o fs_core.o utils.o

minifs: $(OBJS)
	$(CC) $(CFLAGS) -o minifs $(OBJS)

main.o: main.c fs_core.h cache.h disk.h utils.h
fs_core.o: fs_core.c fs_core.h disk.h cache.h
cache.o: cache.c cache.h
disk.o: disk.c disk.h
utils.o: utils.c utils.h

clean:
	rm -f *.o minifs

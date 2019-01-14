CFLAGS= -g -c -Wall

all: clean hello clean_o

hello: main.o server.o mynfs_error.o mynfs_utility.o
	gcc main.o server.o mynfs_error.o mynfs_utility.o -o server

mynfs_utility.o: mynfs_utility.c
	gcc $(CFLAGS) mynfs_utility.c

mynfs_error.o: mynfs_error.c
	gcc $(CFLAGS) mynfs_error.c

server.o: server.c
	gcc $(CFLAGS) server.c

main.o: main.c
	gcc $(CFLAGS) main.c

clean_o:
	rm -rf *.o

clean:
	rm -rf *.o server

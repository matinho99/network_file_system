CFLAGS= -g -c -Wall

all: hello clean_o

hello: main.o client.o mynfs_error.o mynfs_utility.o
	gcc main.o client.o mynfs_error.o mynfs_utility.o -o hello

mynfs_utility.o: mynfs_utility.c
	gcc $(CFLAGS) mynfs_utility.c

mynfs_error.o: mynfs_error.c
	gcc $(CFLAGS) mynfs_error.c

client.o: client.c
	gcc $(CFLAGS) client.c

main.o: main.c
	gcc $(CFLAGS) main.c

clean_o:
	rm -rf *.o

clean:
	rm -rf *.o hello

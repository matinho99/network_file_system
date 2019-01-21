CFLAGS= -g -c -Wall

all: clean lib hello clean_o

lib: client.o mynfs_utility.o mynfs_error.o
	ar rcs libmynfsclient.a client.o mynfs_utility.o mynfs_error.o
	
hello:
	gcc -o client main.c -L. -lmynfsclient

mynfs_utility.o:
	gcc $(CFLAGS) mynfs_utility.c

mynfs_error.o:
	gcc $(CFLAGS) mynfs_error.c

client.o:
	gcc $(CFLAGS) client.c

clean_o:
	rm -rf *.o

clean:
	rm -rf *.o client

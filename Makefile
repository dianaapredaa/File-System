all: build

build:
	gcc -g main.c tree.c -o sd_fs
	valgrind --leak-check=full ./sd_fs

clean:
	rm *.o sd_fs

run:
	valgrind --leak-check=full ./sd_fs

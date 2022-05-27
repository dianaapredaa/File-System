CFLAGS=-g -m32 -Wall -Wextra -std=c99 

all: build

build:
	gcc $(CFLAGS) main.c tree.c my_strdup.c -o sd_fs

clean:
	rm *.o sd_fs

run:
	./sd_fs

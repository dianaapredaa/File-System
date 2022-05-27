# Copyright 2021 - 2022 Omer Tarik Ilhan & Preda Diana 314CA
CFLAGS=-g -m32 -Wall -Wextra -std=c99 
CFILES=main.c tree.c my_strdup.c utils.c

all: build

build:
	gcc $(CFLAGS) $(CFILES) -o sd_fs

clean:
	rm *.o sd_fs

run:
	valgrind --leak-check=full ./sd_fs

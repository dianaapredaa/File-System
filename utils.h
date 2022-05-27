// Copyright 2021 - 2022 Omer Tarik Ilhan & Preda Diana 314CA
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

#include <errno.h>
#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(errno);					\
		}							\
	} while (0)

void free_node(TreeNode* node);

TreeNode* get_path(TreeNode* currentNode, char* path);

#endif  // UTILS_H

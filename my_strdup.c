// Copyright 2021 - 2022 Omer Tarik Ilhan & Preda Diana 314CA
#include <string.h>
#include <stdlib.h>

#include "utils.h"

// I implemented my own strdup() function
// because there is no such function in c99 (required by the checker)
char* strdup(char* string)
{
    if (string == NULL)
        return NULL;
    // malloc duplicate
    char* duplicate = malloc(strlen(string) + 1);
    DIE(duplicate == NULL, "strdup failed");
    // copy string to duplicate
    memcpy(duplicate, string, strlen(string) + 1);
    // return duplicate
    return duplicate;
}

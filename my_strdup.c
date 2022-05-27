#include <string.h>
#include <stdlib.h>

// I implemented my own strdup() function
// because there is no such function in c99 (required by the checker)
char* strdup(char* string)
{
    if (string == NULL)
        return NULL;
    // malloc duplicate
    char* duplicate = malloc(strlen(string) + 1);
    // copy string to duplicate
    memcpy(duplicate, string, strlen(string) + 1);
    // return duplicate
    return duplicate;
}

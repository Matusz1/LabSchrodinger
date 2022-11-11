#include <stdio.h>
#include <stdlib.h>
#include "util.h"

void* msg_malloc(size_t size, const char* message) {

    char* p = malloc(size);
    /* If failed and there is message, print it and exit */
    if (p == NULL && message != NULL) {
        fprintf(stderr, " # ERROR: malloc failed for %s", message);
        exit(EXIT_FAILURE);
    }
    /* else, just return pointer */
    return p;
}
 
void* msg_realloc(void* p, size_t size, const char* message) {

    /* Special case when p = NULL, we just allocate memory */
    if (p == NULL) {
        return msg_malloc(size, message);
    }

    char* r = realloc(p, size);
    if (r == NULL && message != NULL) {
        fprintf(stderr, " # ERROR: malloc failed for %s", message);
        exit(EXIT_FAILURE);
    }

    return p;
}

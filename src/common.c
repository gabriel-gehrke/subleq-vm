#include <stdlib.h>
#include <stdio.h>
#include "common.h"

static inline void swap(char* a, char* b);

char readchar(FILE* file) {
    char result;
    if (fread(&result, 1, 1, file) == 1) {
        return result;
    } else {
        return EOF;
    }
}

int endiannize(int i) {
    char temp;
    char* byteptr = (char*) &i;
    swap(byteptr, byteptr + 3);
    swap(byteptr + 1, byteptr + 2);
    return i;
}

void swap (char* a, char* b) {
    char temp = *a;
    *a = *b;
    *b = temp;
}
#pragma once
#include <stdlib.h>
#include <stdint.h>

/**
 * @brief Inverts the endianness of a 32-bit integer.
 */
int endiannize(int i);

/**
 * @brief Reads a single character from a file stream.
 * @return A valid character if succeeded, EOF if end of file was reached or something went wrong.
 */
char readchar(FILE* file);
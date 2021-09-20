#pragma once
#include <stdlib.h>
#include "common.h"

typedef struct _SUBLEQ_CMD
{
    int addr0; // the address of the first operand
    int addr1; // the address of the second operand
    int baddr; // the branch address
} SUBLEQ_CMD;


int subleq_assemble(FILE* input, FILE* output);

/**
 * @brief Parses a line of SUBLEQ assembly and writes the result to the pointed to SUBLEQ_CMD data type.
 * @param line The null-terminated string to parse.
 * @param line_number the line number in the source file, used for marking errors.
 * @param cmd a pointer to a SUBLEQ_CMD data type.
 * @return 0 if everything went well, -1 if an error occured.
 */
int subleq_assemble_line(const char* line, int line_number, SUBLEQ_CMD* cmd);

void syntax_error(const char* string, int line_number, const char* reason);

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "common.h"
#include "subleq-asm.h"

/**
 * @brief The main method of the subleq assembler. It checks the given parameters, handles the files and calls other methods.
 */
int main(int args_len, char** args) {
    if (args_len != 3) {
        puts("Invalid Arguments. Use the assembler like this:\n\n./subleq-asm [source file] [target file]\n");
        return 1;
    }

    char* src_path = args[1];
    char* dest_path = args[2];

    // open source file, read only, write error message if file couldn't be found
    FILE* src_file = fopen(src_path, "r"); 
    if (src_file == NULL) {
        printf("Error: Could not access file \"%s\"!\n", src_path);
        return errno;
    }

    // open/create binary destination file, overwrite it's contents
    FILE* dest_file = fopen(dest_path, "wb");

    // assemble
    if (subleq_assemble(src_file, dest_file) != 0) puts("Assembling Failed.\n");

    // close files
    fclose(src_file);
    fclose(dest_file);
}

int subleq_assemble(FILE* src_file, FILE* dest_file) {
    // allocate temporary buffer on stack
    char buffer[255];
    char next;
    int line_number = 1;

    // read the source file, assemble line by line
    do {
        for (int i = 0; i < sizeof(buffer); i++) {
            next = readchar(src_file);
            // break if end of line or end of file is reached
            if (next == '\n' || next == EOF) {
                buffer[i] = 0;
                break;
            }
            buffer[i] = next;
        }

        printf("line: %s\n", buffer);
        if (strlen(buffer) == 0) continue;

        // parse a subleq command
        SUBLEQ_CMD cmd; 
        if (subleq_assemble_line(buffer, line_number++, &cmd) != 0) {
            return 1;
        }

        // write to binary output file
        fwrite(&cmd, sizeof(SUBLEQ_CMD), 1, dest_file);

    } while (next != EOF);

    return 0;
}

int subleq_assemble_line(const char* line, int line_number, SUBLEQ_CMD* cmd) {
    const char* WHITESPACE = " \t";
    
    // array of properties to write to
    int* ptrs[] = {&(cmd->addr0), &(cmd->addr1), &(cmd->baddr)};
    int l = sizeof(ptrs) / sizeof(int*);

    // split the line by whitespace
    char* part = strtok(line, WHITESPACE);
    for (int i = 0; i < l; i++) {

        // check if eol was reached
        if (part == NULL) {
            syntax_error(line, line_number, "too few arguments");
            return -1;
        }

        char* endptr; // used by strtol to point to last processed char
        int parsed_number = (int)strtol(part, &endptr, 0);

        // if the endptr is not on the terminating zero, the conversion failed
        if (*endptr != '\0') {
            syntax_error(part, line_number, "not a number");
            return -1;
        }

        // invert endianness of number, store to current pointer
        *(ptrs[i]) = parsed_number;
        
        // next part
        part = strtok(NULL, WHITESPACE);
    }

    return 0;
}

void syntax_error(const char* string, int line_number, const char* reason) {
    printf("Syntax Error (Line %d): \"%s\" [%s]\n", line_number, string, reason);
}
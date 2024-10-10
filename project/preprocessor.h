/*@by Shira Kedmi*/

#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "macro_structure.h"
#include "global_const.h"

/* Number of instructions available in the program */
#define NUM_OF_INSTRUCTIONS 16

/* Number of directives available in the program */
#define NUM_OF_DIRECTIVES 4

/* Code representing an error */
#define ERROR_FOUND -1

/* File suffix for output */
#define SUFFIX_AM ".am"

/* File suffix for input */
#define SUFFIX_AS ".as"



/* Enumeration for different types of lines */
typedef enum {
    START_MACRO_DEFINITION = 1,  /* Start of macro definition */
    END_MACRO_DEFINITION,        /* End of macro definition */
    CALL_MACRO,                  /* Macro call */
    SOME_LINE_CODE               /* General line of code */
} LineType;

/*
 * Preprocesses the given file and updates the macro table.
 *
 * @param file_name - The name of the file to preprocess.
 * @param macro_table - A pointer to the macro table to be updated.
 * @return An integer indicating success (0) or failure (ERROR_FOUND).
 */
int preprocessor(char *file_name, Macro **macro_table);

/*
 * Identifies the type of a line based on its content and updates the macro pointer.
 *
 * @param line_parsing - The line of text to parse.
 * @param macro_pointer - A pointer to the current macro being processed.
 * @param macro_table - A pointer to the macro table.
 * @param line_number - The line number of the line being processed.
 * @return An integer representing the type of the line (one of LineType enum values).
 */
int identify_line_type(char *line_parsing, Macro **macro_pointer, Macro **macro_table, int line_number);

#endif


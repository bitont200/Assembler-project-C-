/*@by Shira Kedmi*/

#ifndef MACRO_STRUCTURE_H
#define MACRO_STRUCTURE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LENGTH_MACRO_NAME 100 /* Maximum length of a macro name */

/*
 * Macro:
 *
 * Structure to store macro data.
 * @param macro_name Name of the macro
 * @param macro_content Pointer to an array of lines in the macro
 * @param num_macro_lines Number of lines in the macro
 * @param next Pointer to the next macro in the list
 */
typedef struct Macro {
    char macro_name[LENGTH_MACRO_NAME];
    char **macro_content;
    int num_macro_lines;
    struct Macro *next;
} Macro;

/**
 * add_line_to_macro:
 *
 * Adds a line of code to the macro.
 * @param macro Pointer to the Macro structure
 * @param line_to_add The line of code to add to the macro
 */
void add_line_to_macro(Macro *macro, char *line_to_add);

/**
 * find_macro:
 *
 * Searches for a macro by name in the macro list.
 * @param head Pointer to the head of the macro list
 * @param macroName Name of the macro to search for
 * @return Pointer to the Macro structure if found, NULL otherwise
 */
Macro *find_macro(Macro *head, char *macroName);

/**
 * free_macro_table:
 *
 * Frees the memory allocated for the macro table.
 * @param head Pointer to the head of the macro list
 */
void free_macro_table(Macro **head);

#endif


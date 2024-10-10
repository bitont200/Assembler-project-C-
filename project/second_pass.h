/*@by Tamar Biton*/

#ifndef SECOND_PASS
#define SECOND_PASS

#include <string.h>
#include <stdio.h>

#include "second_pass_func.h"

/**
 * second_pass:
 * 
 * Function that goes over the file for the second time checks and translate.
 * @param table Pointer to the assembler table structure
 * @param file Pointer to the input file containing assembly code
 * @param macro Pointer to the macto struct in order to check if symbols are the same as macro
 */
int second_pass(struct assembler_table *table, FILE *file, Macro *macro);

#endif /*SECOND_PASS*/

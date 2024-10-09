#ifndef FIRST_PASS
#define FIRST_PASS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "first_pass_func.h"

/**
 * first_pass:
 * 
 * Function that goes over the file for the first time checks and translate.
 * @param table Pointer to the main table.
 * @param file Pointer to the file that we are going trow
 * @param macro Pointer to the macto struct in order to check if symbols are the same as macro
 */
int first_pass(struct assembler_table *table, FILE *file, Macro *macro);

#endif /* FIRST_PASS */

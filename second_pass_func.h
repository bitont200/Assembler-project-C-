#ifndef _SECOND_PASS_FUNC_
#define _SECOND_PASS_FUNC_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "line_structure.h"
#include "tables.h"

/* Defines various constants used in the program */
#define LABEL 3          /*Position of the label in the translated word*/

/**
 * add_symbols_code:
 * 
 * This function adds symbols and code based on line structure.
 * @param lineStc Pointer to the line structure
 * @param table Pointer to the assembler table structure
 * @param lineNum Line number in the input file
 * @return ERROR if has errors, SUCCESS otherwise
 */
int add_symbols_code(struct lineStc *lineStc, struct assembler_table *table, int lineNum);

/**
 * handle_operand_code:
 * 
 * This function handles operand codes based on their types and translate it
 * @param table Pointer to the assembler table structure
 * @param lineStc Pointer to the line structure
 * @param operandNum Number of the operand to handle
 * @param lineNum The line number
 */
int handle_operand_code(struct assembler_table *table, struct lineStc *lineStc, int operandNum, int lineNum);

#endif /* _SECOND_PASS_FUNC_ */ 
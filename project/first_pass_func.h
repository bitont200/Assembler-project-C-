/*@by Tamar Biton*/

#ifndef FIRST_PASS_FUNC
#define FIRST_PASS_FUNC

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tables.h"
#include "line_structure.h"

/* Defines various constants used in the program */
#define OPCODE_POSITION 11     /*Position of the opcode in the translated word*/
#define SOURCE_OPERAND_SORT 7  /*Position of the source operand in the translated word*/
#define TARGET_OPERAND_SORT 3  /*Position of the destination operand in the translated word*/
#define ARE_FIELD 2            /*Position of the ARE code in the translated word*/
#define SOURCE_OPERAND 6       /*Position of the source operand in the translated word for the operand*/
#define TARGET_OPERAND 3       /*Position of the destination operand in the translated word for the operand*/
#define IMMIDIATE 3            /*Position of the immidiate in the translated word for the operand*/


/**
 * has_error:
 * 
 * Checks if a line structure contains errors, and if so prints ERROR and continue.
 * @param lineStc Pointer to the line structure.
 * @param lineNum Line number in the file.
 * @return ERROR if there is an error.
 */
int has_error(struct lineStc *lineStc, int lineNum);
 
/**
 * handle_if_entry:
 * 
 * Checks if symbol the symbol in front of the line is entry, if so handle the different aspects.
 * @param lineStc Pointer to the line structure.
 * @param table Pointer to the table.
 * @param lineNum Line number in the file.
 * @return ERROR if there is an error, SUCCESS otherwise.
 */
int handle_if_entry(struct lineStc *lineStc, struct assembler_table *table, int lineNum);

/**
 * handle_if_extern_or_entry:
 * 
 * Checks if the line is entry line or extern line and checks the diffrent aspects.
 * @param lineStc Pointer to the line structure.
 * @param table Pointer to the table.
 * @param lineNum Line number in the file.
 * @return ERROR if there is an error, SUCCESS otherwise.
 */
int handle_if_extern_or_entry(struct lineStc *lineStc, struct assembler_table *table, int lineNUm);

/**
 * handle_first_code_line:
 * 
 * Translate the first line of the code according to the line structure.
 * @param table Pointer to the table.
 * @param lineStc Pointer to the line structure.
 */
void handle_first_code_line(struct assembler_table *table, struct lineStc *lineStc);

/**
 * handle_if_both_registers:
 * 
 * Translate the code if its a command where both operands are registers.
 * @param table Pointer to the table.
 * @param lineStc Pointer to the line structure.
 */
void handle_if_both_registers(struct assembler_table *table, struct lineStc *lineStc);

/**
 * handle_operand:
 * 
 * Translate the different operand types into binary
 * @param table Pointer to the table.
 * @param lineStc Pointer to the line structure.
 * @param operandNum Number of operand to translate.
 */
void handle_operand(struct assembler_table *table, struct lineStc *lineStc, int operandNum);

/**
 * handle_directive:
 * 
 * Handle the line if its directive in the first pass.
 * @param lineStc Pointer to the line structure.
 * @param table Pointer to the assembler table.
 * @param lineNum Line number in the file.
 * @return ERROR if there is an error, SUCCESS otherwise.
 */
int handle_directive(struct lineStc *lineStc, struct assembler_table *table, int lineNum);

/**
 * handle_instruction:
 * 
 * Handle the line if its instriction in the first pass.
 * @param lineStc Pointer to the line structure.
 * @param table Pointer to the assembler table.
 * @param lineNum Line number in the file.
 * @return ERROR if there is an error, SUCCESS otherwise.
 */
int handle_instruction(struct lineStc *lineStc, struct assembler_table *table, int lineNum);

/**
 * check_and_update_symbol_table:
 * 
 * Goes over the symbol table and checks for logical errors, if there are prints error.
 * @param table Pointer to the assembler table.
 * @param lineNum Line number in the file.
 * @return ERROR if there is an error, SUCCESS otherwise.
 */
int check_and_update_symbol_table(struct assembler_table *table, int lineNum);

#endif /* FIRST_PASS_FUNC*/

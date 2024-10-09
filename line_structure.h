#ifndef LINE_STRUCTURE_H
#define LINE_STRUCTURE_H

#include "command_parsing.h"

#define LENGTH_OPERAND_STRING 5 

/*
 * Structure representing a legal instruction.
 *
 * Members:
 * - name_inst: The name of the instruction (e.g., "mov", "add").
 * - opcode_inst: The opcode associated with the instruction.
 * - source_operand: The source operand for the instruction.
 * - dest_operand: The destination operand for the instruction.
 */
struct legal_instruction {
    char name_inst[LENGTH_OPERAND_STRING];  /* Instruction name */
    int opcode_inst;                        /* Instruction opcode */
    char source_operand[LENGTH_OPERAND_STRING]; /* Source operand */
    char dest_operand[LENGTH_OPERAND_STRING];    /* Destination operand */
};

/*
 * Function to get a line structure from a given line string.
 *
 * Parameters:
 * - string_line: The line string to be parsed.
 * - mac: Pointer to a Macro list for macro resolution.
 *
 * Returns:
 * - A `struct lineStc` containing the parsed line structure.
 */
struct lineStc get_line_structure_from_line(char* string_line, Macro *mac);

#endif


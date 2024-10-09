#ifndef COMMAND_PARSING_H
#define COMMAND_PARSING_H

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "macro_structure.h"
#include "global_const.h"

/* Defines various constants used in the program */
#define SPACES                " \t\v\f"           /* Characters considered as whitespace */
#define MAX_OPERANDS          2                   /* Maximum number of operands per instruction */
#define INVALID_LABEL_NAME_COUNT 28              /* Number of invalid label names */
#define NUM_OF_INSTRUCTION    16                  /* Number of supported instructions */
#define NUM_OF_REGISTERS      8                   /* Number of registers */
#define ADDRESSING_METHOD_0   0                   /* Addressing method 0 */
#define ADDRESSING_METHOD_1   1                   /* Addressing method 1 */
#define ADDRESSING_METHOD_2   2                   /* Addressing method 2 */
#define ADDRESSING_METHOD_3   3                   /* Addressing method 3 */
#define ERROR_DATA            -16385              /* Error code for data errors */
#define ERROR_IMM             -2049               /* Error code for immediate value errors */
#define MAX_NUM_IMM           2047                /* Maximum value for immediate values */
#define MIN_NUM_IMM           -2048               /* Minimum value for immediate values */
#define MAX_NUM_DATA          16383               /* Maximum value for data values */
#define MIN_NUM_DATA          -16384              /* Minimum value for data values */
#define DECIMAL_NUM           10                  /*Decimal number for casting*/

/* Defines the structure for operand type */
typedef struct {
    int type;          /* Type of operand */
    int value;         /* Value of the operand */
} operand_type;

/* Defines the structure for line division */
struct line_division {
    char *substrings[MAX_SIZE_DATA]; /* Array of substrings */
    int strings_count;               /* Number of substrings */
};

/* Defines the structure for instructions */
struct instruction {
    /* Enumeration of instruction types */
    enum {
        mov, cmp, add, sub, lea, clr, not, inc, dec, jmp, bne, red, prn, jsr, rts, stop
    } instructionType;

    /* Defines the operands for the instruction */
    struct {
        /* Enumeration of operand types */
        enum {
            immidiate,    /* Immediate value */
            label,        /* Label */
            indirectReg,  /* Indirect register */
            directReg,    /* Direct register */
            none
        } operandType;

        /* Union of operand options */
        union {
            int immidiate;    /* Immediate value */
            char label[MAX_LABEL_LENGTH]; /* Label name */
            int indirectReg;  /* Indirect register name */
            int directReg;    /* Direct register number */
        } operandOption;
    } operands[MAX_OPERANDS]; /* Array of operands (up to 2) */
};

/* Defines the structure for directives */
struct directive {
    /* Enumeration of directive types */
    enum {
        external,  /* External directive */
        entry,     /* Entry directive */
        data       /* Data directive */
    } directiveType;

    /* Union of directive options */
    union {
        char label[MAX_LABEL_LENGTH];  /* Label for external or entry directives */
        struct {
            int data[MAX_SIZE_DATA];    /* Data values for data directive */
            int dataSize;               /* Number of data values */
        } data;                        /* Data directive options */
    } directiveOptions;
};

/* Defines the structure for a line of code */
struct lineStc {
    char error[MAX_ERROR_MESSAGE]; /* Buffer for error messages */
    char symbol_name[MAX_LABEL_LENGTH]; /* Buffer for symbol name */

    /* Enumeration of line types */
    enum {
        instruction, /* Line contains an instruction */
        directive,   /* Line contains a directive */
        remarks,     /* Line contains remarks */
        empty        /* Line is empty */
    } lineType;

    /* Union of line options */
    union {
        struct instruction inst; /* Instruction line options */
        struct directive dir;    /* Directive line options */
    } lineOption;
};

/* Function prototypes */

/**
 * Checks if a label is valid.
 *
 * @param name_label - The name of the label to check.
 * @param length - The length of the label.
 * @param line - A pointer to a lineStc structure to store error information.
 * @param macro_list - A pointer to the macro list.
 * @return 1 if the label is valid, 0 otherwise.
 */
int is_valid_label(char *name_label, int length, struct lineStc *line, Macro *macro_list);

/**
 * Divides a string into substrings based on spaces and commas.
 *
 * @param input - The input string to be divided.
 * @return A structure containing the substrings and their count.
 */
struct line_division string_division(char *input);

/**
 * Frees the memory allocated for the substrings in a line_division structure.
 *
 * @param result - A pointer to the line_division structure to be freed.
 */
void free_line_division(struct line_division *result); /* Make sure this matches */

/**
 * Checks if a string represents a valid register.
 *
 * @param reg - The string representing the register.
 * @return 1 if it is a valid register, 0 otherwise.
 */
int isRegister(char *reg);

/**
 * Checks if a string represents a valid immediate value.
 *
 * @param number - The string representing the immediate value.
 * @return 1 if it is a valid immediate value, 0 otherwise.
 */
int isImmidiate(char *number);

/**
 * Checks if a string represents a valid digit.
 *
 * @param number - The string to check.
 * @return 1 if it represents a digit, 0 otherwise.
 */
int isDigit(char *number);

/**
 * Checks if a string represents a valid data value.
 *
 * @param str - The string to check.
 * @return 1 if it represents valid data, 0 otherwise.
 */
int isData(char *str);

/**
 * Determines the type of an operand.
 *
 * @param operand - The string representing the operand.
 * @return An operand_type structure describing the operand.
 */
operand_type type_of_operand(char *operand);

#endif


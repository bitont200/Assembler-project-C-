#ifndef OCTAL_TRANSLATION
#define OCTAL_TRANSLATION

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tables.h"
#include "line_structure.h"

/* Defines various constants used in the program */
#define AND_FACTOR 0x07       /*AND factor for translating to octal*/
#define FIRST_NUM 12          /*Position of the first number in the translated word*/ 
#define SECOND_NUM 9          /*Position of the second number in the translated word*/ 
#define THIRD_NUM 6           /*Position of the third number in the translated word*/ 
#define FOURTH_NUM 3          /*Position of the fourth number in the translated word*/ 
#define NUM_WITHOUT_0 1000    /*Number in order to check if 0 needed before address*/ 
#define SUFFIX_OB ".ob"       /*File suffix for output ob*/
#define SUFFIX_ENT ".ent"      /*File suffix for output ent*/
#define SUFFIX_EXT ".ext"      /*File suffix for output ext*/

/**
 * create_file:
 * 
 * Create new file with name and suffix
 * @param file_name Pointer to the file name
 * @param base_name Pointer to the base name of the file
 * @param suffix suffix wanted for the file
 */
void create_file(char *file_name, char *base_name, char *suffix);

/**
 * ob_file:
 * 
 * Creates the ob file, prints the translation of the file
 * @param table Pointer to the assembler table structure
 * @param file Pointer to file
 */
void ob_file(struct assembler_table * table, char *file);

/**
 * entries_file:
 * 
 * Creates the entries file, prints the translation of the file
 * @param table Pointer to the assembler table structure
 * @param file Pointer to file
 */
void entries_file(struct assembler_table * table, char *file);

/**
 * externals_file:
 * 
 * Creates the externals file, prints the translation of the file
 * @param table Pointer to the assembler table structure
 * @param file Pointer to file
 */
void externals_file(struct assembler_table * table, char *file);

#endif
#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <stdlib.h>

#include "preprocessor.h"
#include "first_pass.h"
#include "second_pass.h"
#include "octal_translation.h"

#define MIN_NUM_ARG 2

/**
 * Main:
 * 
 * The main program of the assembler, receives files from the command line and operate them.
 * @param argc Num of arguments sent on the command line
 * @param argv Array of pointers to arguments sent to command line
 * @return EXIT_SUCCESS in any case
 */
int main(int argc, char *argv[]);

#endif /*ASSEMBLER_H*/

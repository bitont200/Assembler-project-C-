#include "second_pass.h"

int second_pass(struct assembler_table *table, FILE *file, Macro *macro) {
    int lineNum = 1, error_flag = 0;
    char line[MAX_SIZE_DATA] = {0};
    struct lineStc lineStc = {0}; /* Structure to hold line information */ 
    table->IC = 0;

    /* Goes over every line in file */
    while (fgets(line, sizeof(line), file)) {
        /* Checks structure of line */

        lineStc = get_line_structure_from_line(line, macro);

        /* Add symbols and code processing */
        if(add_symbols_code(&lineStc, table, lineNum) == ERROR) {
            error_flag++;
            continue;
        }
 
         lineNum++; /* Increment line number */ 
    }
    if (error_flag) {
        return ERROR;
    }
    return EXIT_SUCCESS;
}

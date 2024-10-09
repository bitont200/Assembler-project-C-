#include "first_pass.h"

int first_pass(struct assembler_table *table, FILE *file, Macro *macro) {
    int lineNum = 1, error_flag = 0;
    char line[MAX_LINE_LENGTH] = {0};
    struct lineStc lineStc = {0};
    
    /* Goes over every line in file */
    while (fgets(line, sizeof(line), file)){
        /* Checks structure of line */
        lineStc = get_line_structure_from_line(line, macro);

        /* Checks if line has error */
        if (has_error(&lineStc, lineNum) == ERROR){
            lineNum++;
            error_flag++; 
            continue;
        }
        
        /* In case it is a derective line: */
        if (lineStc.lineType == directive) {
            if(handle_directive(&lineStc, table, lineNum) == ERROR){
                error_flag++;
            }
        } else if (lineStc.lineType == instruction) {
            /* In case it is a instruction line: */
            if(handle_instruction(&lineStc, table, lineNum)== ERROR){
                error_flag++;
            } 
        }
        /* Empty line: */    
        lineNum++;
    }

    /* Checks for logical errors in the symbol table and handle it: */
    if(check_and_update_symbol_table(table, lineNum) == ERROR){
        error_flag++;
    }
    /* If there are errors, stop */
    if (error_flag) {
        return ERROR;
    }
    /* Update table IC and DC values */
    return EXIT_SUCCESS;
}

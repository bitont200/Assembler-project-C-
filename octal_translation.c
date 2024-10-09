#include "octal_translation.h"

/*Create new file*/
void create_file(char *file_name, char *base_name, char *suffix) {
    /*Copy the name of the file*/    
    strcpy(file_name, base_name);
    /*Adds the suffix*/
    strcat(file_name, suffix);
    /*Adds and of string*/
    *(file_name + strlen(base_name) + strlen(suffix)) = EOS;
}

/*Create ob file*/
void ob_file(struct assembler_table * table, char *file) {
    int i;
    char file_name[LENGTH_FILE_NAME];
    FILE * newFile;
    char octal[] = {'0', '1', '2', '3', '4', '5', '6', '7'};

    create_file(file_name, file, SUFFIX_OB);

    newFile = fopen(file_name, "w");

    /*Prints the octal translation of ob file*/
    if (newFile) {
        fprintf(newFile, "  %d %d\n", table->IC, table->DC);
        for (i = 0; i < table->IC; i++) {
            if (i+START_POINT<NUM_WITHOUT_0) {
                fprintf(newFile, "0");
            }
            fprintf(newFile, "%d %c%c%c%c%c\n",
                    i+START_POINT,
                    octal[(table->code_lines[i] >> FIRST_NUM) & AND_FACTOR],
                    octal[(table->code_lines[i] >> SECOND_NUM) & AND_FACTOR],
                    octal[(table->code_lines[i] >> THIRD_NUM) & AND_FACTOR],
                    octal[(table->code_lines[i] >> FOURTH_NUM) & AND_FACTOR],
                    octal[(table->code_lines[i]) & AND_FACTOR]);
        }
        for (i = 0; i < table->DC; i++) {
            if (i+START_POINT<NUM_WITHOUT_0) {
                fprintf(newFile, "0");
            }
            fprintf(newFile, "%d %c%c%c%c%c\n",
                    table->IC + i + START_POINT,
                    octal[(table->data_lines[i] >> FIRST_NUM) & AND_FACTOR],
                    octal[(table->data_lines[i] >> SECOND_NUM) & AND_FACTOR],
                    octal[(table->data_lines[i] >> THIRD_NUM) & AND_FACTOR],
                    octal[(table->data_lines[i] >> FOURTH_NUM) & AND_FACTOR],
                    octal[(table->data_lines[i]) & AND_FACTOR]);
        }
        fclose(newFile);
    }
}

/*Create entries file*/
void entries_file(struct assembler_table * table, char *file) {
    if (table->entries_count == 0) {
        return;
    } else {
        char file_name[LENGTH_FILE_NAME];
        FILE * newFile;

        create_file(file_name, file, SUFFIX_ENT);

        newFile = fopen(file_name, "w");

        if (newFile) {
            struct node *current = table->symbol_table.head;
            while (current != NULL) {
                struct symbol *current_symbol = (struct symbol *)current->data;
                if (current_symbol->symbolType == symbol_entryCommand || current_symbol->symbolType == symbol_entryData) {
                    if (current_symbol->address + START_POINT < NUM_WITHOUT_0) {
                        fprintf(newFile, "%s 0%d\n", current_symbol->symbolName, current_symbol->address+START_POINT);
                    } else {
                        fprintf(newFile, "%s %d\n", current_symbol->symbolName, current_symbol->address+START_POINT);
                    }
                }
                current = current->next;
            }
            fclose(newFile);
        }
    }  
}

/*Create externals file*/
void externals_file(struct assembler_table * table, char *file) {
    if (table->extern_count == 0) {
        return;
    } else {
        char file_name[LENGTH_FILE_NAME];
        FILE * newFile;

        create_file(file_name, file, SUFFIX_EXT);

        newFile = fopen(file_name, "w"); 

        if (newFile) {
            struct node *current = table->symbol_table.head;
            while (current != NULL) {
                struct symbol *current_symbol = (struct symbol *)current->data;
                if (current_symbol->symbolType == symbol_external) {
                    struct node *currentLine = current_symbol->extern_addresses.head;
                    while (currentLine != NULL) {
                    int *line = currentLine->data;
                    if (*line + START_POINT < NUM_WITHOUT_0) {
                        fprintf(newFile, "%s 0%d\n", current_symbol->symbolName, *line+START_POINT);
                    } else {
                        fprintf(newFile, "%s %d\n", current_symbol->symbolName, *line+START_POINT);
                    }  
                    currentLine = currentLine->next;
                }
                }
                current = current->next;
            }
            fclose(newFile);
        }
    }  
}

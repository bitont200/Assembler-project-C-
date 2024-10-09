/*@by Shira Kedmi*/

#include "preprocessor.h"

int identify_line_type(char *line_parsing, Macro **macro_pointer, Macro **macro_table, int line_number) {
    char *line_ptr;                      /* Pointer to traverse the line of text */
    Macro *macro;                       /* Pointer to a Macro structure */
    char *names_of_instructions[NUM_OF_INSTRUCTIONS] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};
    /* Array of reserved instruction names */
    char *names_of_directives[NUM_OF_DIRECTIVES] = {".data", ".string", ".entry", ".extern"};
    /* Array of reserved directive names */
    int i;
    int j = 0;
    char name_of_macro[LENGTH_MACRO_NAME] = {0};   /* Buffer to store macro name */

    line_ptr = line_parsing;

    /* Skip leading whitespace */
    while (isspace(*line_ptr)) {
        line_ptr++;
    }

    /* Identify start of macro definition */
    if (strncmp(line_ptr, "macr", 4) == 0 && isspace(line_ptr[4])) {
        line_ptr += 4;
        while (isspace(*line_ptr)) {
            line_ptr++;
        }

        while (*line_ptr && !isspace(*line_ptr)) {
            name_of_macro[j++] = *line_ptr++;
        }
        name_of_macro[j] = EOS;
        if (!isalpha(name_of_macro[0])) {
            fprintf(stderr, "Error on line %d: The first character of the macro name '%s' is not a letter.\n", line_number, name_of_macro);
            return ERROR_FOUND;
        }

        /* Check if macro name is a reserved instruction or directive name */
        for (i = 0; i < NUM_OF_INSTRUCTIONS; i++) {
            if (strncmp(name_of_macro, names_of_instructions[i], strlen(names_of_instructions[i])) == 0)  {
                fprintf(stderr, "Error on line %d: The macro name '%s' is a reserved instruction name in the assembly program.\n", line_number, name_of_macro);
                return ERROR_FOUND;
            }
        }

        for (i = 0; i < NUM_OF_DIRECTIVES; i++) {
            if (strncmp(name_of_macro, names_of_directives[i], strlen(names_of_directives[i])) == 0)  {
                fprintf(stderr, "Error on line %d: The macro name '%s' is a reserved directive name in the assembly program.\n", line_number, name_of_macro);
                return ERROR_FOUND;
            }
        }

        while (*line_ptr != EOS) {
            if (!isspace(*line_ptr)) {
                fprintf(stderr, "Error on line %d: There are invalid characters in the macro definition line after the macro name.\n", line_number);
                return ERROR_FOUND;
            }
            line_ptr++;
        }

        if (find_macro(*macro_table, name_of_macro) != NULL) {
            fprintf(stderr, "Error on line %d: A macro with this name '%s' has already been defined.\n", line_number, name_of_macro);
            return ERROR_FOUND;
        }

        /* Allocate memory for new macro */
        macro = malloc(sizeof(Macro));
        if (macro == NULL) {
            fprintf(stderr, "Error allocating memory for macro");
            free_macro_table(&macro);
            exit(EXIT_FAILURE);
        }
        macro->macro_content = NULL;       /* Initialize macro content pointer */
        macro->num_macro_lines = 0;        /* Initialize number of macro lines */
        macro->next = NULL;                /* Initialize next macro pointer */

        strcpy(macro->macro_name, name_of_macro); /* Set macro name */

        /* Add macro to macro table */
        if (*macro_table == NULL) {
            *macro_table = macro;
        } else {
            Macro *current = *macro_table;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = macro;
        }

        *macro_pointer = macro;
        return START_MACRO_DEFINITION;
    }

    /* Identify end of macro definition */
    if (strncmp(line_ptr, "endmacr", 7) == 0 && isspace(line_ptr[7])) {
        line_ptr += 7;
        while (*line_ptr != EOS) {
            if (!isspace(*line_ptr)) {
                fprintf(stderr,"Error on line %d: There are invalid characters after the macro end setting.\n", line_number);
                return ERROR_FOUND;
            }
            line_ptr++;
        }
        return END_MACRO_DEFINITION;
    }

    /* Identify macro call */
    while (*line_ptr && !isspace(*line_ptr)) {
        name_of_macro[j++] = *line_ptr++;
    }
    name_of_macro[j] = EOS;

    macro = find_macro(*macro_table, name_of_macro);
    if (macro != NULL) {
        *macro_pointer = macro;
        return CALL_MACRO;
    } else {
        return SOME_LINE_CODE;
    }
}

int preprocessor(char *file_name, Macro **macro_table) {
    char analysis_line[MAX_LINE_LENGTH] = {0};  /* Buffer to read lines from input file */
    Macro *macro_pointer = *macro_table; /* Pointer to the current macro being processed */
    char am_file_name[LENGTH_FILE_NAME]; /* File name for output */
    char as_file_name[LENGTH_FILE_NAME]; /* File name for input */
    FILE *am_file;                       /* Pointer to the output file */
    FILE *as_file;                       /* Pointer to the input file */
    int line_number = 1;                /* Line number in the input file */
    int case_type;                      /* Type of line as determined by identify_line_type */

    /* Construct file names */
    strcpy(as_file_name, file_name);
    strcat(as_file_name, SUFFIX_AS);

    as_file = fopen(as_file_name, "r");
    if (as_file == NULL) {
        fprintf(stderr,"Error opening .as file\n");
        exit(EXIT_FAILURE);
    }

    strcpy(am_file_name, file_name);
    strcat(am_file_name, SUFFIX_AM);

    am_file = fopen(am_file_name, "w");
    if (am_file == NULL) {
        fprintf(stderr,"Error opening .am file\n");
        fclose(as_file);
        exit(EXIT_FAILURE);
    }

    /* Read lines from .as file and process */
    while (fgets(analysis_line, MAX_LINE_LENGTH, as_file) != NULL) {
        case_type = identify_line_type(analysis_line, &macro_pointer, macro_table, line_number);

        if (case_type == ERROR_FOUND) {
            fclose(as_file);
            fclose(am_file);
            return ERROR_FOUND;
        }

        switch (case_type) {
            case START_MACRO_DEFINITION:
                /* No action required for start of macro definition */
                break;

            case END_MACRO_DEFINITION:
                macro_pointer = NULL;  /* Reset macro pointer after macro definition ends */
                break;

            case CALL_MACRO:
            {
                int i;
                for (i = 0; i < macro_pointer->num_macro_lines; i++) {
                    fputs(macro_pointer->macro_content[i], am_file); /* Write macro content to output file */
                }
                macro_pointer = NULL;  /* Reset macro pointer after macro call */
            }
                break;

            case SOME_LINE_CODE:
                if (macro_pointer != NULL) {
                    add_line_to_macro(macro_pointer, analysis_line); /* Add line to current macro if inside a macro definition */
                } else {
                    fputs(analysis_line, am_file); /* Write line to output file if not inside a macro */
                }
                break;
        }

        line_number++;  /* Increment line number for the next line */
    }

    /* Clean up */
    fclose(as_file);
    fclose(am_file);

    return EXIT_SUCCESS;
}


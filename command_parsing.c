/*@by Shira Kedmi*/
#include "command_parsing.h"

struct line_division string_division(char *input) {
    struct line_division result; /* Result structure containing substrings and their count */
    int i;
    char buffer[MAX_SIZE_DATA + 1]; /* Temporary buffer for building substrings */
    int buffer_index = 0;

    result.strings_count = 0;

    for (i = 0; i < MAX_SIZE_DATA &&  input[i] != EOS; i++) {
        if (input[i] == ',' || input[i] == ' ') {
            if (buffer_index > 0) {
                buffer[buffer_index] = EOS; /* Null-terminate the substring */
                result.substrings[result.strings_count] = malloc(buffer_index + 1);
                strcpy(result.substrings[result.strings_count], buffer);
                result.strings_count++;
                buffer_index = 0;
            }
            if (input[i] == ',') {
                /* Handle commas as separate substrings */
                result.substrings[result.strings_count] = malloc(2);
                result.substrings[result.strings_count][0] = ',';
                result.substrings[result.strings_count][1] = EOS;
                result.strings_count++;
            }
        } else {
            /* Copy characters to the buffer */
            buffer[buffer_index++] = input[i];
        }
    }

    /* Handle the last substring */
    if (buffer_index > 0) {
        buffer[buffer_index] = EOS; /* Null-terminate the substring */
        result.substrings[result.strings_count] = malloc(buffer_index + 1);
        strcpy(result.substrings[result.strings_count], buffer);
        result.strings_count++;
    }

    return result;
}

void free_line_division(struct line_division *result) {
    int i;
    for (i = 0; i < result->strings_count; i++) {
        free(result->substrings[i]); /* Free each allocated substring */
    }
}

int is_valid_label(char *name_label, int length, struct lineStc *line, Macro *macro_list){
    int i;
    char* label = name_label;
    Macro * macro = macro_list;
    struct lineStc *line_stc = line;
    int label_length = length - 1; /* Length of the label without the colon */
    char *invaild_label_name[] =
            {".data", ".string", ".entry", ".extern", "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
             "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "red",
             "prn", "jsr", "rts", "stop","macr","endmacr"};

    /* Check if the label length is within the allowed limit */
    if (label_length > MAX_LABEL_LENGTH) {
        strncpy(line_stc->error, "The length of the label is greater than 31", MAX_ERROR_MESSAGE);
        return ERROR;
    }

    /* Check if the label starts with an alphabetic letter */
    if (!isalpha(label[0])) {
        strncpy(line_stc->error, "The label name does not start with an alphabetic letter", MAX_ERROR_MESSAGE);
        return ERROR;
    }

    /* Check if the label contains only alphabetic letters or digits */
    for (i = 0; i <= label_length; i++) {
        if (!isalpha(label[i]) && !isdigit(label[i])) {
            strncpy(line_stc->error, "The label name is invalid, contains characters other than alphabetic letters or numbers", MAX_ERROR_MESSAGE);
            return ERROR;
        }
    }

    /* Check if the label name is in the list of invalid label names */
    for (i = 0; i < INVALID_LABEL_NAME_COUNT; i++) {
        if (strcmp(label, invaild_label_name[i]) == 0) {
            strncpy(line_stc->error, "The name of the label as a word name is stored in the assembler", MAX_ERROR_MESSAGE);
            return ERROR;
        }
    }

    /* Check if the label name is the same as a macro name */
    if (find_macro(macro, label) != NULL) {
        strncpy(line_stc->error, "The label name is the same as a macro name", MAX_ERROR_MESSAGE);
        return ERROR;
    }
    return 0; /* Label is valid */
}

int isRegister(char *reg) {
    char *registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
    int i;

    for(i = 0; i < NUM_OF_REGISTERS; i++) {
        if(strcmp(reg, registers[i]) == 0) {
            return i; /* Return register index */
        }
    }
    return ERROR; /* Register not found */
}

int isImmidiate(char *number) {
    char *pointer;
    long num;
    /* Skip the '+' sign if present */
    if (number[0] == '+') {
        number++;
    }

    /* Check if the string is not empty */
    if (*number == EOS) {
        return ERROR_IMM;
    }

    /* Use strtol to convert string to long */
    num = strtol(number, &pointer, DECIMAL_NUM);

    /* Check if pointer is not null and points to the end of the string */
    if (*pointer != EOS || number == pointer) {
        return ERROR_IMM; /* Not a valid number */
    }

    /* Check if the number is in the range -2048 to 2047 */
    if (num >= MIN_NUM_IMM && num <= MAX_NUM_IMM) {
        return num;
    }

    return ERROR_IMM; /* Number out of range */
}

int isDigit(char *number) {
    char *pointer;
    long num;
    /* Skip the '+' sign if present */
    if (number[0] == '+') {
        number++;
    }

    /* Check if the string is not empty */
    if (*number == EOS) {
        return ERROR_DATA;
    }

    /* Use strtol to convert string to long */
    num = strtol(number, &pointer, DECIMAL_NUM);

    /* Check if pointer is not null and points to the end of the string */
    if (*pointer != EOS || number == pointer) {
        return ERROR_DATA; /* Not a valid number */
    }

    /* Check if the number is in the range -2048 to 2047 */
    if (num >= MIN_NUM_DATA && num <= MAX_NUM_DATA) {
        return num;
    }

    return ERROR_DATA; /* Number out of range */
}

int isData(char *str) {
    int count = 0;
    char *number = 0;

    if (str == NULL || *str == EOS) {
        return ERROR;
    }
    number = strtok(str, ",");

    while (number != NULL) {
        if (isDigit(number) == ERROR_DATA) {
            return ERROR;
        }
        count++;
        number = strtok(NULL, ",");
    }
    return count;
}

operand_type type_of_operand(char *operand) {
    int helper;
    operand_type operandType;
    operandType.type = ERROR;
    operandType.value = 0;

    helper = isRegister(operand);
    if(helper != ERROR) {
        operandType.type = ADDRESSING_METHOD_3; /* Register addressing */
        operandType.value = helper;
    } else if (operand[0] == '*') {
        helper = isRegister(&operand[1]);
        if(helper != ERROR) {
            operandType.type = ADDRESSING_METHOD_2; /* Indirect register addressing */
            operandType.value = helper;
        }
    } else if (operand[0] == '#') {
        helper = isImmidiate(&operand[1]);
        if(helper != ERROR_IMM) {
            operandType.type = ADDRESSING_METHOD_0; /* Immediate addressing */
            operandType.value = helper;
        }
    } else {
        operandType.type = ADDRESSING_METHOD_1; /* Direct addressing */
    }
    return operandType; /* Return operand type */
}


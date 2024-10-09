/*@by Shira Kedmi*/

#include "line_structure.h"

struct lineStc get_line_structure_from_line(char* string_line, Macro *mac) {
    struct lineStc line_stc;            /*The line structure*/
    struct line_division strings;      /*A structure to hold the row substrings*/
    int i, j, len, index, length_line, result, dataIndex, string_count = 0, in_quotes = 0, found = 0;
    char *current, *temp_ptr, *data_pos, *line, *string_pos;
    char temp[MAX_SIZE_DATA + 1], result_data[MAX_SIZE_DATA + 1], result_string[MAX_SIZE_DATA + 1] = {0};
    operand_type operand;
    Macro *macro;

    /*The structure of the commands, the operands, and the legal reference methods*/
    struct legal_instruction inst[NUM_OF_INSTRUCTION]={
            {"mov",  0,  "0123", "123"}, {"cmp",  1,  "0123", "0123"},
            {"add",  2,  "0123", "123"}, {"sub",  3,  "0123", "123"},
            {"lea",  4,  "1",    "123"}, {"clr",  5,   {0},    "123"},
            {"not",  6,  {0},    "123"}, {"inc",  7,   {0},    "123"},
            {"dec",  8,   {0},    "123"}, {"jmp",  9,   {0},    "12"},
            {"bne",  10,  {0},    "12"}, {"red",  11,  {0},    "123"},
            {"prn",  12,  {0},    "0123"}, {"jsr",  13,  {0},    "12"},
            {"rts",  14,  {0},     {0}}, {"stop", 15,  {0},     {0}}
    };
    char buffer_line[MAX_SIZE_DATA + 1] = {0};
    index = 0;
    length_line = 0;
    result = 0;
    line = string_line;
    macro = mac;
    memset(&line_stc, 0, sizeof(line_stc));
    memset(&strings, 0, sizeof(strings));

    while (line[length_line] != LINE_DROP) {
        length_line++;
    }

    /*Checking if the line length is correct*/
    if (length_line > MAX_SIZE_DATA) {
        strncpy(line_stc.error, "The line length is greater than 80 characters", MAX_ERROR_MESSAGE);
        free_line_division(&strings);
        return line_stc;
    }
    strncpy(buffer_line, line, length_line);
    buffer_line[length_line + 1] = LINE_DROP;
    buffer_line[length_line + 2] = EOS;

    strings = string_division(buffer_line);      /*Splitting the line into substrings*/

    if (strings.strings_count == 0) {                /*Checking whether the row is empty*/
        line_stc.lineType = empty;
        free_line_division(&strings);
        return line_stc;
    }

    if (strings.substrings[0][0] == ';') {             /*Checking whether the line is a comment*/
        line_stc.lineType = remarks;
        free_line_division(&strings);
        return line_stc;
    }

    /*Checking whether there are unnecessary commas*/
    for (j = 0; j < strings.strings_count - 1 && (strcmp(strings.substrings[j], ".string") != 0); j++) {
        if (strcmp(strings.substrings[j], ",") == 0 && strcmp(strings.substrings[j + 1], ",") == 0) {
            strncpy(line_stc.error, "Invalid operand, there is an unnecessary comma", MAX_ERROR_MESSAGE);
            free_line_division(&strings);
            return line_stc;
            
        }
    }

    /*Checking whether there is a label*/
    len = strlen(strings.substrings[index]);
    if (len > 0 && strings.substrings[index][len - 1] == ':') {
        if (strings.strings_count != 1) {
            /*Checking if the label name is valid*/
            if (is_valid_label(strings.substrings[index], len-1, &line_stc, macro) == ERROR) {
                free_line_division(&strings);
                return line_stc;
            } else {
                /*Copying the label name to the structure*/
                strncpy(line_stc.symbol_name, strings.substrings[index], len - 1);
                line_stc.symbol_name[len] = EOS;
                strings.strings_count--;
                index++;
            }
        } else {
            strncpy(line_stc.error, "Invalid line, missing code after label", MAX_ERROR_MESSAGE);
            free_line_division(&strings);
            return line_stc;
        }

    }

    /*Checking whether it is .extern or .entry*/
    if (strcmp(strings.substrings[index], ".extern") == 0 || strcmp(strings.substrings[index], ".entry") == 0) {
        line_stc.lineType = directive;
        line_stc.symbol_name[0] = EOS;
        line_stc.lineOption.dir.directiveType = (strcmp(strings.substrings[index], ".extern") == 0) ? external : entry;
        index++;
        len = strlen(strings.substrings[index]);
        if (is_valid_label(strings.substrings[index], len, &line_stc, macro) == ERROR) {
            free_line_division(&strings);
            return line_stc;
        } else {
            strcpy(line_stc.lineOption.dir.directiveOptions.label, strings.substrings[index]);
            line_stc.lineOption.dir.directiveOptions.label[len] = EOS;
            index++;
        }
        if (strings.strings_count > 2) {
            strncpy(line_stc.error, "There are invalid characters at the end of the command", MAX_ERROR_MESSAGE);
            free_line_division(&strings);
            return line_stc;
        }
        free_line_division(&strings);
        return line_stc;
    }

    if (strcmp(strings.substrings[index], ".data") == 0) {
        index++;
        /*.data search*/
        data_pos = strstr(buffer_line, ".data");

        if (data_pos == NULL) {
            result_data[0] = EOS;

        }


        data_pos += strlen(".data");
        /*Removing all white characters*/
        while (*data_pos != EOS && isspace((unsigned char)*data_pos)) {
            data_pos++;
        }


        temp_ptr = temp;
        /*Dividing the 'data' array by commas so that it is possible to work on it and check if the values
            entered into '.data' are valid*/
        while (*data_pos != EOS) {
            if (!isspace((unsigned char)*data_pos)) {
                *temp_ptr++ = *data_pos;
            }
            data_pos++;
        }
        *temp_ptr = EOS;
        /*Copying the array*/
        strcpy(result_data, temp);
        /*Checking whether the value entered in '.data' is valid*/
        result = isData(result_data);
        if (result == ERROR) {
            strncpy(line_stc.error, "You entered the information in '.data' incorrectly", MAX_ERROR_MESSAGE);
            free_line_division(&strings);
            return line_stc;
        }

        dataIndex = 0;

        while ((line_stc.symbol_name[0] != EOS && index <= strings.strings_count) || (line_stc.symbol_name[0] == EOS && index < strings.strings_count)) {
            current = strings.substrings[index];

            if (strcmp(current, ",") != 0) {
                line_stc.lineOption.dir.directiveOptions.data.data[dataIndex] = strtol(current, &current, 10);
                dataIndex++;
            }
            index++;
        }

        line_stc.lineOption.dir.directiveOptions.data.dataSize = result;
        line_stc.lineType = directive;
        line_stc.lineOption.dir.directiveType = data;
        index++;
        if (strings.strings_count > (result+result)) {
            strncpy(line_stc.error, "There are invalid characters at the end of the command", MAX_ERROR_MESSAGE);
            free_line_division(&strings);
            return line_stc;
        }
        free_line_division(&strings);
        return line_stc;
    }

    if (strcmp(strings.substrings[index], ".string") == 0) {
        index++;

        /*Searching for the position of .string in a string*/
        string_pos = strstr(buffer_line, ".string");
        if (string_pos == NULL) {
            strncpy(line_stc.error, "'.string' not found in the line", MAX_ERROR_MESSAGE);
            free_line_division(&strings);
            return line_stc;
        }

        /*Skipping the position of .string*/
        string_pos += strlen(".string");

        /*Skip white spaces after .string*/
        while (isspace((unsigned char)*string_pos)) {
            string_pos++;
        }

        /*Checking whether the string of .string starts with quotes*/
        if (*string_pos != '"') {
            strncpy(line_stc.error, "The '.string' string is not in a valid format, it does not start with quotes", MAX_ERROR_MESSAGE);
            free_line_division(&strings);
            return line_stc;
        }

        /*Starting to process the string*/
        j = 0;
        result_string[j++] = *string_pos++;
        in_quotes = 1;  /*Setting a flag to indicate that we are inside the quotes*/
        while (*string_pos != EOS) {
            if (*string_pos == '"') {
                if (in_quotes) {

                    result_string[j++] = *string_pos++;
                    result_string[j] = EOS;  /*Add a string termination character*/
                    /*Checking that there are no extra characters after the command*/
                    while (isspace((unsigned char)*string_pos)) {
                        string_pos++;
                    }
                    if (*string_pos != EOS) {
                        strncpy(line_stc.error, "Unexpected characters after the closing quote", MAX_ERROR_MESSAGE);
                        free_line_division(&strings);
                        return line_stc;
                    } 

                    result_string[j++] = *string_pos++;
                    in_quotes = !in_quotes;
                }
            } else {
                result_string[j++] = *string_pos++;
            }
        }

        /*If we finished the loop without finding closing quotes*/
        if (in_quotes) {
            strncpy(line_stc.error, "Missing closing quote for the string", MAX_ERROR_MESSAGE);
            free_line_division(&strings);
            return line_stc;
        }
        i = 1;
        while (result_string[i] != EOS) {
            line_stc.lineOption.dir.directiveOptions.data.data[string_count++] = (int)result_string[i];
            i++;
        }

        line_stc.lineOption.dir.directiveOptions.data.data[string_count-1] = EOS;

        line_stc.lineOption.dir.directiveOptions.data.dataSize = string_count;
        line_stc.lineType = directive;
        line_stc.lineOption.dir.directiveType = data;
        index++;

        if (result_string[string_count] != '"') {
            strncpy(line_stc.error, "There are invalid characters at the end of the command", MAX_ERROR_MESSAGE);
            free_line_division(&strings);
            return line_stc;
        }
        free_line_division(&strings);
        return line_stc;
    }

    /*Checking if there is a command*/
    for (i = 0; i < NUM_OF_INSTRUCTION; i++) {
        if (strcmp(strings.substrings[index], inst[i].name_inst) == 0) {
            line_stc.lineType = instruction;
            line_stc.lineOption.inst.instructionType = inst[i].opcode_inst;
            index++;
            /*Checking whether the command has no operands at all*/
            if (strcmp(inst[i].source_operand, "") == 0 && strcmp(inst[i].dest_operand,"")==0) {
                if (strings.strings_count > 1) {
                    strncpy(line_stc.error, "There are invalid characters at the end of the command", MAX_ERROR_MESSAGE);
                    free_line_division(&strings);
                    return line_stc;
                }
                line_stc.lineOption.inst.operands[0].operandType = none;
                line_stc.lineOption.inst.operands[1].operandType = none;
                free_line_division(&strings);
                return line_stc;
            }

            if (strcmp(strings.substrings[index],",") == 0) {
                strncpy(line_stc.error, "Invalid command, there is an unnecessary comma", MAX_ERROR_MESSAGE);
                free_line_division(&strings);
                return line_stc;
            }
            /*Checking whether the command has only one operand*/
            if (strcmp(inst[i].source_operand, "") == 0 && strcmp(inst[i].dest_operand, "") != 0) {
                if (strings.strings_count < 2) {
                    strncpy(line_stc.error, "Invalid command, missing operand", MAX_ERROR_MESSAGE);
                    free_line_division(&strings);
                    return line_stc;
                }
                /*Checking whether the received operand is valid and defined*/
                operand = type_of_operand(strings.substrings[index]);
                if (operand.type == ERROR) {
                    strncpy(line_stc.error, "Invalid command, invalid operand entered", MAX_ERROR_MESSAGE);
                    free_line_division(&strings);
                    return line_stc;
                }
                /*Checking whether the addressing method of the operand is legal for this command,
                    * the test is carried out in the legal command structure defined*/
                if (strchr(inst[i].dest_operand, operand.type + '0') != NULL) {
                    found = 1;
                }

                if (found != 1) {
                    strncpy(line_stc.error, "The addressing method of the destination operand is not suitable for this command", MAX_ERROR_MESSAGE);
                    free_line_division(&strings);
                    return line_stc;
                }

                line_stc.lineOption.inst.operands[0].operandType = none;
                /*Checking what type of operand address and to me it is an update in the structure*/
                switch (operand.type) {
                    case ADDRESSING_METHOD_0:
                        line_stc.lineOption.inst.operands[1].operandType = immidiate;
                        line_stc.lineOption.inst.operands[1].operandOption.immidiate = operand.value;
                        break;
                    case ADDRESSING_METHOD_1:
                        line_stc.lineOption.inst.operands[1].operandType = label;
                        strcpy(line_stc.lineOption.inst.operands[1].operandOption.label, strings.substrings[index]);
                        break;
                    case ADDRESSING_METHOD_2:
                        line_stc.lineOption.inst.operands[1].operandType = indirectReg;
                        line_stc.lineOption.inst.operands[1].operandOption.indirectReg = operand.value;
                        break;
                    case ADDRESSING_METHOD_3:
                        line_stc.lineOption.inst.operands[1].operandType = directReg;
                        line_stc.lineOption.inst.operands[1].operandOption.directReg = operand.value;
                        break;
                }
                index++;

                if (strings.strings_count > 2) {
                    strncpy(line_stc.error, "There are invalid characters at the end of the command", MAX_ERROR_MESSAGE);
                    free_line_division(&strings);
                    return line_stc;
                }
                free_line_division(&strings);
                return line_stc;
            }
            /*Checking whether the command has 2 operands*/
            if (inst[i].source_operand != NULL && inst[i].dest_operand != NULL) {
                if (strings.strings_count < 3) {
                    strncpy(line_stc.error, "Invalid command, missing operand", MAX_ERROR_MESSAGE);
                    free_line_division(&strings);
                    return line_stc;
                }
                /*Checking whether the operand type is defined and legal*/
                operand = type_of_operand(strings.substrings[index]);

                if (operand.type == ERROR) {
                    strncpy(line_stc.error, "Invalid command, invalid operand entered", MAX_ERROR_MESSAGE);
                    free_line_division(&strings);
                    return line_stc;
                }
                /*Checking whether the addressing method of the operand is legal for this command,
                    * the test is carried out in the legal command structure defined*/

                if (strchr(inst[i].source_operand, operand.type + '0') != NULL) {
                    found = 1;
                }


                if (found != 1) {
                    strncpy(line_stc.error, "The addressing method of the source operand is not suitable for this command", MAX_ERROR_MESSAGE);
                    free_line_division(&strings);
                    return line_stc;
                }
                /*Checking what is an operand addressing method and updating the structure*/

                switch (operand.type) {
                    case ADDRESSING_METHOD_0:
                        line_stc.lineOption.inst.operands[0].operandType = immidiate;
                        line_stc.lineOption.inst.operands[0].operandOption.immidiate = operand.value;
                        break;
                    case ADDRESSING_METHOD_1:
                        line_stc.lineOption.inst.operands[0].operandType = label;
                        strcpy(line_stc.lineOption.inst.operands[0].operandOption.label, strings.substrings[index]);
                        break;
                    case ADDRESSING_METHOD_2:
                        line_stc.lineOption.inst.operands[0].operandType = indirectReg;
                        line_stc.lineOption.inst.operands[0].operandOption.indirectReg = operand.value;
                        break;
                    case ADDRESSING_METHOD_3:
                        line_stc.lineOption.inst.operands[0].operandType = directReg;
                        line_stc.lineOption.inst.operands[0].operandOption.directReg = operand.value;
                        break;
                }
                index++;
                if (strcmp(strings.substrings[index], ",") != 0) {
                    strncpy(line_stc.error, "Invalid command, missing comma between operands", MAX_ERROR_MESSAGE);
                    free_line_division(&strings);
                    return line_stc;
                }
                index++;

                found = 0;
                /*Checking whether the operand type is valid and defined*/
                operand = type_of_operand(strings.substrings[index]);
                if (operand.type == ERROR) {
                    strncpy(line_stc.error, "Invalid command, invalid operand entered", MAX_ERROR_MESSAGE);
                    free_line_division(&strings);
                    return line_stc;
                }
                /*Checking whether the addressing method of the operand is legal for this command,
                    * the test is carried out in the legal command structure defined*/

                if (strchr(inst[i].dest_operand, operand.type + '0') != NULL) {
                    found = 1;
                }

                if (found != 1) {
                    strncpy(line_stc.error, "The addressing method of the destination operand is not suitable for this command", MAX_ERROR_MESSAGE);
                    free_line_division(&strings);
                    return line_stc;
                }
                /*Checking what is an operand addressing method and updating the structure*/
                switch (operand.type) {
                    case ADDRESSING_METHOD_0:
                        line_stc.lineOption.inst.operands[1].operandType = immidiate;
                        line_stc.lineOption.inst.operands[1].operandOption.immidiate = operand.value;
                        break;
                    case ADDRESSING_METHOD_1:
                        line_stc.lineOption.inst.operands[1].operandType = label;
                        strcpy(line_stc.lineOption.inst.operands[1].operandOption.label, strings.substrings[index]);
                        break;
                    case ADDRESSING_METHOD_2:
                        line_stc.lineOption.inst.operands[1].operandType = indirectReg;
                        line_stc.lineOption.inst.operands[1].operandOption.indirectReg = operand.value;
                        break;
                    case ADDRESSING_METHOD_3:
                        line_stc.lineOption.inst.operands[1].operandType = directReg;
                        line_stc.lineOption.inst.operands[1].operandOption.directReg = operand.value;
                        break;
                }
                index++;

                if (strings.strings_count > 4) {
                    strncpy(line_stc.error, "There are invalid characters at the end of the command", MAX_ERROR_MESSAGE);
                    free_line_division(&strings);
                    return line_stc;
                }
                free_line_division(&strings);
                return line_stc;
            }
        }
    }
    /*Otherwise, if no command/directive, the instruction name is invalid in assembly language*/

    strncpy(line_stc.error, "The command name is not valid in assembly language", MAX_ERROR_MESSAGE);
    free_line_division(&strings);
    return line_stc;
}



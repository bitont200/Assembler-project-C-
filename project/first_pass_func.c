/*@by Tamar Biton*/

#include "first_pass_func.h"

/*Checks if line has any errors*/
int has_error(struct lineStc *lineStc, int lineNum) {
    if (strlen(lineStc->error) > 0){
        fprintf(stderr,"Error on line %d: %s.\n", lineNum, lineStc->error);
        return ERROR;
    }
    return EXIT_SUCCESS;
}

/*Checks if symbol is entry*/
int handle_if_entry(struct lineStc *lineStc, struct assembler_table *table, int lineNum) {
    /*Search for symbol*/
    struct symbol * currentSyb = symbol_table_search_symbol(&table->symbol_table,lineStc->symbol_name);

    if(currentSyb) { /*checks if symbol is already in table*/
        if(currentSyb->symbolType==symbol_entry) {
            if(lineStc->lineType == directive) {
                currentSyb->symbolType = symbol_entryData;
                currentSyb->address = table->DC;
            } else if (lineStc->lineType == instruction) {
                currentSyb->symbolType = symbol_entryCommand;
                currentSyb->address = table->IC;
            }
        } else {
            fprintf(stderr,"Error on line %d: redefinition of symbol %s.\n", lineNum, currentSyb->symbolName);
            return ERROR;
        }
    } else { /*Not in table - add to table*/
        if(lineStc->lineType == directive) {
            currentSyb = make_new_symbol(lineStc->symbol_name, symbol_data, lineNum, table->DC);
        } else if (lineStc->lineType == instruction) {
            currentSyb = make_new_symbol(lineStc->symbol_name, symbol_command, lineNum, table->IC);
        }
        symbol_table_add_symbol(&table->symbol_table, currentSyb);
    }
    return EXIT_SUCCESS;
}

/*Checks if symbol is entry or extern*/
int handle_if_extern_or_entry(struct lineStc *lineStc, struct assembler_table *table, int lineNum) {
    /*Search for symbol*/
    struct symbol * currentSyb = symbol_table_search_symbol(&table->symbol_table, lineStc->lineOption.dir.directiveOptions.label);
    if(currentSyb) { /*checks if symbol is already in table*/
        if (currentSyb->symbolType == symbol_entry) {
            if (currentSyb->symbolType == symbol_command){
                currentSyb->symbolType = symbol_entryCommand;
            } else if (currentSyb->symbolType == symbol_data){
                currentSyb->symbolType = symbol_entryData;
            } else {
                fprintf(stderr,"Error on line %d: redefinition of symbol %s.\n", lineNum, currentSyb->symbolName);
                return ERROR;
            }
        } else if (currentSyb->symbolType == symbol_command) {
            table->entries_count++;
            currentSyb->symbolType = symbol_entryCommand;
        } else if (currentSyb->symbolType == symbol_data) {
            table->entries_count++;
            currentSyb->symbolType = symbol_entryData;
        } else {
            fprintf(stderr,"Error on line %d: redefinition of symbol %s.\n", lineNum, currentSyb->symbolName);
            return ERROR;
        }
    } else if (!currentSyb) { /*Not in table - add to table*/
        currentSyb = make_new_symbol(lineStc->lineOption.dir.directiveOptions.label, lineStc->lineOption.dir.directiveType, lineNum, 0);
        if(lineStc->lineOption.dir.directiveType == external) {
            table->extern_count++;
            currentSyb->extern_addresses.head = NULL;
        } else if (lineStc->lineOption.dir.directiveType == entry){
            table->entries_count++;
        }
        symbol_table_add_symbol(&table->symbol_table, currentSyb);
    } else {
        fprintf(stderr,"Error on line %d: redefinition of symbol %s.\n", lineNum, lineStc->symbol_name);
        return ERROR;
    }
    return EXIT_SUCCESS;
}

/*Handle first code line*/
void handle_first_code_line(struct assembler_table *table, struct lineStc *lineStc) {
    table->code_lines[table->IC] = lineStc->lineOption.inst.instructionType << OPCODE_POSITION;
    if (lineStc->lineOption.inst.operands[0].operandType != none) {
        table->code_lines[table->IC] |=  1 << (SOURCE_OPERAND_SORT + lineStc->lineOption.inst.operands[0].operandType);
        table->code_lines[table->IC] |= 1 << (TARGET_OPERAND_SORT + lineStc->lineOption.inst.operands[1].operandType);
    } else if (lineStc->lineOption.inst.operands[1].operandType != none && lineStc->lineOption.inst.operands[0].operandType == none){
        table->code_lines[table->IC] |= 1 << (TARGET_OPERAND_SORT + lineStc->lineOption.inst.operands[1].operandType);
    }
    table->code_lines[table->IC] |= 1 << ARE_FIELD;
    table->IC++;
}

/*Handle if both opernds are registers*/
void handle_if_both_registers(struct assembler_table *table, struct lineStc *lineStc) {
    /* Source operand */
    switch (lineStc->lineOption.inst.operands[0].operandType) {
        case indirectReg:
            table->code_lines[table->IC] = lineStc->lineOption.inst.operands[0].operandOption.indirectReg << SOURCE_OPERAND;
            break;
        case directReg:
            table->code_lines[table->IC] = lineStc->lineOption.inst.operands[0].operandOption.directReg << SOURCE_OPERAND;
            break;
        default:
            break;
    }
    /* Destination operand */
    switch (lineStc->lineOption.inst.operands[1].operandType) {
        case indirectReg:
            table->code_lines[table->IC] |= lineStc->lineOption.inst.operands[1].operandOption.indirectReg << TARGET_OPERAND;
            break;
        case directReg:
            table->code_lines[table->IC] |= lineStc->lineOption.inst.operands[1].operandOption.directReg << TARGET_OPERAND;
            break;
        default:
            break;
    }
    table->code_lines[table->IC] |= 1 << ARE_FIELD;
    table->IC++;
}

/*Handle opernds*/
void handle_operand(struct assembler_table *table, struct lineStc *lineStc, int operandNum) {
    switch (lineStc->lineOption.inst.operands[operandNum].operandType) {
        case none:
            break;
        case immidiate:
            table->code_lines[table->IC] = lineStc->lineOption.inst.operands[operandNum].operandOption.immidiate << IMMIDIATE;
            table->code_lines[table->IC] |= 1 << ARE_FIELD;
            table->IC++;
            break;
        case label:
            table->code_lines[table->IC] = 0;
            table->IC++;
            break;
        case indirectReg:
        case directReg:
            if (lineStc->lineOption.inst.operands[1].operandType != none) {
                switch (operandNum) {
                    /* Source operand */
                    case 0:
                        if (lineStc->lineOption.inst.operands[operandNum].operandType == indirectReg) {
                            table->code_lines[table->IC] = lineStc->lineOption.inst.operands[operandNum].operandOption.indirectReg << SOURCE_OPERAND;
                        } else {
                            table->code_lines[table->IC] = lineStc->lineOption.inst.operands[operandNum].operandOption.directReg << SOURCE_OPERAND;
                        }
                        table->code_lines[table->IC] |= 1 << ARE_FIELD;
                        table->IC++;
                        break;
                        /* Destination operand */
                    case 1:
                        if (lineStc->lineOption.inst.operands[operandNum].operandType == indirectReg) {
                            table->code_lines[table->IC] = lineStc->lineOption.inst.operands[operandNum].operandOption.indirectReg << TARGET_OPERAND;
                        } else {
                            table->code_lines[table->IC] = lineStc->lineOption.inst.operands[operandNum].operandOption.directReg << TARGET_OPERAND;
                        }
                        table->code_lines[table->IC] |= 1 << ARE_FIELD;
                        table->IC++;
                        break;
                    default:
                        break;
                }
                break;
            } else {
                if (lineStc->lineOption.inst.operands[operandNum].operandType == indirectReg) {
                    table->code_lines[table->IC] = lineStc->lineOption.inst.operands[operandNum].operandOption.indirectReg << TARGET_OPERAND;
                } else {
                    table->code_lines[table->IC] = lineStc->lineOption.inst.operands[operandNum].operandOption.directReg << TARGET_OPERAND;
                }
                table->code_lines[table->IC] |= 1 << ARE_FIELD;
                table->IC++;
            }
        default:
            break;
    }


}

/*Handle if the line is directive*/
int handle_directive(struct lineStc *lineStc, struct assembler_table *table, int lineNum) {
    int numOfData;
    /*Checks if has symbol*/
    if (lineStc->symbol_name[0] != EOS) {
        if (handle_if_entry(lineStc, table, lineNum)) {
            return ERROR;
        }
    }
    /*Checks if is data*/
    if (lineStc->lineOption.dir.directiveType == data) {

        memcpy(&table->data_lines[table->DC], &lineStc->lineOption.dir.directiveOptions.data, lineStc->lineOption.dir.directiveOptions.data.dataSize * (sizeof(int)));
        numOfData = lineStc->lineOption.dir.directiveOptions.data.dataSize;
        table->DC += numOfData;
    } else if (lineStc->lineOption.dir.directiveType <= entry){ /*Checks if entry or extern*/
        if(handle_if_extern_or_entry(lineStc, table, lineNum)) {
            return ERROR;
        }
    }
    return EXIT_SUCCESS;
}

/*Handle if the line is instruction*/
int handle_instruction(struct lineStc *lineStc, struct assembler_table *table, int lineNum) {
    /*Checks if has symbol*/
    if (lineStc->symbol_name[0] != EOS) {
        if(handle_if_entry(lineStc, table, lineNum)) {
            return ERROR;
        }
    }
    /*Handle first code line*/
    handle_first_code_line(table, lineStc);
    if((lineStc->lineOption.inst.operands[0].operandType == indirectReg || lineStc->lineOption.inst.operands[0].operandType == directReg) && (lineStc->lineOption.inst.operands[1].operandType == indirectReg || lineStc->lineOption.inst.operands[1].operandType == directReg)) {
        /*Handle if both opernds are registers*/
        handle_if_both_registers(table, lineStc);
    } else {
        /*Handle opernds*/
        handle_operand(table,lineStc, 0);
        handle_operand(table,lineStc, 1);
    }
    return EXIT_SUCCESS;
}

/*Checks the symbol table - logical errors*/
int check_and_update_symbol_table(struct assembler_table *table, int lineNum) {
    struct node *current = table->symbol_table.head;
    while (current != NULL) {
        struct symbol *current_symbol = (struct symbol *)current->data;
        if (current_symbol->symbolType == symbol_entry) {
            fprintf(stderr,"Error on line %d: symbol %s declered as entry but is not defined. \n", current_symbol->lineOfSymbol, current_symbol->symbolName);
            return ERROR;
        }
        if ((current_symbol->symbolType == symbol_data) || (current_symbol->symbolType == symbol_entryData)) {
            current_symbol->address += table->IC;
        }
        current = current->next;
    }
    return EXIT_SUCCESS;
}

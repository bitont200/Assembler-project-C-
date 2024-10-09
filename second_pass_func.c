/*@by Tamar Biton*/

#include "second_pass_func.h"

/*Function to add the addresses to the code*/
int add_symbols_code(struct lineStc *lineStc, struct assembler_table *table, int lineNum) {
    /*If instruction*/
    if(lineStc->lineType == instruction){
        table->IC++;
        /*If both operands are registers - go on*/
        if((lineStc->lineOption.inst.operands[0].operandType == indirectReg || lineStc->lineOption.inst.operands[0].operandType == directReg) && (lineStc->lineOption.inst.operands[1].operandType == indirectReg || lineStc->lineOption.inst.operands[1].operandType == directReg)) {
            table->IC++;
            return EXIT_SUCCESS;
        }
        /*Cheks operands*/
        if (handle_operand_code(table, lineStc, 0, lineNum)) {
            return ERROR;
        }
        if (handle_operand_code(table, lineStc, 1, lineNum)) {
            return ERROR;
        }
    }
    return EXIT_SUCCESS;
}

/*Function that checks the operand - if symbol add the address*/
int handle_operand_code(struct assembler_table *table, struct lineStc *lineStc, int operandNum, int lineNum) {   
	struct symbol * currentSyb; 
    switch (lineStc->lineOption.inst.operands[operandNum].operandType) {
    case none:
        break;
    case immidiate: /*If not symbol - go on*/
    case indirectReg:
    case directReg:
        table->IC++;
        break;
    case label: /*If symbol:*/
       	currentSyb = symbol_table_search_symbol(&table->symbol_table, lineStc->lineOption.inst.operands[operandNum].operandOption.label);
        if (currentSyb) {
            /*Add address to extern symbols*/
            if (currentSyb->symbolType == symbol_external) { 
                extern_addresses_add_address(&currentSyb->extern_addresses, &table->IC);
                table->code_lines[table->IC] |= 1;
                ll_destroy(&currentSyb->extern_addresses);
            } else {
                /*Translate address of symbol*/
                table->code_lines[table->IC] = (currentSyb->address+START_POINT)<<LABEL;
                table->code_lines[table->IC] |= 1<<1;
            }
        } else {
            fprintf(stderr,"Error on line %d: the symbol %s is not defined.\n", lineNum, lineStc->lineOption.inst.operands[operandNum].operandOption.label);
            return ERROR;  
        }
        table->IC++;
        break;       
    default:
        break;
    }   
    return EXIT_SUCCESS; 
}

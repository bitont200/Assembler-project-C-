/*@by Tamar Biton*/

#ifndef TABLES_H
#define TABLES_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "generic_linked_list.h"
#include "global_const.h"

/* Defines various constants used in the program */
#define MAX_SIZE_OF_FILE 4096  /*Max size of file */


/** Symbol:
 * 
 * Struct representing a symbol
 * @param symbolName The symbol name
 * @param symbolType Type of the symbol
 * @param lineOfSymbol Number of line of the symbol
 * @param address Adress of the symbol
 * @param extern_addresses If extern symbol - adresses of the appearens
 */
struct symbol{
    char symbolName[32]; 
    enum {
        symbol_external, /*Extern symbol*/
        symbol_entry,    /*Entry symbol*/ 
        symbol_command,  /*Symbol before command*/  
        symbol_data,     /*Symbol before data*/
        symbol_entryCommand, /*Entry symbol before command*/
        symbol_entryData     /*Entry symbol before data*/
    }symbolType;           
    int lineOfSymbol;     
    int address;      
    struct linked_list extern_addresses;        
};

/** Assembler_table:
 * 
 * Struct representing the assembler table
 * @param IC count of IC
 * @param DC count of DC
 * @param entries_count count of entry symbols
 * @param extern_count count of externs symbols
 * @param code_lines lines of translated code
 * @param data_lines lines of translated data
 * @param symbol_table linked list of symbols
 */
struct assembler_table {
    int IC;                                    
    int DC;                         
    int entries_count;                
    int extern_count;                 
    int code_lines[MAX_SIZE_OF_FILE];            
    int data_lines[MAX_SIZE_OF_FILE];           
    struct linked_list symbol_table;  
};

/**
 * make_new_symbol:
 * 
 * Function that creates a new symbol.
 * @param symbolName Pointer to a symbol name.
 * @param symbolType symbolType of new symbol.
 * @param lineOfSymbol line of the symbol.
 * @param address address of the symbol.
 * @return the new symbol created.
 */
struct symbol * make_new_symbol(char *symbolName, int symbolType, int lineOfSymbol, int address);

/**
 * symbol_table_add_symbol:
 * 
 * Function to add a new symbol to a symbol table.
 * @param symbol_table Pointer to a symbol table.
 * @param new_symbol Pointer to the new symbol to be added.
 */
void symbol_table_add_symbol(struct linked_list * symbol_table, struct symbol * new_symbol);

/**
 * symbol_table_search_symbol:
 * 
 * Function to search for a symbol in a symbol table by name.
 * @param symbol_table Pointer to a symbol table.
 * @param name The name of the symbol to search for.
 * @return Pointer to the found symbol, or NULL if not found.
 */
struct symbol * symbol_table_search_symbol(struct linked_list * symbol_table, char name[]);

/**
 * extern_addresses_add_address:
 * 
 * Function to add adress to linked list of extern adresses symbols.
 * @param extern_addresses, pointer to linked list of addresses.
 * @param address address to add.
 */
void extern_addresses_add_address(struct linked_list * extern_addresses, int * address);

/**
 * ll_destroy_symbols:
 * 
 * Function in order to destroy symbols
 * @param ll Pointer to linked list of symbols to destroy
 */
void ll_destroy_symbols(struct linked_list *ll);
#endif /* TABLES_H */

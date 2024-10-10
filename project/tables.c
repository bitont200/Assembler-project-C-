/*@by Tamar Biton*/

#include "tables.h"

/* Function to make new symbol */
struct symbol *make_new_symbol(char *symbolName, int symbolType, int lineOfSymbol, int address) {
    /*Allocate memory for the new symbol*/ 
    struct symbol *newSymbol = (struct symbol*)malloc(sizeof(struct symbol));
    if (!newSymbol) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);        
    }
    /*Copy parameters*/
    strncpy(newSymbol->symbolName, symbolName, sizeof(newSymbol->symbolName) - 1);
    newSymbol->symbolName[sizeof(newSymbol->symbolName) - 1] = EOS; 
    newSymbol->symbolType = symbolType;
    newSymbol->lineOfSymbol = lineOfSymbol;
    newSymbol->address = address;
    return newSymbol;
}

/* Function to add new symbol */
void symbol_table_add_symbol(struct linked_list *symbol_table, struct symbol *new_symbol) {
    /*Allocate memory for the new symbol*/ 
    struct symbol *symbol_copy = (struct symbol *)malloc(sizeof(struct symbol));
    if (!symbol_copy) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);        
    }

    /*Copy the symbol data*/ 
    memcpy(symbol_copy, new_symbol, sizeof(struct symbol));

    /*Insert the symbol into the linked list*/ 
    ll_insert(symbol_table, symbol_copy);
}

/*Function to search symbol in table*/
struct symbol *symbol_table_search_symbol(struct linked_list *symbol_table, char name[]) {
    struct node *current = symbol_table->head;
    while (current != NULL) {
		struct symbol *current_symbol = (struct symbol *) current -> data;
        if (strcmp(current_symbol->symbolName, name) == 0) {
            return current_symbol;
        }
        current = current->next;
    }
    /*Return NULL if the symbol is not found*/ 
    return NULL;
}

/*Function to add address to extern symbol*/
void extern_addresses_add_address(struct linked_list * extern_addresses, int * address){
    int *new_address = (int *)malloc(sizeof(int));
    if (!new_address) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);        
    }

    memcpy(new_address, address, sizeof(int));
    /*Insert the symbol into the linked list*/ 
    ll_insert(extern_addresses, new_address);
}

void ll_destroy_symbols(struct linked_list *ll) {
    struct node *pointer;

    /* Check if the linked list is NULL */
    if (ll == NULL) {
        return;
    }

    /* Pointer to traverse the linked list */
    pointer = ll->head;
    while (pointer != NULL) {
        /* Save the current node and move to the next node */
        struct node *temp = pointer;
        struct symbol *sym = (struct symbol *)temp->data;

        /* Free the memory allocated for the current symbol */
        free(sym);

        /* Move to the next node */
        pointer = pointer->next;
        
        /* Free the memory allocated for the current node */
        free(temp);
    }

    /* Reset the head pointer and size of the linked list */
    ll->head = NULL;
    ll->size = 0;
}


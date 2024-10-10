/*@by Shira Kedmi*/

#include "macro_structure.h"

void add_line_to_macro(Macro *macro, char *line_to_add) {
    /* Reallocate memory to add a new line to the macro */
    macro->macro_content = realloc(macro->macro_content, (macro->num_macro_lines + 1) * sizeof(char *));
    if (macro->macro_content == NULL) {
        /* Print an error message and exit if memory allocation fails */
        printf("Error reallocating memory for macro lines");
        exit(EXIT_FAILURE);
    }

    /* Allocate memory for the new line */
    macro->macro_content[macro->num_macro_lines] = malloc(strlen(line_to_add) + 1);
    if (macro->macro_content[macro->num_macro_lines] == NULL) {
        /* Print an error message and exit if memory allocation fails */
        printf("Error allocating memory for line buffer");
        exit(EXIT_FAILURE);
    }

    /* Copy the new line to the macro content */
    strcpy(macro->macro_content[macro->num_macro_lines], line_to_add);

    /* Increment the line count */
    macro->num_macro_lines++;
}

Macro *find_macro(Macro *head, char *macroName) {
    /* Traverse the linked list to find the macro by name */
    while (head != NULL) {
        if (strcmp(head->macro_name, macroName) == 0) {
            /* Return the macro if the name matches */
            return head;
        }
        head = head->next;
    }
    /* Return NULL if the macro is not found */
    return NULL;
}

void free_macro_table(Macro **head) {
    Macro *current = *head;
    Macro *next;
    int i;

    /* Traverse the linked list to free all macros */
    while (current != NULL) {
        next = current->next;

        /* Free each line in the macro content */
        for (i = 0; i < current->num_macro_lines; i++) {
            free(current->macro_content[i]);
        }

        /* Free the macro content array and the macro itself */
        free(current->macro_content);
        free(current);

        /* Move to the next macro in the list */
        current = next;
    }

    /* Set the head of the list to NULL */
    *head = NULL;
}


/*@by Tamar Biton*/

#include "generic_linked_list.h"

/* Function to insert a new node to linked list */
void ll_insert(struct linked_list *ll, void *data) {
    struct node *new_node;
    struct node *current;
    current = ll->head;
    
    /* Allocate memory for the new node */
    new_node = (struct node *)calloc(1, sizeof(struct node));
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
 
    /* Initialize the new node with provided data */
    new_node->data = data;
    new_node->next = NULL;
    
    /* If the linked list is empty, insert the new node at the head */
    if (ll->head == NULL) {
        ll->head = new_node;
    } else {
        /* Otherwise, find the last node and insert the new node at the end */
        current = ll->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }

    ll->size++;
}

/* Function to destroy the linked list and free allocated memory */
void ll_destroy(struct linked_list *ll) {
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
        pointer = pointer->next;
        /* Free the memory allocated for the current node */
        free(temp);
    }

    /* Reset the head pointer and size of the linked list */
    ll->head = NULL;
    ll->size = 0;
}

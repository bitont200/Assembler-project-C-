/*@by Tamar Biton*/

#ifndef GENERIC_LINKED_LIST_H
#define GENERIC_LINKED_LIST_H

#include <stdlib.h>
#include <stdio.h>

/**
 * Node:
 * 
 * Node structure for a linked list.
 * @param data Pointer to the data held by the node
 * @param next Pointer to the next node in the list
 */
struct node {
    void *data;        
    struct node *next;  
};

/**
 * Linked_list:
 * 
 * Structure representing a linked list.
 * @param head Pointer to the head node of the list
 * @param size Number of nodes in the list
 */
struct linked_list {
    struct node *head; 
    int size;           
};

/**
 * ll_insert:
 * 
 * Inserts a new node with the given data at the head of the linked list.
 * @param ll Pointer to the linked list.
 * @param data Pointer to the data to be stored in the new node.
 */
void ll_insert(struct linked_list *ll, void *data);

/**
 * ll_destroy:
 * 
 * Destroys the linked list, freeing all allocated memory.
 * @param ll Pointer to the linked list to be destroyed.
 */
void ll_destroy(struct linked_list *ll);

#endif /* GENERIC_LINKED_LIST_H */

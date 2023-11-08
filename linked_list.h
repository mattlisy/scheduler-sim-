#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "process.h"

// Type definition for Linked list data structure
typedef struct {
    	Process_node* head;
    	Process_node* tail;
   	int size;
} Linked_list;

// Returns the size of a Linked list
int sizeof_Linkedlist(const Linked_list list);

// Creates a Linked list
Linked_list create_Linkedlist();

// Inserts a node created from the data from 'process' before the element at 'elem'
void insert_before(Linked_list* list, const Process process, const int elem);

// Returns the process at element 'elem'
Process get(const Linked_list* list, const int elem);

// Finds the first process with an arrival time equal to 'compare_value'
int find_arrival_t(const Linked_list* list, const int compare_value);

// Removes and returns the process at element 'elem'
Process pop(Linked_list* list, const int elem);

// Prints the elements of the Linked list
void print_Linkedlist(const Linked_list list);

// Destroys the Linked list and frees its memory
void destroy_Linkedlist(Linked_list* list);

// increment each time_waited member Process in a Linked list
void incr_time_waited_Linkedlist(Linked_list* list);
#endif

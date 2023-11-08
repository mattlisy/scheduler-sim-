#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


// Creates a Linked list
Linked_list create_Linkedlist() {
	Linked_list new;
	new.head = new.tail = NULL;
	new.size = 0;
	return new;
}

// Returns the size of a Linked list
int sizeof_Linkedlist(const Linked_list list) {
	return list.size;
}

// Inserts a node created from the data from 'process' before the element at 'elem'
void insert_before(Linked_list* list, const Process process, const int elem) {
	assert(list != NULL);
  	assert(elem <= list->size);
  	Process_node* new = malloc(sizeof(Process_node));
  	new->process = process;    
 	if (list->size == 0) {
		new->next = NULL;
		list->head = list->tail = new;
	} else if (elem == 0) {
	new->next = list->head;
	list->head = new;    
	} else if (elem == list->size) {
		new->next = NULL;    
 	       	list->tail->next = new;
 	       	list->tail = new;
	} else {
	      	int i;
 	     	Process_node* temp;
 	     	for (i = 1, temp = list->head; temp != NULL && i <= elem; i++, temp = temp->next) {
     	   		if (i == elem) {
     	       			new->next = temp->next;
		 		temp->next = new;
     	   		}
     	     	}
	}
   	list->size++;
   	return;
}

// Returns the process at element 'elem'
Process get(const Linked_list* list, const int elem) {
	assert(list != NULL);
	assert(list->size != 0);
	assert(elem < list->size);
	if (elem == 0){
		return list->head->process;
	} else if (list->size-1 == elem) {
		return list->tail->process;
	} else {
		int i;
		Process_node* temp; 
		for (i = 1, temp = list->head->next; temp != NULL && elem >= i; i++, temp = temp->next) {
			if (elem == i) {
				return temp->process;
			}
		}
	}
	return (Process){-1,-1,-1,-1,-1};

}

// Finds the first process with an arrival time equal to 'compare_value'
int find_arrival_t(const Linked_list* list, const int compare_value) {
	assert(list != NULL);
	assert(list->size != 0);
	int i;
	Process_node* temp; 
	for (i = 0, temp = list->head; temp != NULL; i++, temp = temp->next) {
		if (temp->process.arrival_t == compare_value) {
			return i;
		}
	}
	return -1;
}

// Removes and returns the process at element 'elem'
Process pop(Linked_list* list, const int elem) {
	assert(list != NULL);
	assert(list->size != 0);
	assert(elem < list->size);
	if (elem == 0){
		list->size--;
		Process rvalue = list->head->process;	
		Process_node* temp = list->head;
		list->head = list->head->next;
		free(temp);
		return rvalue;
	} else {
		int i;
		Process_node* temp; 
		for (i = 0, temp = list->head; temp != NULL && elem > i; i++, temp = temp->next) {
			if (elem == i+1) {
				Process rvalue = temp->next->process;
				Process_node* del = temp->next;
				if (elem == list->size-1) {
					temp->next = NULL;
					list->tail = temp;	
					
				}else {	
					temp->next = temp->next->next;
				}			
				list->size--;
				free(del);
				return rvalue;
			}
		}
	}
	return (Process){-1,-1,-1,-1,-1};
}


// Prints the elements of the Linked list
void print_Linkedlist(const Linked_list list) {
	printf("\n\t\t\t\t   ====List====\n\n=========================================================================================\n");
	if (list.size == 0) {
		printf("list is empty\n");
		return;
	} else {
		for(Process_node* temp = list.head; temp != NULL; temp = temp->next) { 
		    printf("pid: %i, Arrival time: %i, Total CPU time: %i, I/O frequency: %i, I/O duration: %i\n\n========================================================================================\n", temp->process.pid, temp->process.arrival_t, temp->process.totalCPU_t, temp->process.io_freq, temp->process.io_dur);             }		
	}
	return;
}

// Destroys the Linked list and frees its memory
void destroy_Linkedlist(Linked_list* list) {
	assert(list != NULL);
	while(list->head != NULL) {
		Process_node* del = list->head;
		list->head = list->head->next;
		free(del);

	}
	list->tail = NULL;
	list->size = 0;
	return;
}

// increment each time_waited member Process in a Linked list
void incr_time_waited_Linkedlist(Linked_list* list) {
	assert(list != NULL);
	assert(list->size != 0);
	for (Process_node* temp = list->head; temp != NULL; temp = temp->next) {
		temp->process.time_waited++;
	}
} 

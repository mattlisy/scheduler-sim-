#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "Queue.h"

// returns the size of the Queue
int sizeof_Queue(const Queue queue) {
	return  queue.size;
}

// creates a Queue data structure
Queue create_Queue() {
   	Queue new;
    	new.head = new.tail = NULL;
    	new.size = 0;
    	return new;
}

// returns the value at front of Queue
Process peek(const Queue* queue) {
	assert(queue->size != 0);
	return queue->head->process;
}
	
// removes node at front and returns the value 
Process dequeue(Queue* queue) {
    	assert(queue != NULL);
    	assert(queue->size != 0);
    	Process rValue = queue->head->process;
    	Process_node* old = queue->head;
    	if (queue->size == 1) {
       		queue->head = NULL;
        	queue->tail = NULL;
    	} else {
      		queue->head = queue->head->next;
    	}
    	free(old);
    	queue->size--;
    	return rValue;
}

// adds a new node with data from process to the end of the Queue
void enqueue(Queue* queue, const Process process) {
	assert(queue != NULL);
    	Process_node* new = malloc(sizeof(Process_node));
    	assert(new != NULL);
    	new->process = process;
    	new->next = NULL;
    	if (queue->size == 0) {
       		queue->head = new;
        	queue->tail = new;
    	} else {
        	queue->tail->next = new;
        	queue->tail = new;
    	}	
    	queue->size++;
}

// prints the Queue to terminal
void print_Queue(Queue queue) {
	printf("\n\t\t\t\t   ===Queue===\n\n========================================================================================\n");
	if (queue.size == 0) {
		printf("queue is empty");	
		return;
	} else {	
		for(Process_node* temp = queue.head; temp != NULL; temp = temp->next) {

			printf("pid: %i, Arrival time: %i, Total CPU time: %i, I/O frequency: %i, I/O duration: %i\n\n========================================================================================\n", temp->process.pid, temp->process.arrival_t, temp->process.totalCPU_t, temp->process.io_freq, temp->process.io_dur);
			}
		}	
	return;

}

// free all nodes from the Queue
void destroy_Queue(Queue* queue) {
	assert(queue != NULL);

    	while (queue->head != NULL) {
        	Process_node* del = queue->head;
        	queue->head = queue->head->next; 
		free(del);
    	}

    	queue->size = 0;
	queue->tail = NULL;
}


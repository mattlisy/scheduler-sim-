#ifndef QUEUE_H
#define QUEUE_H

#include "process.h"

// type definition for the Queue data structure
typedef struct {
    Process_node* head;
    Process_node* tail;
    int size;
} Queue;

// returns the size of the Queue
int sizeof_Queue(const Queue queue);

// creates a Queue data structure
Queue create_Queue();

// removes node at front and returns the value 
Process dequeue(Queue* queue);

// returns the value at front of Queue
Process peek(const Queue* queue);

// adds a new node with data from process to the end of the Queue
void enqueue(Queue* queue, const Process process);

// prints the Queue to terminal
void print_Queue(const Queue queue);

// free all nodes from the Queue
void destory_Queue(Queue* queue);

#endif


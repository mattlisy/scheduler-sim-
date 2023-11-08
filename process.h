#ifndef PROCESS_H
#define PROCESS_H

// type defintion for Process 
typedef struct {
	int pid;
	int arrival_t;
	int totalCPU_t;
	int io_freq;
	int io_dur;
	int elapsed_time;
	int time_waited;
} Process;

// type defintion for Process_node to be used in data structures
typedef struct Process_node {
	Process process;
	struct Process_node* next;
} Process_node;

#endif 

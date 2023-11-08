/*
Matthew Lisy 101231092 
Kyle Taticek 101193550

assumptions: 
	
	- interrupt is not needed for this sim
	
	- terminate has priority over event 

	- admit has priority over event completion	

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "Queue.h"
#include "linked_list.h"

#define BUFFER_SIZE 64 		// buffer to read file
#define START_TIME 0 		// sim start time
#define NO_PROCESS (Process){-1, -1, -1, -1, -1, -1, -1}  // represents a process that does not exist
#define INTERRUPT_FREQ 50 	// interrupt happens every 50ms


int timer = START_TIME;
int processes = 0;

void help(void) { 

	printf("commands: -i \"file.csv\" / --input \"file.csv\"\n");
	
}
/*
	Simulator Output to file
*/
void Output(const int pid, const char* old_state, const char* new_state) {
	time_t now = time(NULL);
	char filename[50];
	struct tm* timeinfo = localtime(&now);
	strftime(filename, sizeof(filename), "Simulator-Output-%Y%m%d%H%M%S.txt", timeinfo);
  
	FILE* Output = fopen(filename, "a");
	if (Output == NULL) {
		perror("error opening output file");
	} 
	fprintf(Output, "time: %i, pid: %i, old state: %s, new_state: %s\n\n", timer, pid, old_state, new_state);
	fclose(Output);	
}
	
/*
	Takes data csv provides and creates processes from them. 
	Each process is added to a linked list. 
	Linked list is returned represent a data structure for the new state
*/ 
Linked_list input(char** file) { 
	// print file name
	printf("file: %s\n", *file);
	// open file
	FILE *data = fopen(*file, "r");
	if (data == NULL) { 
		perror("error opening file");
		exit(0);
	}
	
	Linked_list new_list = create_Linkedlist();
	char buffer[BUFFER_SIZE];
	int first_iteration = 1; // flag to skip headers of CSV
	while (fgets(buffer, BUFFER_SIZE, data)) { 
		
		// skip headers
		if (first_iteration) {
			first_iteration = 0;
			continue;
		}
		Process new;	

		// fill new process 	
		new.pid = atoi(strtok(buffer, ","));
		new.arrival_t = atoi(strtok(NULL, ","));
		new.totalCPU_t = atoi(strtok(NULL, ","));
		new.io_freq = atoi(strtok(NULL, ","));
		new.io_dur = atoi(strtok(NULL, ","));	
		new.elapsed_time = 0;
		new.time_waited = 0;

		// add process to new
		insert_before(&new_list, new, 0);
		processes++;	
	} 
	fclose(data);	
	return new_list;
}
/*
	Checks to see if a process does not exist
	returns a bool to represent this; 1 if y; 0 if n
*/
bool isNoProcess(Process* process) {
	return (process->pid == -1 &&
        	process->arrival_t == -1 &&
 	        process->totalCPU_t == -1 &&
             	process->io_freq == -1 &&
             	process->io_dur == -1 &&
            	process->elapsed_time == -1 && process->time_waited == -1);
}

/*
	Admits the process in the new state to the ready state
*/
void admit(Linked_list* new_list, Queue* ready_queue) {
	int x;
	while(sizeof_Linkedlist(*new_list) != 0 && (x = find_arrival_t(new_list, timer)) != -1) {

		Process temp = pop(new_list, x); 	
		printf("\n ===admit: %i===\n", temp.pid);
		Output(temp.pid, "new", "ready"); 
		enqueue(ready_queue, temp);	
	}	
	return;	
}	

/*
	dispatches a process to from ready state to a running state
*/
void dispatch(Queue* ready_queue, Process* running) {
	if (sizeof_Queue(*ready_queue) == 0) {
		return;
	} else if (isNoProcess(running)){
		*running = dequeue(ready_queue);
		printf("\n ===dispatch: %i===\n", running->pid);	
		Output(running->pid, "ready", "running");

	}
	return; 
}
/*
	Terminates a process when program is complete
	Transitions the process from a running state to a terminate state
*/ 
void terminate(Process* running) {
	if (isNoProcess(running)) {	
		return;
	} else if (running->elapsed_time >= running->totalCPU_t) {
		Output(running->pid, "running", "terminated");	
		printf("\n ===terminated: %i===\n", running->pid);
		*running = NO_PROCESS;	
		processes--;
	}
	return;
}


/*

void interrupt(Process* running, Queue* ready_queue) {
	if (isNoProcess(running)) {
		return;
	}else if (running->elapsed_time % INTERRUPT_FREQ == 0) {
		Output(running->pid, "ready", "interrupt");
		printf("\n===interrupt: %i===\n", running->pid);
		enqueue(ready_queue, *running);
		*running = NO_PROCESS;
	}
	
	return;

}	
*/

/*
	io/system event will occur depending on the process io frequency
	Transitions the process from running state to a wait state
*/
void event(Process* running, Linked_list* wait_list) {
	if (isNoProcess(running)) {
		return;
	} else if(running->elapsed_time % running->io_freq == 0) {
		Output(running->pid, "running", "waiting");
		printf("\n === event: %i===\n", running->pid);
		insert_before(wait_list, *running, 0);
		*running = NO_PROCESS;
	} 		
	return;
}

/*
	increments wait time for each process in the wait state
	Transitions process from wait state to ready state when
	io/system event has finished exection
*/
void event_complete(Linked_list* wait_list, Queue* ready_queue) {

	if (sizeof_Linkedlist(*wait_list) == 0) {	
		return;
	} else {
		incr_time_waited_Linkedlist(wait_list);
		for (int i = 0; i < sizeof_Linkedlist(*wait_list); i++) {
			Process compare_value = get(wait_list, i);
			if (compare_value.time_waited == compare_value.io_dur) {
				Output(compare_value.pid, "waiting", "ready");
				printf("\n ===event complete: %i===\n", compare_value.pid);
				Process ready = pop(wait_list, i);
				ready.time_waited = 0;	
				enqueue(ready_queue, ready);	
				i--; // reprocess curr elem; pop shift elems down
			} else {
				printf("\nwaiting: %i: %i\n", compare_value.pid, compare_value.time_waited);
			}
		}
	}
	
	return;
} 



int main(int agrc, char* agrv[]) {
	
	//data structures 	
	Linked_list new_list;	
	Queue ready_queue = create_Queue();
	Process running = NO_PROCESS;
	Linked_list wait_list = create_Linkedlist();		

	// cli flags
	if (agrc < 2) {
		perror("error no commands supplied");
		return EXIT_FAILURE;
	}	

	for (int i = 1; i < agrc; i++) { 
		if ((strcmp(agrv[i], "-h") == 0) || (strcmp(agrv[i], "--help") == 0)) {
			help();
		} else if ((strcmp(agrv[i], "-i") == 0) || (strcmp(agrv[i],"--input") == 0)) {
			new_list = input(&agrv[++i]);
		} else {
			printf("%s is not a command", agrv[i]);
		}		
	}
	
	// sim loop	
	while(processes){

		printf("\ntime: %i\n", timer);
		if (!isNoProcess(&running)) {	
			printf("running: %i: %i\n", running.pid, running.elapsed_time);
		}	

		admit(&new_list, &ready_queue);
			
		event_complete(&wait_list, &ready_queue);

			
		terminate(&running);			

		// interrupt(&running, &ready_queue);
		
		event(&running, &wait_list);
		

		dispatch(&ready_queue, &running);

		if (!isNoProcess(&running)) {	
			running.elapsed_time++;	
		}

		timer++;			
	}	
}

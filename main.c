/***************************************************************************
 *
 *   File        : main.c
 *   Student Id  : <INSERT STUDENT ID HERE>
 *   Name        : <INSERT STUDENT NAME HERE>
 *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <string.h>
#include "tasks.h"

#define DEBUG_MAIN 1

int main(int argc, char *argv[]) {
	
	/* TODO: Parse Command Line Arguments */
	char* flow_file = NULL;
	int resolution = 0;
	
	flow_file = argv[1];
	resolution = atoi(argv[2]);
	
	struct timeval start;
	struct timeval stop;
        
	//Check for correct input arguments
	if (argc != 3){
		printf("usage: %s <data file> <resolution>", argv[0]);
		return 0;
	}
	if(DEBUG_MAIN){
		printf("Data File: %s\nGrid Resolution: %d\n",flow_file,resolution);
	}
	printf("\n");
        

	/* Task 1: Find the maximum velocity difference */
	gettimeofday(&start, NULL);
	maxveldiff(flow_file);
	gettimeofday(&stop, NULL);
	double elapsed_ms = (stop.tv_sec - start.tv_sec) * 1000.0;
	elapsed_ms += (stop.tv_usec - start.tv_usec) / 1000.0;
	printf("TASK 1:  %.2f milliseconds\n", elapsed_ms);
	printf("\n");

	/* Task 2: Coarser Grid */
	gettimeofday(&start, NULL);
	coarsegrid(flow_file, resolution);
	gettimeofday(&stop, NULL);
    	elapsed_ms = (stop.tv_sec - start.tv_sec) * 1000.0;
    	elapsed_ms += (stop.tv_usec - start.tv_usec) / 1000.0;
    	printf("TASK 2:  %.2f milliseconds\n", elapsed_ms);
	printf("\n");

	/* Task 3: Searching */
//	gettimeofday(&start, NULL);
//	searching(flow_file);
//	gettimeofday(&stop, NULL);
//    	elapsed_ms = (stop.tv_sec - start.tv_sec) * 1000.0;
//    	elapsed_ms += (stop.tv_usec - start.tv_usec) / 1000.0;
//   	printf("TASK 3:  %.2f milliseconds\n", elapsed_ms);
//        printf("\n");
//
//	/* Task 4: Statistics */
//	gettimeofday(&start, NULL);
//	vortcalc(flow_file);
//	gettimeofday(&stop, NULL);
//    	elapsed_ms = (stop.tv_sec - start.tv_sec) * 1000.0;
//    	elapsed_ms += (stop.tv_usec - start.tv_usec) / 1000.0;
//    	printf("TASK 4:  %.2f milliseconds\n", elapsed_ms);
	
	printf("All Tasks Successful\n");
    
	return (EXIT_SUCCESS);
}

/***************************************************************************
 *
 *   File        : tasks.c
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
#include <assert.h>

//Constants
#define BUFFER 50

//Debug Switches
#define DEBUG_TASK1 0
#define DEBUG_TASK2 0
#define DEBUG_TASK3 0
#define DEBUG_TASK4 0
#define TASK2_CONSOLE_RESULTS 0

#define BST_SUCCESS 1
#define BST_FAILURE 0


void maxveldiff(const char* flow_file)
{
    
    //Search Data keeping track of uMin, uMax, vMin, vMax
    //O(n)
    
    //Open Data File
    FILE *data_in;
    data_in = fopen(flow_file, "r");
    if(data_in == NULL){
        printf("Failed to Open File\n");
        return;
    }
    
    //Store Column Headings
    char headings[BUFFER];
    
    //Check heading format
    fscanf(data_in, "%s",headings);
    if(strcmp(headings,"x,y,u,v")!=0){
        printf("Incorrect data format - expecting <x,y,u,v>\n");
        printf("Data is in %s\n",headings);
        return;
    }
     
    //Initialise Temp Variables
    float xTemp, yTemp, uTemp, vTemp;
    
    //Use first point to initialise min and max variables
    fscanf(data_in, "%f,%f,%f,%f", &xTemp,&yTemp,&uTemp,&vTemp);
    
    float uMin = uTemp;
    float uMax = uTemp;
    float vMin = vTemp;
    float vMax = vTemp;
    
    struct point_data *uMin_point = setData1(xTemp,yTemp, uTemp,vTemp);
    struct point_data *uMax_point = setData1(xTemp,yTemp, uTemp,vTemp);
    struct point_data *vMin_point = setData1(xTemp,yTemp, uTemp,vTemp);
    struct point_data *vMax_point = setData1(xTemp,yTemp, uTemp,vTemp);
    
    //Process Data
    while (fscanf(data_in, "%f,%f,%f,%f", &xTemp,&yTemp,&uTemp,&vTemp) > 0){
        
        if(DEBUG_TASK1){
            printf("%f,%f,%f,%f\n", xTemp,yTemp,uTemp,vTemp);
        }
        
        if(uTemp>uMax){
            uMax = uTemp;
            uMax_point = setData1(xTemp,yTemp, uTemp,vTemp);
        }
        if(uTemp<uMin){
            uMin = uTemp;
            uMin_point = setData1(xTemp,yTemp, uTemp,vTemp);
        }
        if(vTemp>vMax){
            vMax = vTemp;
            vMax_point = setData1(xTemp,yTemp, uTemp,vTemp);
        }
        if(vTemp<vMin){
            vMin = vTemp;
        }
    }
    
    //Close Data File
    fclose(data_in);
    
    //Print Results to Console
    if(DEBUG_TASK1){
        printf("%s\n",headings);
        printData(uMin_point);
        printData(uMax_point);
        printData(vMin_point);
        printData(vMax_point);
    }
    
    //Print Results to CSV
    FILE *data_out;
    data_out = fopen("task1.csv","w+");
    
    fprintf(data_out,"%s\n",headings);
    fprintData(uMin_point,data_out);
    fprintData(uMax_point,data_out);
    fprintData(vMin_point,data_out);
    fprintData(vMax_point,data_out);
    
    fclose(data_out);
    
    return;
}

void coarsegrid(const char* flow_file, int resolution)
{
    //Grid Range
    
    float CG_XMIN = 10;
    float CG_XMAX = 70;
    float CG_YMIN = -20;
    float CG_YMAX = 20;
    
    //Calculate dX and dY
    float dx = (CG_XMAX-CG_XMIN)/(resolution-1);
    float dy = (CG_YMAX-CG_YMIN)/(resolution-1);
    
    if(DEBUG_TASK2){
        printf("dx = %f dy= %f\n",dx,dy);
    }
    
    list_t** grid[resolution];
    
    for(int i=0; i<resolution; i++){
    	grid[i] = (list_t**)malloc(resolution * sizeof(list_t*));

    	for(int j=0; j<resolution; j++){
    		grid[i][j] = list_new();

//    	Use head node to store running sum for (x,y,u,v) for averaging
			list_push_front(grid[i][j],setData2(0,0,0,0,0));

			if(DEBUG_TASK2){
				printf("1 grid[%i][%i] -",i,j);
				printList(grid[i][j],1);
			}
    	}
    }
    
    if(DEBUG_TASK2){
       for(int i=0; i<resolution; i++){
            for(int j=0; j<resolution; j++){
            	printf("2 grid[%i][%i] -",i,j);
            	printList(grid[i][j],1);
            }
        } 
    }
    
    
    //Open Data File
    FILE *data_in;
    data_in = fopen(flow_file, "r");
    
    if(data_in == NULL){
        printf("Failed to Open File\n");
        return;
    }
    
    //Store Column Headings
    char headings[BUFFER];
    
    //Check heading format
    fscanf(data_in, "%s",headings);
    if(strcmp(headings,"x,y,u,v")!=0){
        printf("Incorrect data format - expecting <x,y,u,v>\n");
        printf("Data is in %s\n",headings);
        return;
    }
     
    //Initialise Temp Variables
    float xTemp, yTemp, uTemp, vTemp;
    float xShift, yShift;
    
    
    while (fscanf(data_in, "%f,%f,%f,%f", &xTemp,&yTemp,&uTemp,&vTemp) > 0){
        
        if(DEBUG_TASK2){
            printf("Processing Point (x,y,u,v) = (%f,%f,%f,%f)\n",
                    xTemp,yTemp,uTemp,vTemp);
        }
        
        //Check if point is within range
        if(xTemp>=CG_XMIN && xTemp<=CG_XMAX && yTemp>=CG_YMIN && yTemp<=CG_YMAX){
            
            //Shift by XMIN and YMIN
            xShift = xTemp - CG_XMIN;
            yShift = yTemp - CG_YMIN;
            
            //Calculate Grid location
            int xCell = xShift/dx;
            int yCell = yShift/dy;
            
            if(DEBUG_TASK2){
                printf("Shifted (x,y)= (%f,%f) -> Grid[%i,%i]\n",xShift,yShift,xCell,yCell);
            }

            //Add values to running total
            runningSum(grid[xCell][yCell],xTemp,yTemp,uTemp,vTemp);

            //Add to back of Linked List
            list_push_back(grid[xCell][yCell],setData1(xTemp,yTemp,uTemp,vTemp));
        }else{
        	if(DEBUG_TASK2){
        		printf("Point out of range\n");
        	}

        }
    }
    
    //Check List Sizes
    if(DEBUG_TASK2){
       for(int i=0; i<resolution; i++){
            for(int j=0; j<resolution; j++){
                printf("grid[%i,%i]->Num_elements = %i\n",i,j,grid[i][j]->num_elements);
                printf("Sum (x,y,u,v) = (%f,%f,%f,%f)\n",
                		grid[i][j]->head->data.x,grid[i][j]->head->data.y,
						grid[i][j]->head->data.u,grid[i][j]->head->data.v);
            }
        } 
    }
    
    //Convert head of each list from running sum to average and calculate head value
    for(int i=0; i<resolution; i++){
		for(int j=0; j<resolution; j++){
			calcAverage(grid[i][j]);
			calcSValue(grid[i][j]);
		}
    }

    if(TASK2_CONSOLE_RESULTS){
	   for(int i=0; i<resolution; i++){
			for(int j=0; j<resolution; j++){
				printf("grid[%i,%i] - (x_av,y_av,u_av,v_av,s) = (%f,%f,%f,%f,%f)\n",i,j,
						grid[i][j]->head->data.x,grid[i][j]->head->data.y,
						grid[i][j]->head->data.u,grid[i][j]->head->data.v,
						grid[i][j]->head->data.s);
			}
		}
	}

   //Transfer cell averages to array

    int numCells = pow(resolution,2);
    int index = 0;

    struct point_data sPoints[numCells];

    for(int i=0; i<resolution; i++){
		for(int j=0; j<resolution; j++){
			sPoints[index] = grid[i][j]->head->data;
			index += 1;
		}
    }

    if(TASK2_CONSOLE_RESULTS){
    	printf("S-Points Before Sorting\n");
       	   for(int i=0; i<numCells; i++){
       		   printDataS(&sPoints[i]);
       	   }
     }

    //Sort Array d = descending, s = sort by S values
//    mergeSort(sPoints,0,numCells,'d','s');
    qsort(sPoints,numCells,sizeof(struct point_data),qSortCMP_sDecending);

    if(TASK2_CONSOLE_RESULTS){
    	printf("S-Points After Sorting\n");
   	   for(int i=0; i<numCells; i++){
   		   printDataS(&sPoints[i]);
   	   }
    }

    //Print Results to CSV
	FILE *data_out;
	data_out = fopen("task2.csv","w+");

	fprintf(data_out,"%s,s\n",headings);
	for(int i=0; i<numCells; i++){
	   fprintDataS(&sPoints[i],data_out);
	}
	fclose(data_out);

    //TODO Free Memory

}

void searching(const char* flow_file)
{
	//Open Data File
	FILE *data_in;
	data_in = fopen(flow_file, "r");

	if(data_in == NULL){
		printf("Failed to Open File\n");
		return;
	}

	//Store Column Headings
	char headings[BUFFER];

	//Check heading format
	fscanf(data_in, "%s",headings);
	if(strcmp(headings,"x,y,u,v")!=0){
		printf("Incorrect data format - expecting <x,y,u,v>\n");
		printf("Data is in %s\n",headings);
		return;
	}

	//Create List of y=0 Points
	list_t* midPointsList = list_new();

	float xTemp, yTemp, uTemp, vTemp;

	//Process Points
	while(fscanf(data_in, "%f,%f,%f,%f", &xTemp,&yTemp,&uTemp,&vTemp) > 0){

		if(DEBUG_TASK3){
			printf("Processing Point (x,y,u,v) = (%f,%f,%f,%f)\n",
					xTemp,yTemp,uTemp,vTemp);
		}

		if(yTemp == 0){
		    list_push_back(midPointsList,setData1(xTemp,yTemp,uTemp,vTemp));
		}else{
			if(DEBUG_TASK3){
				printf("Point out of Range\n");
			}
		}
	}

	//Create Array y=0 Points
	point_data_Array midPointsArray;
	midPointsArray.array = malloc(midPointsList->num_elements*sizeof(point_data));
	midPointsArray.size = 0;

	//Populate Array
	for(int i=0; midPointsList->num_elements>0; i++){
		midPointsArray.array[i] = list_pop_front(midPointsList);
		midPointsArray.size++;
	}

	if(DEBUG_TASK3){
		printf("Data Before Sorting\n");
		for(int i=0; i<midPointsArray.size; i++){
			printData(&midPointsArray.array[i]);
		}
	}

	//Sort Array a = ascending, u = sort by U values
//	mergeSort(midPointsArray.array,0,midPointsArray.size,'a','u');

	qsort(midPointsArray.array,midPointsArray.size,sizeof(struct point_data),qSortUcmp);

	if(DEBUG_TASK3){
		printf("Data After Sorting\n");
		for(int i=0; i<midPointsArray.size; i++){
			printData(&midPointsArray.array[i]);
		}
	}

	//Insert into Sorted Linked List
	for(int i=0; i<midPointsArray.size; i++){
		list_push_back(midPointsList,&midPointsArray.array[i]);
	}

	//Insert into BST
	bst_t* bst = bst_new(no_free, floatcmp);
	perfect_insert(bst, midPointsArray.array, 0, midPointsArray.size - 1);
	assert(bst->num_elements == midPointsArray.size);

	if(DEBUG_TASK3){
		printf("BST - num_elements = %d\n", bst->num_elements);
		print_BST(bst->root);
	}

	//Get Max U Velocity
	float uMax = midPointsArray.array[midPointsArray.size-1].u;

	//Target u Velocity
	float uTarget = 0.4*uMax;


	if(DEBUG_TASK3){
		printf("uMax = %f, uTarget = %f\n",uMax,uTarget);
	}

	FILE *data_out;
	data_out = fopen("task3.csv","w+");

	struct timeval start;
	struct timeval stop;

	//Linear Search of Array
	gettimeofday(&start, NULL);
	float closestValue = arrayLinearSearch(midPointsArray.array,uTarget,data_out);
	fprintf(data_out,"%f\n",closestValue);
	gettimeofday(&stop, NULL);
	double 	elapsed_ms = (stop.tv_sec - start.tv_sec) * 1000.0;
			elapsed_ms += (stop.tv_usec - start.tv_usec) / 1000.0;
	printf("TASK 3 Array Linear Search:  %.2f milliseconds\n", elapsed_ms);


	//Binary Search of Array
	gettimeofday(&start, NULL);
		closestValue = arrayBinarySearch(midPointsArray.array,midPointsArray.size,uTarget,data_out);
	fprintf(data_out,"%f\n",closestValue);
	gettimeofday(&stop, NULL);
		elapsed_ms = (stop.tv_sec - start.tv_sec) * 1000.0;
		elapsed_ms += (stop.tv_usec - start.tv_usec) / 1000.0;
	printf("TASK 3 Array Binary Search:  %.2f milliseconds\n", elapsed_ms);

	//Linear Search of Linked List
	gettimeofday(&start, NULL);
		closestValue = linkedListLinearSearch(midPointsList->head,uTarget,data_out);
	fprintf(data_out,"%f\n",closestValue);
	gettimeofday(&stop, NULL);
		elapsed_ms = (stop.tv_sec - start.tv_sec) * 1000.0;
		elapsed_ms += (stop.tv_usec - start.tv_usec) / 1000.0;
	printf("TASK 3 Linked List Linear Search:  %.2f milliseconds\n", elapsed_ms);

	//Linear Search of Linked List
	gettimeofday(&start, NULL);
	bstSearch(bst->root,uTarget,data_out);
	printf("%f\n",closestValue);
	gettimeofday(&stop, NULL);
		elapsed_ms = (stop.tv_sec - start.tv_sec) * 1000.0;
		elapsed_ms += (stop.tv_usec - start.tv_usec) / 1000.0;
	printf("TASK 3 BST Search:  %.2f milliseconds\n", elapsed_ms);

}

void vortcalc(const char* flow_file)
{
	//Open Data File
	FILE *data_in;
	data_in = fopen(flow_file, "r");

	if(data_in == NULL){
		printf("Failed to Open File\n");
		return;
	}

	//Store Column Headings
	char headings[BUFFER];

	//Check heading format
	fscanf(data_in, "%s",headings);
	if(strcmp(headings,"x,y,u,v")!=0){
		printf("Incorrect data format - expecting <x,y,u,v>\n");
		printf("Data is in %s\n",headings);
		return;
	}

	//Define holding variables
	float xTempPrev, xTemp;
	float yTempPrev, yTemp;
	float uTempPrev, uTemp;
	float vTempPrev, vTemp;

	int xRange = 1;
	int yRange = 1;
	int endOfX = 0;
	int endOfY = 0;

	fscanf(data_in, "%f,%f,%f,%f", &xTempPrev,&yTempPrev,&uTempPrev,&vTempPrev);

	//Scan data to calculate nessarry dimensions of x and y
	while(fscanf(data_in, "%f,%f,%f,%f", &xTemp,&yTemp,&uTemp,&vTemp) > 0){
		if(xTempPrev>xTemp){
					endOfX = 1;
		}
		if(xTempPrev != xTemp && endOfX == 0){
			xRange++;
		}
		if(yTempPrev != yTemp && endOfY == 0){
			yRange++;
		}
		if(yTempPrev > yTemp){
			endOfY = 1;
		}
		xTempPrev = xTemp;
		yTempPrev = yTemp;
		uTempPrev = uTemp;
		vTempPrev = vTemp;
	}

	if(DEBUG_TASK4){
			printf("Array Dimensions = [%i,%i]\n",xRange,yRange);
	}

	//Define 2D array to hold points
	point_data*** points = (point_data***)malloc(xRange*sizeof(point_data**));


	for(int i = 0; i <= xRange; i++){
		points[i] = (point_data**)malloc(yRange*sizeof(point_data*));

		for(int j = 0 ; j <= yRange ; j++){
			points[i][j] = (point_data*)malloc(sizeof(point_data));
		}
	}

	//Re open File to scan for the second time.
	rewind(data_in);

	int xIndex = 0;
	int yIndex = 0;

	fscanf(data_in, "%s",headings);

	//Process Data
	while(fscanf(data_in, "%f,%f,%f,%f", &xTemp,&yTemp,&uTemp,&vTemp) > 0){
		if(xIndex > xRange-1){
			xIndex = 0;
			yIndex++;
		}
		if(yIndex > yRange-1){
			yIndex = 0;
		}
		points[xIndex][yIndex]=setData1(xTemp,yTemp,uTemp,vTemp);
		xIndex++;
	}

	if(DEBUG_TASK4){
		for(int i = 0; i < xRange; i++){
			for(int j = 0; j < yRange; j++){
				printData(points[i][j]);
			}
		}
	}

	//Define 2D array for omega values

	float omega[xRange][yRange];

	for(int i = 0 ; i < xRange ; i++){
		for(int j = 0 ; j < yRange ; j++){
			if(i == xRange-1 && j == yRange-1){
				omega[i][j] = cornerCase(points,i,j);
			}else if(j == yRange-1){
				omega[i][j] = yEdgeCase(points,i,j);
			}else if(i == xRange-1){
				omega[i][j] = xEdgeCase(points,i,j);
			}else{
				omega[i][j] = normalCase(points,i,j);
			}
		}
	}
	if(DEBUG_TASK4){
		printf("\nOmega Vales\n");
		for(int i = 0 ; i < xRange ; i++){
			for(int j = 0 ; j < yRange ; j++){
				printf("%f\n",omega[i][j]);
			}
		}
	}

	//Copy values into 1D array to be sorted
	int numValues = xRange*yRange;
	int index = 0;

	float omegaArray[numValues];

	for(int i = 0 ; i < xRange ; i++){
		for(int j = 0 ; j < yRange ; j++){
			omegaArray[index] = omega[i][j];
			index++;
		}
	}

	qsort(omegaArray,numValues,sizeof(float),qSortCMP_floats);

	if(DEBUG_TASK4){
		printf("\nSorted Vales\n");
		for(int i = 0 ; i<numValues ; i++){
			printf("%f\n",omegaArray[i]);
		}
	}



	FILE *data_out;
	data_out = fopen("task4.csv","w+");

	fprintf(data_out,"omega\n");
	for(int i=0; i<numValues; i++){
	   fprintf(data_out,"%f\n",omegaArray[i]);
	}

	fclose(data_out);

}

//##########################################################################
//########################### FUNCTIONS ####################################
//##########################################################################

float cornerCase(point_data*** points, int i, int j){
	float part1 = (points[i][j]->v - points[i-1][j]->v)/
			(points[i][j]->x-points[i-1][j]->x);
	float part2 = (points[i][j]->u-points[i][j-1]->u)/
			(points[i][j]->y - points[i][j-1]->y);
	return (part1 - part2);
}

float yEdgeCase(point_data*** points, int i, int j){
	float part1 = (points[i+1][j]->v - points[i][j]->v)/
			(points[i+1][j]->x-points[i][j]->x);
	float part2 = (points[i][j]->u-points[i][j-1]->u)/
			(points[i][j]->y - points[i][j-1]->y);
	return (part1 - part2);
}

float xEdgeCase(point_data*** points, int i, int j){
	float part1 = (points[i][j]->v - points[i-1][j]->v)/
			(points[i][j]->x-points[i-1][j]->x);
	float part2 = (points[i][j+1]->u-points[i][j]->u)/
			(points[i][j+1]->y - points[i][j]->y);
	return (part1 - part2);
}
float normalCase(point_data*** points, int i, int j){
	float part1 = (points[i+1][j]->v - points[i][j]->v)/
			(points[i+1][j]->x-points[i][j]->x);
	float part2 = (points[i][j+1]->u-points[i][j]->u)/
			(points[i][j+1]->y - points[i][j]->y);
	return (part1 - part2);
}

struct point_data* setData1(float x, float y, float u, float v){
    struct point_data *point = malloc(sizeof(struct point_data));
    point->x=x;
    point->y=y;
    point->u=u;
    point->v=v;
    point->s=0;
    return point;
}

struct point_data* setData2(float x, float y, float u, float v, float s){
    struct point_data *point = malloc(sizeof(struct point_data));
    point->x=x;
    point->y=y;
    point->u=u;
    point->v=v;
    point->s=s;
    return point;
}

void printData(struct point_data *point){
    printf("%f,%f,%f,%f\n",point->x,point->y,point->u,point->v);
}

void printDataS(struct point_data *point){
    printf("%f,%f,%f,%f,%f\n",point->x,point->y,point->u,point->v,point->s);
}

void fprintData(struct point_data *point,FILE *file){
    fprintf(file,"%f,%f,%f,%f\n",point->x,point->y,point->u,point->v);
}

void fprintDataS(struct point_data *point,FILE *file){
    fprintf(file,"%f,%f,%f,%f,%f\n",
    		point->x,point->y,point->u,point->v,point->s);
}

void fprintNode(struct node node,FILE *file){
    fprintf(file,"%f,%f,%f,%f,%f\n",
    		node.data.x,node.data.y,node.data.u,node.data.v,node.data.s);
}

list_t* list_new()
{
    list_t* list;
    list = (list_t*)malloc(sizeof(list_t));
    assert(list != NULL);
    list->head = NULL;
    list->tail = NULL;
    list->num_elements = 0;
    return list;
}

void list_push_front(list_t* list, struct point_data* data)
{
    assert(list != NULL);
    node_t* new = (node_t*)malloc(sizeof(node_t));
    assert(new != NULL);
    new->data = *data;
    new->next = list->head;
    list->head = new;
    if (list->tail == NULL)
        list->tail = new;
    list->num_elements++;
}

void list_push_back(list_t* list, struct point_data* data)
{
    assert(list != NULL);
    node_t* new = (node_t*)malloc(sizeof(node_t));
    assert(new != NULL);
    new->data = *data;
    new->next = NULL;
    if (list->tail)
        list->tail->next = new;
    list->tail = new;
    if (list->head == NULL)
        list->head = new;
    list->num_elements++;
}

struct point_data list_pop_front(list_t* list)
{
    assert(list != NULL);
    assert(list->num_elements > 0);
    node_t* old;
    assert(list->head != NULL);
    old = list->head;
    list->head = list->head->next;
    struct point_data d = old->data;
    free(old);
    list->num_elements--;
    if (list->num_elements == 0) {
        list->head = NULL;
        list->tail = NULL;
    }
    return d;
}

void runningSum(list_t* list, float x, float y,float u,float v){
	assert(list != NULL && "running sum = NULL");
	list->head->data.x += x;
	list->head->data.y += y;
	list->head->data.u += u;
	list->head->data.v += v;
}
void calcAverage(list_t* list){
	if(list->num_elements>0){
		list->head->data.x = list->head->data.x/(list->num_elements-1);
		list->head->data.y = list->head->data.y/(list->num_elements-1);
		list->head->data.u = list->head->data.u/(list->num_elements-1);
		list->head->data.v = list->head->data.v/(list->num_elements-1);
	}else{
		printf("WARNING - Cannot Divide by 0\n");
	}
}

void calcSValue(list_t* list){
	assert(list != NULL);
	float x_av_2 = pow(list->head->data.x,2);
	float y_av_2 = pow(list->head->data.y,2);
	float u_av_2 = pow(list->head->data.u,2);
	float v_av_2 = pow(list->head->data.v,2);
	list->head->data.s = 100*((sqrt(u_av_2+v_av_2)/sqrt(x_av_2+y_av_2)));
}

void printList(list_t* list,int recursive){
	assert(list != NULL);
	printf("num_elements->%i\n",list->num_elements);

	node_t* tempNode = (node_t*)malloc(sizeof(node_t));
	tempNode = list->head;

	printf("%f,%f,%f,%f,%f\n",tempNode->data.x,tempNode->data.y,
			tempNode->data.u,tempNode->data.v,tempNode->data.s);
	if(recursive){
		while(tempNode->next != NULL){
			printf("%f,%f,%f,%f\n",tempNode->data.x,tempNode->data.y,
					tempNode->data.u,tempNode->data.v);
			tempNode = tempNode->next;
		}
	}
}

void printNode(struct node node){
	printf("%f,%f,%f,%f,%f\n",
			node.data.x,node.data.y,node.data.u,node.data.v,node.data.s);
}

/* create a new empty bst structure */
bst_t* bst_new(void (*delfunc)(void*), int (*cmpfunc)(struct point_data*, struct point_data*))
{
    bst_t* bst;
    bst = (bst_t*)malloc(sizeof(bst_t));
    assert(bst != NULL);
    bst->root = NULL;
    bst->num_elements = 0;
    bst->del = delfunc;
    bst->cmp = cmpfunc;
    return bst;
}

/* free all memory assocated with a subtree */
void bst_free_subtree(bst_t* bst, BSTnode_t* n)
{
    assert(bst != NULL);
    if (n) {
        bst_free_subtree(bst, n->left);
        bst_free_subtree(bst, n->right);
        bst->del(n->data);
        free(n);
        bst->num_elements--;
    }
}

/* free all memory associated with a bst */
void bst_free(bst_t* bst)
{
    assert(bst != NULL);
    bst_free_subtree(bst, bst->root);
    free(bst);
}

/* insert a new element into the bst */
int bst_insert(bst_t* bst, void* d)
{
    assert(bst != NULL);
    assert(d != NULL);
    BSTnode_t* parent = NULL;
    BSTnode_t* tmp = bst->root;
    while (tmp) {
        parent = tmp;
        if (bst->cmp(tmp->data, d) > 0) { // element is smaller
            tmp = tmp->left;
        }
        else if (bst->cmp(tmp->data, d) < 0) { // element is bigger
            tmp = tmp->right;
        }
        else {
            printf("Error floatcmp returned: %i\n",bst->cmp(tmp->data, d));

            return BST_FAILURE;
        }
    }

    /* insert as child of parent */
    BSTnode_t* new_node = (BSTnode_t*)malloc(sizeof(BSTnode_t));
    assert(new_node != NULL);
    new_node->data = d;
    new_node->left = NULL;
    new_node->right = NULL;
    if (parent != NULL) {
        if (bst->cmp(parent->data, d) > 0) { // element is smaller
            assert(parent->left == NULL);
            parent->left = new_node;
        }
        else {
            assert(parent->right == NULL);
            parent->right = new_node;
        }
    }
    else {
        assert(bst->root == NULL);
        bst->root = new_node;
    }
    bst->num_elements++;

    return BST_SUCCESS;
}

int floatcmp(struct point_data* a, struct point_data* b)
{
	if(a->u > b->u)
		return 1;
	else
		return -1;

//    return (*(float*)a) - (*(float*)b);
}
// Implement this function (0)
void perfect_insert(bst_t* bst, struct point_data* array, int low, int high)
{
    if (low <= high) {
    	// Choose root from array and insert
    	// Recursively do the same on left and right (1)
        int mid = low + (high - low) / 2;
        point_data* ptr = array + mid;
        bst_insert(bst, ptr);
        perfect_insert(bst, array, low, mid - 1);
        perfect_insert(bst, array, mid + 1, high);
    }
}

void no_free(void* v)
{
}

int make_unique(int* array, int n)
{
    int dest = 0;
    int itr = 1;
    while (itr != n) {
        if (array[dest] != array[itr]) {
            array[++dest] = array[itr];
        }
        itr++;
    }
    return dest+1;
}
void print_BST(BSTnode_t* node){
	assert(node != NULL);
	if(node->left != NULL){
		print_BST(node->left);
	}
	printData((struct point_data*)(node->data));
	if(node->right != NULL){
		print_BST(node->right);
	}
	return;
}
float absF(float a, float b){
	if(a<b)
		return b-a;
	else
		return a-b;
}


// Returns element closest to target in array[]
float arrayBinarySearch(struct point_data array[],int size,float target,FILE *file)
{
    // Check for Edge Cases
    if (target <= array[0].u)
        return array[0].u;
    if (target >= array[size - 1].u)
        return array[size - 1].u;

    // Binary Search
    int i = 0, j = size, mid = 0;
    while (i < j) {
        mid = (i + j) / 2;

        if (array[mid].u == target)
            return array[mid].u;

        fprintf(file,"%f,",array[mid].u);
        /* If target is less than array element,
            then search in left */
        if (target < array[mid].u) {

            // If target is greater than previous
            // to mid, return closest of two
            if (mid > 0 && target > array[mid - 1].u)
            	return getClosest(array[mid - 1].u,array[mid].u, target);

            /* Repeat for left half */
            j = mid;
        }

        // If target is greater than mid
        else {
            if (mid < size - 1 && target < array[mid + 1].u)
                return getClosest(array[mid].u, array[mid + 1].u, target);
            // update i
            i = mid + 1;
        }
    }
    // Only single element left after search
    return array[mid].u;
}

// Method to compare which one is the more close.
// We find the closest by taking the difference
// between the target and both values. It assumes
// that val2 is greater than val1 and target lies
// between these two.
float getClosest(float val1, float val2, float target)
{
    if (target - val1 >= val2 - target)
        return val2;
    else
        return val1;
}

float arrayLinearSearch(struct point_data array[],float uTarget, FILE *file){
	int i = 0;
	while(absF(uTarget, array[i+1].u) <	absF(uTarget, array[i].u)){
		fprintf(file,"%f,",array[i].u);
		i++;
	}
	return array[i].u;
}

float linkedListLinearSearch(node_t *node, float uTarget, FILE *file){
	assert(node != NULL);
	if(node->next == NULL){
		return node->data.u;
	}else if(absF(uTarget, node->next->data.u) < absF(uTarget,node->data.u)){
		fprintf(file,"%f,",node->data.u);
		return linkedListLinearSearch(node->next,uTarget,file);
	}
	return node->data.u;
}

void bstSearch(BSTnode_t* root, float uTarget, FILE *file){
	BSTnode_t *closest_point = root;
	bstSearchUtil(root,closest_point,uTarget,file);
	return;
}

void bstSearchUtil(BSTnode_t* root, BSTnode_t* closestNode, float uTarget,FILE *file){
	if(root == NULL){
		return;
	}
	struct point_data *current_point = (struct point_data*)(root->data);
	struct point_data *closest_node = (struct point_data*)(closestNode->data);

	fprintf(file,"%f,",current_point->u);

	//Check if target matches node
	if(current_point->u == uTarget){
		closestNode = root;
		return;
	}

	//Check if current node closer than closestNode
	if(absF(current_point->u,uTarget) < absF(closest_node->u,uTarget)){
		closestNode = root;
	}

	//Traverse Left or Right
	if(current_point->u < uTarget){
		bstSearchUtil(root->right,closestNode,uTarget,file);
	}else{
		bstSearchUtil(root->left,closestNode,uTarget,file);
	}
}

int qSortUcmp(const void *a, const void *b)
{
    struct point_data *ia = (struct point_data *)a;
    struct point_data *ib = (struct point_data *)b;

    if(ia->u > ib->u)
    	return 1;
    else
    	return -1;
}
int qSortCMP_sDecending(const void *a, const void *b){
	struct point_data *ia = (struct point_data *)a;
	struct point_data *ib = (struct point_data *)b;

	if(ia->s > ib->s)
		return -1;
	else
		return 1;
}
int qSortCMP_floats(const void *a, const void *b){
	float *ia = (float*)a;
	float *ib = (float*)b;

	if(*ia > *ib)
		return -1;
	else
		return 1;
}

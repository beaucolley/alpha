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

#define BUFFER 50
#define DEBUG_TASK1 0
#define DEBUG_TASK2 1
#define TASK2_CONSOLE_RESULTS 1

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
    
    struct point_data *uMin_point = setData(xTemp,yTemp, uTemp,vTemp);
    struct point_data *uMax_point = setData(xTemp,yTemp, uTemp,vTemp);
    struct point_data *vMin_point = setData(xTemp,yTemp, uTemp,vTemp);
    struct point_data *vMax_point = setData(xTemp,yTemp, uTemp,vTemp);
    
    //Process Data
    while (fscanf(data_in, "%f,%f,%f,%f", &xTemp,&yTemp,&uTemp,&vTemp) > 0){
        
        if(DEBUG_TASK1){
            printf("%f,%f,%f,%f\n", xTemp,yTemp,uTemp,vTemp);
        }
        
        if(uTemp>uMax){
            uMax = uTemp;
            uMax_point = setData(xTemp,yTemp, uTemp,vTemp);
        }
        if(uTemp<uMin){
            uMin = uTemp;
            uMin_point = setData(xTemp,yTemp, uTemp,vTemp);
        }
        if(vTemp>vMax){
            vMax = vTemp;
            vMax_point = setData(xTemp,yTemp, uTemp,vTemp);
        }
        if(vTemp<vMin){
            vMin = vTemp;
            vMin_point = setData(xTemp,yTemp, uTemp,vTemp);
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
    
    int CG_XMIN = 10;
    int CG_XMAX = 70;
    int CG_YMIN = -20;
    int CG_YMAX = 20;
    
    //Calculate dX and dY
    float dx = (CG_XMAX-CG_XMIN)/(resolution-1);
    float dy = (CG_YMAX-CG_YMIN)/(resolution-1);
    
    if(DEBUG_TASK2){
        printf("dx = %f dy= %f\n",dx,dy);
    }
    
    //Define a 2D array of linked lists to store points for each grid cell
    list_t*** grid = (list_t***)malloc(sizeof(list_t**));
    
    for(int i=0;i<resolution;i++){
        grid[i] =(list_t**)malloc(resolution * sizeof(list_t*));
        
        for(int j=0;j<resolution;j++){
            grid[i][j] = list_new();
            
            //Use head node to store running sum for (x,y,u,v) for averaging
            list_push_front(grid[i][j],setData(0,0,0,0));

            if(DEBUG_TASK2){
            	printf("1 -");
            	printList(grid[i][j],1);
            }


        }
    }
    
    if(DEBUG_TASK2){
       for(int i=0; i<resolution; i++){
            for(int j=0; j<resolution; j++){
            	printf("2 -");
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
    
//    fscanf(data_in, "%f,%f,%f,%f", &xTemp,&yTemp,&uTemp,&vTemp);   
//    list_push_front(grid[1][1],setData(xTemp,yTemp,uTemp,vTemp));
//    printf("NumElements: %d\n",grid[1][1]->num_elements);
    
    while (fscanf(data_in, "%f,%f,%f,%f", &xTemp,&yTemp,&uTemp,&vTemp) > 0){
        
        if(DEBUG_TASK2){
            printf("Processing Point (x,y,u,v) = (%f,%f,%f,%f)\n",
                    xTemp,yTemp,uTemp,vTemp);
        }
        
        //Check if point is within range
        if(xTemp>=CG_XMIN && xTemp<=CG_XMAX && yTemp>=CG_YMIN && yTemp<=CG_YMAX){
            
            //Shift by XMIN and YMIN
            xTemp = xTemp - CG_XMIN;
            yTemp = yTemp - CG_YMIN;
            
            //Calculate Grid location
            int xCell = round(xTemp/dx);
            int yCell = round(yTemp/dy);
            
            if(DEBUG_TASK2){
                printf("Shifted (x,y)= (%f,%f) -> Grid[%i,%i]\n",xTemp,yTemp,xCell,yCell);
            }
            //Add values to running total
            //TODO Need to sum non shifted Values
            runningSum(grid[xCell][yCell],xTemp,yTemp,yTemp,vTemp);

            //Add to back of Linked List
            list_push_back(grid[xCell][yCell],setData(xTemp,yTemp,yTemp,vTemp));
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
    
    //Convert head of each list from running sum to average
    for(int i=0; i<resolution; i++){
		for(int j=0; j<resolution; j++){
			printf("S Value and Average: grid[%i,%i]\n",i,j);
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

    //TODO Sort and print in decending S values

    int numCells = pow(resolution,2);
    int index = 0;

    struct node sPoints[numCells];

    for(int i=0; i<resolution; i++){
		for(int j=0; j<resolution; j++){
			sPoints[index] = *grid[i][j]->head;
			index += 1;
		}
    }

    	//Create node array
    	//Merge Sort Node Array
    	//Open File
    	//fPrint to CSV
    	//Close Node Array

    //TODO Free Memory

}

void searching(const char* flow_file)
{
    printf("searching() - IMPLEMENT ME!\n");

 	//Create Sub Array of all y = 0
 	//O(n)

 	//Quicksort uMin to uMax
 	//O(nlog(n))

 	//Create Linked List
 	//O(n)

 	//Create BST
 	//O(nLog(n))

 	//Linear Search of Array
 	//O(n)

 	//Binary Seach of Array
 	//O(logN)

 	//Linear Seach of Linked List
 	//O(N)

    //Search on BST
}

void vortcalc(const char* flow_file)
{
    printf("vortcalc() - IMPLEMENT ME!\n");
}

struct point_data* setData(float x, float y, float u, float v){
    struct point_data *point = malloc(sizeof(struct point_data));
    point->x=x;
    point->y=y;
    point->u=u;
    point->v=v;
    return point;
}

void printData(struct point_data *point){
    printf("%f,%f,%f,%f\n",point->x,point->y,point->u,point->v);
}

void fprintData(struct point_data *point,FILE *file){
    fprintf(file,"%f,%f,%f,%f\n",point->x,point->y,point->u,point->v);
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

void runningSum(list_t* list, float x, float y,float u,float v){
	assert(list != NULL);
	list->head->data.x += x;
	list->head->data.y += y;
	list->head->data.u += u;
	list->head->data.v += v;
}
void calcAverage(list_t* list){
	if(list->num_elements>0){
		list->head->data.x = list->head->data.x/list->num_elements;
		list->head->data.y = list->head->data.y/list->num_elements;
		list->head->data.u = list->head->data.u/list->num_elements;
		list->head->data.v = list->head->data.v/list->num_elements;
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

// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* create temp arrays */
    int L[n1], R[n2];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void mergeSort(int arr[], int l, int r)
{
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);

        merge(arr, l, m, r);
    }
}

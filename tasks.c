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
            	printf("1 - grid[%i,%i]\n",i,j);
				printf("->Num_elements = %i\n",grid[i][j]->num_elements);
				printf("->Head = %p\n",grid[i][j]->head);
            }


        }
    }
    
    if(DEBUG_TASK2){
       for(int i=0; i<resolution; i++){
            for(int j=0; j<resolution; j++){
            printf("2 - grid[%i,%i]->Num_elements = %i\n",i,j,grid[i][j]->num_elements);
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
    //O(1)
  	//Sort into 2D array of linked list

    //O(n)

    //Average Each Linked List
    //O(n)

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
    assert(new);
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
	list->head->data.x = list->head->data.x/list->num_elements;
	list->head->data.y = list->head->data.y/list->num_elements;
	list->head->data.u = list->head->data.u/list->num_elements;
	list->head->data.v = list->head->data.v/list->num_elements;
}

void calcSValue(list_t* list){
	float x_av_2 = pow(list->head->data.x,2);
	float y_av_2 = pow(list->head->data.y,2);
	float u_av_2 = pow(list->head->data.u,2);
	float v_av_2 = pow(list->head->data.v,2);
	list->head->data.s = 100*((sqrt(u_av_2+v_av_2)/sqrt(x_av_2+y_av_2)));
}

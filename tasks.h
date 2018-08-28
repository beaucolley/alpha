/***************************************************************************
 *
 *   File        : tasks.h
 *   Student Id  : <INSERT STUDENT ID HERE>
 *   Name        : <INSERT STUDENT NAME HERE>
 *
 ***************************************************************************/


#ifndef TASKS_H

typedef struct point_data{
    float x;
    float y;
    float u;
    float v;
    float s;
}point_data;

/* node type */
typedef struct node node_t;

struct node {
    struct point_data data;
    node_t* next;
};

/* linked list type */
typedef struct {
    int num_elements;
    node_t* head;
    node_t* tail;
} list_t;


void maxveldiff(const char* flow_file);

void coarsegrid(const char* flow_file, int resolution);

void searching(const char* flow_file);

void vortcalc(const char* flow_file);

struct point_data* setData1(float x, float y, float u, float v);

struct point_data* setData2(float x, float y, float u, float v, float s);

void printData(struct point_data *point);

void fprintData(struct point_data *point, FILE *file);

void fprintNode(struct node node,FILE *file);

list_t* list_new();

void list_push_front(list_t* list, struct point_data* data);

void list_push_back(list_t* list, struct point_data* data);

void runningSum(list_t* list, float x, float y,float u,float v);

void calcAverage(list_t* list);

void calcSValue(list_t* list);

void printList(list_t* list,int recursive);

void printNode(struct node node);

void merge(struct node arr[], int l, int m, int r);

void mergeSort(struct node arr[], int l, int r);


#endif

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

/* node type */
typedef struct BSTnode BSTnode_t;

struct BSTnode {
    void* data;
    BSTnode_t* left;
    BSTnode_t* right;
};

/* bst type */
typedef struct {
    int num_elements;
    BSTnode_t* root;
    void (*del)(void*);
    int (*cmp)(const void*, const void*);
} bst_t;

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

typedef struct {
  point_data *array;
  size_t size;
}point_data_Array;

void maxveldiff(const char* flow_file);

void coarsegrid(const char* flow_file, int resolution);

void searching(const char* flow_file);

void vortcalc(const char* flow_file);

struct point_data* setData1(float x, float y, float u, float v);

struct point_data* setData2(float x, float y, float u, float v, float s);

void printData(struct point_data *point);

void printDataS(struct point_data *point);

void fprintData(struct point_data *point, FILE *file);

void fprintDataS(struct point_data *point,FILE *file);

void fprintNode(struct node node,FILE *file);

list_t* list_new();

void list_push_front(list_t* list, struct point_data* data);

void list_push_back(list_t* list, struct point_data* data);

struct point_data list_pop_front(list_t* list);

void runningSum(list_t* list, float x, float y,float u,float v);

void calcAverage(list_t* list);

void calcSValue(list_t* list);

void printList(list_t* list,int recursive);

void printNode(struct node node);

void merge(struct point_data arr[], int l, int m, int r, char order, char element);

void mergeSort(struct point_data arr[], int l, int r, char order, char element);

bst_t* bst_new(void (*delfunc)(void*), int (*cmpfunc)(const void*, const void*));

void bst_free_subtree(bst_t* bst, BSTnode_t* n);

void bst_free(bst_t* bst);

int bst_insert(bst_t* bst, void* d);

int floatcmp(struct point_data* a, struct point_data* b);

void perfect_insert(bst_t* bst, struct point_data* array, int low, int high);

void no_free(void* v);

int make_unique(int* array, int n);


#endif

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

// Linked List node type
typedef struct node node_t;

//Linked List type
typedef struct {
    int num_elements;
    node_t* head;
    node_t* tail;
} list_t;

//Linked List Node
struct node {
    struct point_data data;
    node_t* next;
};

//Array Structure
typedef struct {
  point_data *array;
  size_t size;
}point_data_Array;

//BST Node Type
typedef struct BSTnode BSTnode_t;

//BST Type
typedef struct {
    int num_elements;
    BSTnode_t* root;
    void (*del)(void*);
    int (*cmp)(struct point_data*, struct point_data*);
} bst_t;

//BST Node
struct BSTnode {
    void* data;
    BSTnode_t* left;
    BSTnode_t* right;
};

float cornerCase(point_data*** points, int i, int j);

float xEdgeCase(point_data*** points, int i, int j);

float yEdgeCase(point_data*** points, int i, int j);

float normalCase(point_data*** points, int i, int j);

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

bst_t* bst_new(void (*delfunc)(void*), int (*cmpfunc)(struct point_data*, struct point_data*));

void bst_free_subtree(bst_t* bst, BSTnode_t* n);

void bst_free(bst_t* bst);

int bst_insert(bst_t* bst, void* d);

int floatcmp(struct point_data* a, struct point_data* b);

void perfect_insert(bst_t* bst, struct point_data* array, int low, int high);

void no_free(void* v);

int make_unique(int* array, int n);

void print_BST(BSTnode_t* node);

float absF(float a, float b);

float getClosest(float val1, float val2, float target);

float arrayBinarySearch(struct point_data array[], int size, float target, FILE *file);

float arrayLinearSearch(struct point_data array[],float uTarget,FILE *file);

float linkedListLinearSearch(node_t *node, float uTarget, FILE *file);

void bstSearch(BSTnode_t* root, float uTarget, FILE *file);

void bstSearchUtil(BSTnode_t* root, BSTnode_t* closestNode, float uTarget,FILE *file);

int qSortUcmp(const void *a, const void *b);

int qSortCMP_sDecending(const void *a, const void *b);

int qSortCMP_floats(const void *a, const void *b);

#endif

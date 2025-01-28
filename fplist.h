#ifndef FPLIST_H_
#define FPLIST_H_

#if !fp_node_alloc && !FP_FREE
#include <stdlib.h>
#endif

#include <stddef.h>
#include <stdio.h>

#ifndef FP_MALLOC
#define FP_MALLOC(s) malloc(s) 
#endif

#ifndef FP_FREE
#define FP_FREE(p) free(p)
#endif

#define fp_create(e) { e, NULL }

typedef void* FP_Any;

typedef struct FP_Node {
  FP_Any this;
  struct FP_Node *next;
} FP_Node;

typedef struct {
  FP_Node *head;
  FP_Node *tail;
} FP_List;

/* Common switch function */
void fp_switch(FP_Any, FP_Any);

/* Allocates a node using FP_MALLOC */
FP_Node* fp_node_alloc();

/* Allocates a list using FP_MALLOC */
FP_List* fp_list_alloc();

/* Frees the list using FP_FREE */
void fp_list_free(FP_List *);

/* Adds the element to a new node at the end and returns a pointer to it */
FP_Node *fp_list_append(FP_List *, FP_Node *);

/* Allocates a new node to the end and returns a pointer to it */
FP_Node *fp_list_create_append(FP_List *);

/* Inserts the the element in a node at the beggining and returns a pointer to it */
FP_Node *fp_list_insert(FP_List *, FP_Node *);

/* Allocates a new node to the beggining and returns a pointer to it */
FP_Node *fp_list_create_insert(FP_List *);

/* Returns a pointer to the node with the corresponding element. NULL if not found */
FP_Node *fp_list_find(FP_List *, FP_Any);

/* Removes the node with the corresponding element and returns a pointer to the removed node */
FP_Node *fp_list_remove(FP_List *, FP_Any);

/* Removes and frees the node with the corresponding element. Returns 0 if the element was not found and 1 otherwise */
int fp_list_delete(FP_List *, FP_Any);

/* Replaces the node with corresponding element and returns the pointer to new node, NULL if the element was not found */
FP_Node *fp_list_replace(FP_List *, FP_Any, FP_Any);

/* Prints the node to stdout */
void fp_node_print(FP_Node *);

/* Prints the list to stdout */
void fp_list_print(FP_List *);

#endif //FPLIST_H_

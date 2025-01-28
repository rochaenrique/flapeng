#include "fplist.h"

void fp_switch(FP_Any a, FP_Any b)
{
  FP_Any temp = a;
  a = b;
  b = temp;
}

FP_Node* fp_node_alloc()
{
  return (FP_Node *)FP_MALLOC(sizeof(FP_Node *));
}

FP_List* fp_list_alloc()
{
  return (FP_List *)FP_MALLOC(sizeof(FP_List *));
}

void fp_node_free(FP_List *list)
{
  FP_Node *p, *q;
  for (p = list->head; p; p = q) {
	q = p->next;
	FP_FREE(p);
  }
}

FP_Node *fp_list_append(FP_List *list, FP_Node* node)
{
  if (list->tail) 
	list->tail->next = node;
  else if (list->head) 
	list->head->next = node;
  else {
	node->next = NULL;
	return list->head = node;
  }
  
  return list->tail = node;
}

FP_Node *fp_list_create_append(FP_List *list)
{
  FP_Node *node;
  return (node = fp_node_alloc()) ? fp_list_append(list, node) : NULL;
}

FP_Node *fp_list_insert(FP_List *list, FP_Node *node)
{
  if (list->head) {
	node->next = list->head;
	if (!list->tail) 
	  list->tail = list->head;
	return list->head = node;
  }

  node->next = list->tail;
  return list->head = node;
}

FP_Node *fp_list_create_insert(FP_List *list)
{
  FP_Node *node;
  return (node = fp_node_alloc()) ? fp_list_insert(list, node) : NULL;
}


FP_Node *fp_list_find(FP_List *list, FP_Any el)
{
  FP_Node *p;
  for (p = list->head; p && p->this != el; p = p->next);
  return p;
}

FP_Node *fp_list_find_pre(FP_List *list, FP_Any el)
{
  FP_Node *p = list->head;
  if (p->this != el)
	for (; p && p->next && p->next->this != el; p = p->next);

  return p;
}


FP_Node *fp_list_remove(FP_List *list, FP_Any el)
{
  FP_Node *p, *q;
  if (p = fp_list_find_pre(list, el)) {
	q = p->next;
	p->next = p->next->next;
	return q;
  }
  return NULL;
}

int fp_list_delete(FP_List *list, FP_Any el)
{
  int a;
  FP_Node *p;
  if (a = ((p = fp_list_remove(list, el)) != NULL)) 
	FP_FREE(p);

  return a;
}

FP_Node *fp_list_replace(FP_List *list, FP_Any from, FP_Any to)
{
  FP_Node *p;
  if ((p = fp_list_find(list, from))) 
	p->this = to;

  return p;
}

void fp_node_print(FP_Node *p)
{
  if (p) 
	printf("%p { this: %p, next: %p }\n", p, p->this, p->next );
  else
	printf("%p {}\n", p);
}


void fp_list_print(FP_List *list)
{
  FP_Node *p;
  int c = 0;
  if (list) 
	for (p = list->head; p; p = p->next, c++) {
	  printf("\t[%d] ", c);
	  fp_node_print(p);
	}

  printf("\tlist size: %d\n", c);
}

#include "fplist.h"

void fp_switch(FP_Any a, FP_Any b)
{
  FP_Any temp = a;
  a = b;
  b = temp;
}

FP_Node *fp_node_alloc()
{
  FP_Node *p;
  if ((p = (FP_Node *)FP_MALLOC(sizeof(FP_Node *))))
	p->next = p->this = NULL;
  
  return p;
}

FP_List *fp_list_alloc()
{
  FP_List *list;
  if ((list = (FP_List *)FP_MALLOC(sizeof(FP_List *)))) 
	list->tail = list->head = NULL;
  
  return list;
}

void fp_list_free(FP_List *list)
{
  FP_Node *p, *q;
  for (p = list->head; p; p = q) {
	q = p->next;
	FP_FREE(p);
  }
  FP_FREE(list);
}

FP_Node *fp_list_append(FP_List *list, FP_Node* node)
{
  if (list->tail) 
	list->tail->next = node;
  else if (list->head)
	list->head->next = node;
  else 
	return list->head = node;

  list->tail = node;
  list->tail->next = NULL;
  return list->tail;
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

FP_Node *_fp_list_find_pair(FP_List *list, FP_Any el, FP_Node **pre)
{
  FP_Node *p;
  if ((p = list->head)->this == el) 
	*pre = NULL;
  else 
	for (p = p->next; p && p->this != el; p = p->next)
	  *pre = p;
  return p;
}


FP_Node *fp_list_remove(FP_List *list, FP_Any el)
{
  FP_Node *found, *pre;
  if ((found = _fp_list_find_pair(list, el, &pre))) {
	if (!pre)
	  list->head = found->next;
	else
	  pre->next = found->next;
	return found;
  }
  return NULL;
}

int fp_list_delete(FP_List *list, FP_Any el)
{
  int a;
  FP_Node *p;
  if ((a = ((p = fp_list_remove(list, el))) != NULL)) 
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

FP_Node *fp_list_rotate(FP_List *list)
{
 FP_Node *p;
  return (p = fp_list_remove(list, list->head->this)) ? fp_list_append(list, p) : NULL;
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

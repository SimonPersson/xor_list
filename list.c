#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef int value_t;

typedef struct node {
	struct node *next_prev;
	value_t data;
} node;

typedef struct {
	node *head;
	node *tail;
} list;

void init(list *l)
{
	l->head=l->tail = NULL;
}

value_t insert_back(list *l, value_t d)
{
	node *new_node = malloc(sizeof(node));

	if(l->head==NULL) //empty list, new_node is the only node
	{
		l->head = l->tail = new_node;
		new_node->next_prev = (node *)((uintptr_t)new_node^(uintptr_t)new_node);
	}
	else
	{
		node *old_tail = l->tail;
		old_tail->next_prev = (node*)((uintptr_t)old_tail->next_prev^(uintptr_t)l->head);
		old_tail->next_prev = (node*)((uintptr_t)old_tail->next_prev^(uintptr_t)new_node);
		l->tail = new_node;
		new_node->next_prev = (node *)((uintptr_t)old_tail^(uintptr_t)l->head);
		l->head->next_prev = (node *)(((uintptr_t)l->head->next_prev^(uintptr_t)old_tail)^(uintptr_t)l->tail);
	}
	return new_node->data = d;
}

value_t insert_front(list *l, value_t d)
{
	node *new_node = malloc(sizeof(node));

	if(l->head==NULL) //empty list, new_node is the only node
	{
		l->head = l->tail = new_node;
		new_node->next_prev = (node *)((uintptr_t)new_node^(uintptr_t)new_node);
	}
	else
	{
		node *old_head = l->head;
		old_head->next_prev = (node*)((uintptr_t)old_head->next_prev^(uintptr_t)l->tail);
		old_head->next_prev = (node*)((uintptr_t)old_head->next_prev^(uintptr_t)new_node);
		l->head = new_node;
		new_node->next_prev = (node *)((uintptr_t)old_head^(uintptr_t)l->tail);
		l->tail->next_prev = (node *)(((uintptr_t)l->tail->next_prev^(uintptr_t)old_head)^(uintptr_t)l->head);
	}
	return new_node->data = d;
}

void remove_head(list *l)
{
	node *oldhead = l->head;

	l->head = (node *)((uintptr_t)oldhead->next_prev^(uintptr_t)l->tail);

	l->head->next_prev = (node *)((uintptr_t)l->head->next_prev^(uintptr_t)oldhead);
	l->head->next_prev = (node *)((uintptr_t)l->head->next_prev^(uintptr_t)l->tail);

	l->tail->next_prev = (node *)((uintptr_t)l->tail->next_prev^(uintptr_t)oldhead);
	l->tail->next_prev = (node *)((uintptr_t)l->tail->next_prev^(uintptr_t)l->head);

	free(oldhead);
}
void remove_tail(list *l)
{
	node *oldtail = l->tail;

	l->tail = (node *)((uintptr_t)oldtail->next_prev^(uintptr_t)l->head);

	l->tail->next_prev = (node *)((uintptr_t)l->tail->next_prev^(uintptr_t)oldtail);
	l->tail->next_prev = (node *)((uintptr_t)l->tail->next_prev^(uintptr_t)l->head);

	l->head->next_prev = (node *)((uintptr_t)l->head->next_prev^(uintptr_t)oldtail);
	l->head->next_prev = (node *)((uintptr_t)l->head->next_prev^(uintptr_t)l->tail);
	free(oldtail);
}

void for_each(node *from, node *to, void (*func)(node *n, void *a), void *a)
{
	node *it = from;
	node *prev = to;
	do{
		node *tmp=it;
		it=(node *)((uintptr_t)it->next_prev^(uintptr_t)prev);
		prev=tmp;

		func(tmp,a);
	}while(it!=from);
}

void for_each_forward(list *l, void (*func)(node *n, void *a), void *a)
{
	for_each(l->head, l->tail, func, a);
}

void for_each_backward(list *l, void (*func)(node *n, void *a), void *a)
{
	for_each(l->tail, l->head, func, a);
}


void print_node(node *n, void *not_used)
{
	printf("%d\n", n->data);
}

void free_node(node *n, void *not_used)
{
	free(n);
}

void add_node(node *n, void *a)
{
	int *add=(int*)a;
	n->data+=*add;
}

int main()
{
	list l;
	init(&l);

	printf("Inserting 3, 4, 1, 5, 6 and 2 at the back of the list.\n");
	insert_back(&l, 3);
	insert_back(&l, 4);
	insert_back(&l, 1);
	insert_back(&l, 5);
	insert_back(&l, 6);
	insert_back(&l, 2);
	for_each_forward(&l, print_node, NULL);

	printf("Print list backwards.\n");
	for_each_backward(&l, print_node, NULL);

	printf("Inserting 5 in front of the list.\n");
	insert_front(&l, 5);
	for_each_forward(&l, print_node, NULL);

	printf("Removing tail.\n");
	remove_tail(&l);
	for_each_forward(&l, print_node, NULL);

	printf("Removing head.\n");
	remove_head(&l);
	for_each_forward(&l, print_node, NULL);

	printf("Add 5 to each element in the list.\n");
	int add=5;
	for_each_forward(&l, add_node, &add);
	for_each_forward(&l, print_node, NULL);

	printf("Freeing list.\n");
	for_each_forward(&l, free_node, NULL);
	return 0;
}

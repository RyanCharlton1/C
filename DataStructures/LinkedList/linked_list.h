#ifndef LINKEDLISTRYAN
#define LINKEDLISTRYAN

typedef struct Node{
    struct Node *prev, *next;
    void *data;
} Node;

typedef struct{
    Node *head, *tail;
} LinkedList;

typedef LinkedList Queue;
typedef LinkedList Stack;

Node* init_node();
LinkedList* init_linkedlist();

void free_node(Node*);
void free_linkedlist(LinkedList*);


void prepend_list(LinkedList*, void*);
void append_list(LinkedList*, void*);

void remove_head(LinkedList*);
void remove_tail(LinkedList*);

void insert_before(LinkedList*, Node*, void*);
void insert_after(LinkedList*, Node*, void*);

void push_queue(Queue*, void*);
void* pop_queue(Queue*);

void push_stack(Stack*, void*);
void* pop_stack(Stack*);

unsigned int length_linkedlist(LinkedList*);
LinkedList* combine_linkedlist(LinkedList*, LinkedList*);
LinkedList* quicksort(LinkedList*, int(void*, void*));

#endif
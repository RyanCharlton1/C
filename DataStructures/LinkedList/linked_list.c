#include <stdlib.h>
#include <stdio.h>

#include "DataStructures/LinkedList/linked_list.h"

Node* init_node(){
    Node* n = (Node*)malloc(sizeof(Node));
    n->prev = NULL;
    n->next = NULL;
    n->data = NULL;
    return n;
}

LinkedList* init_linkedlist(){
    LinkedList* ll = (LinkedList*)malloc(sizeof(LinkedList));
    ll->head = NULL;
    ll->tail = NULL;
    return ll;
}

void free_node(Node* n){
    free(n);
}
//will free all nodes contained but not node data
void free_linkedlist(LinkedList* ll){
    while(ll->head){
        remove_head(ll);
    }
    free(ll);
}

void prepend_list(LinkedList *ll, void *data){
    Node *n = init_node();
    n->data = data;
    n->next = ll->head;
    if(n->next)
        n->next->prev = n;
    ll->head = n;
    if(ll->tail == NULL)
        ll->tail = n;
}

void append_list(LinkedList *ll, void *data){
    Node *n = init_node();
    n->data = data;
    n->prev = ll->tail;
    if(n->prev)
        n->prev->next = n;
    ll->tail = n;
    if(ll->head == NULL)
        ll->head = n;
}

void remove_head(LinkedList *ll){
    if(ll->head == NULL)
        return;

    if(ll->head == ll->tail){
        free_node(ll->head);
        ll->head = NULL;
        ll->tail = NULL;
    }
    else{
        Node *n = ll->head->next;
        free_node(ll->head);
        ll->head = n;
        n->prev = NULL;
    }
}

void remove_tail(LinkedList *ll){
    if(ll->tail == NULL)
        return;
    
    if(ll->head == ll->tail){
        free_node(ll->tail);
        ll->head = NULL;
        ll->tail = NULL;
    }
    else{
        Node *n = ll->tail->prev;
        free_node(ll->tail);
        ll->tail = n;
        n->next = NULL;
    }
}

void insert_before(LinkedList* ll, Node* n, void* data){
    if(n == NULL)
        return;
    if(n == ll->head){
        prepend_list(ll, data);
        return;
    }

    Node *n2 = init_node();
    n2->data = data;
    n2->next = n;
    n2->prev = n->prev;
    if(n->prev)
        n->prev->next = n2;
    n->prev = n2;

}

void insert_after(LinkedList *ll, Node *n, void* data){
    if(n == NULL)
        return;
    if(n == ll->tail){
        prepend_list(ll, data);
        return;
    }

    Node *n2 = init_node();
    n2->data = data;
    n2->next = n->next;
    n2->prev = n;
    if(n->next)
        n->next->prev = n2;
    n->next = n2;
}

void push_queue(Queue *q, void *data){
    append_list(q, data);
}

void* pop_queue(Queue *q){
    void *data = q->head->data;
    remove_head(q);
    return data;
}

void push_stack(Stack* s, void *data){
    prepend_list(s, data);
}

void* pop_stack(Stack *s){
    void *data = s->head->data;
    remove_head(s);
    return data;
}

unsigned int length_linkedlist(LinkedList *ll){
    int length = 0;
    for(Node *n = ll->head; n != NULL; n = n->next)
        length++;

    return length;
}

LinkedList* combine_linkedlist(LinkedList* ll1, LinkedList* ll2){
    LinkedList *ll = init_linkedlist();
    for(Node *n = ll1->head; n != NULL; n = n->next)
        append_list(ll, n->data);

    for(Node *n = ll2->head; n != NULL; n = n->next)
        append_list(ll, n->data);

    return ll;
}

LinkedList* quicksort(LinkedList *ll, int compare(void*, void*)){
    int length = length_linkedlist(ll);

    if(length < 1){
        return init_linkedlist();
    }

    if(length == 1){
        LinkedList *sorted = init_linkedlist();
        append_list(sorted, ll->head->data);
        return sorted;
    }
    
    void* inplace = ll->head->data;
    LinkedList *less = init_linkedlist();
    LinkedList *more = init_linkedlist();

    for(Node *n = ll->head->next; n != NULL; n = n->next){
        if(compare(inplace, n->data) > 0)
            prepend_list(less, n->data);
        else
            prepend_list(more, n->data);
    }

    LinkedList *lesscpy = less;
    LinkedList *morecpy = more;

    less = quicksort(less, compare);
    more = quicksort(more, compare);

    append_list(less, inplace);
    LinkedList *sorted = combine_linkedlist(less, more);

    free_linkedlist(less);
    free_linkedlist(more);
    free_linkedlist(lesscpy);
    free_linkedlist(morecpy);
    return sorted;
}
/*
int compare(void *ptr1, void *ptr2){
    return ptr1 - ptr2;
}

int main(){
    LinkedList *ll = init_linkedlist();
    LinkedList *ll2 = init_linkedlist();

    int arr[] = {9,12,2,42,1,532,2,35,2,0,32,4,2,7,4,2};

    for(int i = 0; i < 5; i++){
        append_list(ll, arr[i]);
    }

    for(int i = 0; i < 5; i++){
        append_list(ll2, arr[7 + i]);
    }
    
    LinkedList *ll3 = combine_linkedlist(ll,ll2);

    for(Node *n = ll3->head; n != NULL; n = n->next)
        printf("%d, ", n->data);
    putchar('\n');

    LinkedList *ll4 = quicksort(ll3, compare);

    for(Node *n = ll4->head; n != NULL; n = n->next)
        printf("%d, ", n->data);
    putchar('\n');

    free_linkedlist(ll);
    free_linkedlist(ll2);
    free_linkedlist(ll3);
    free_linkedlist(ll4);
}
*/

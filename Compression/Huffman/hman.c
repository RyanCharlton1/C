#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "DataStructures/BinaryTree/binary_tree.h"
#include "DataStructures/BitArray/bit_array.h"
#include "DataStructures/LinkedList/linked_list.h"

#define EOT 4

int compare(void *n1, void *n2)
{
    return ((WeightedLeaf *)n1)->weight - ((WeightedLeaf *)n2)->weight;
}

void heapify(char *heap, int i, WeightedLeaf *wl)
{
    heap[i] = (char)wl->data;

    if (wl->left)
        heapify(heap, 2 * i, (WeightedLeaf *)wl->left);
    if (wl->right)
        heapify(heap, 2 * i + 1, (WeightedLeaf *)wl->right);
}

char *index_to_string(int index)
{
    int order = (int)log2(index);
    char *str = (char *)malloc(order * sizeof(char));

    for (int i = 1; index != 1; i++)
    {
        if (index % 2 != 0)
            str[order - i] = '1';
        else
            str[order - i] = '0';

        index /= 2;
    }

    str[order] = '\0';
    return str;
}

void write_heapchar_bitarr(BitArray *ba, char c, char *heap)
{
    int index;
    for (index = 1; heap[index] != c; index++)
    {
    }

    char *symbol = index_to_string(index);
    for (int j = 0; j < strlen(symbol); j++)
    {
        if (symbol[j] == '0')
            write_bitarr(ba, 0);
        else
            write_bitarr(ba, 1);
    }
    free(symbol);
}

void write_heap_bitarr(BitArray *ba, int i, char* heap){
    if(heap[i]){
        write_bitarr(ba, 1);
        write_char_bitarr(ba, heap[i]);
    }
    else{
        write_bitarr(ba, )
    }
}

int main(int argc, char **argv)
{
    // char* str = argv[1];
    char *str = "A paragraph is a self-contained unit of discourse.jdoiwafisniwuaehfupnweiniawuehfpvnspaibvuiawafnspiewhfionepibvuipawenvpis";
    // count chars in input
    int *count = (int *)calloc(256, sizeof(int));
    for (int i = 0; i < strlen(str); i++)
        count[str[i]]++;

    // EOT marker
    count[EOT] = 1;

    int leaf_count = 0;
    // create leaves and put them in queue
    LinkedList *leaves = init_linkedlist();
    for (int i = 0; i < 256; i++)
    {
        if (count[i])
        {
            WeightedLeaf *lf = init_weightedleaf();
            lf->data = i;
            lf->weight = count[i];

            push_queue(leaves, lf);
            leaf_count++;
        }
    }

    // sort queue
    LinkedList *sorted = quicksort(leaves, compare);

    WeightedLeaf *wl;

    // build expression tree
    LinkedList *tmp;
    WeightedLeaf *wl1, *wl2;
    while (sorted->head != sorted->tail)
    {
        wl1 = (WeightedLeaf *)pop_queue(sorted);
        wl2 = (WeightedLeaf *)pop_queue(sorted);

        wl = init_weightedleaf();
        wl->data = NULL;
        wl->weight = wl1->weight + wl2->weight;
        wl->left = wl1;
        wl->right = wl2;

        push_queue(sorted, wl);
        sorted = quicksort(sorted, compare);

        leaf_count++;
    }

    // make heap
    unsigned short int heapsize = (int)pow(2, depth_tree(sorted->head->data));
    char *heap = (char *)calloc(heapsize, sizeof(char));
    
    heapify(heap, 1, (WeightedLeaf *)sorted->head->data);
    heap[0] = heapsize;
    // write heap to bit array
    

    // encode
    for (int i = 0; i < strlen(str); i++)
    {
        write_heapchar_bitarr(ba, str[i], heap);
    }
    // add EOF
    write_heapchar_bitarr(ba, EOT, heap);
    save_file(ba, "encoded.txt");

    free(heap);
    free_bitarr(ba);
    free_linkedlist(leaves);
    free_linkedlist(sorted);

    // decode
    BitArray *decode = init_bitarr();
    open_file(decode, "encoded.txt");

    heapsize = read_char_bitarr(decode);
    heap = (char *)calloc(heapsize, sizeof(char));

    heap[0] = heapsize;
    for (int i = 1; i < heapsize; i++)
        if (read_bitarr(decode))
            heap[i] = read_char_bitarr(decode);

    int messagelength = 1;
    char *message = (char *)malloc(sizeof(char));

    char c;
    do
    {
        // get char from heap
        int i = 1;

        while (!(c = heap[i]))
        {
            if (read_bitarr(decode))
            {
                i = 2 * i + 1;
            }
            else
            {
                i = 2 * i;
            }
        }

        message[messagelength - 1] = c;
        messagelength++;
        message = (char *)realloc(message, messagelength * sizeof(char));
    } while (c != EOT);

    message[messagelength - 1] = '\0';

    FILE *decodefile = fopen("decode.txt", "w");
    fwrite(message, strlen(message), 1, decodefile);
    fclose(decodefile);
    putchar('\n');

    free(message);
    free_bitarr(decode);
    return 0;
}
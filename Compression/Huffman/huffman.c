#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Compression/Huffman/huffman.h"

#include "DataStructures/BinaryTree/binary_tree.h"
#include "DataStructures/BitArray/bit_array.h"
#include "DataStructures/LinkedList/linked_list.h"

#include "Maths/ints.h"

#define EOT 4
#define LEFT 2
#define RIGHT 1
#define LEAF (void*)256

int compare(void *n1, void *n2)
{
    return ((WeightedLeaf *)n1)->weight - ((WeightedLeaf *)n2)->weight;
}

void write_tree_bitarr(BitArray *ba, WeightedLeaf *wl){
    if(wl->data != LEAF){
        write_bitarr(ba, 1);
        write_char_bitarr(ba, (char)wl->data);
    }
    else{
        write_bitarr(ba, 0);
        write_tree_bitarr(ba, wl->left);
        write_tree_bitarr(ba, wl->right);
    }
}

void create_path_table(unsigned long int *table, unsigned long int path, WeightedLeaf *leaf){
    if(leaf->data != LEAF){
        char c = (char)leaf->data;
        table[c] = path;
    }
    else{
        create_path_table(table, path * 10 + LEFT, leaf->left);
        create_path_table(table, path * 10 + RIGHT, leaf->right);
    }
}

void read_tree_bitarr(BitArray *ba, Leaf *l){
    if(read_bitarr(ba)){
        l->data = read_char_bitarr(ba);
    } 
    else{
        l->left = init_leaf();
        l->right = init_leaf();
        read_tree_bitarr(ba, l->left);
        read_tree_bitarr(ba, l->right);
    }
}

void print_tree(Leaf *l){
    if(l->data != LEAF){
        print_tree(l->left);
        printf("/A\\");
        print_tree(l->right);
    }
    else{
        printf("%c", l->data);
    }
}

void print_wtree(WeightedLeaf *l){
    if(l->data != LEAF){
        print_wtree(l->left);
        printf("/%d\\", l->weight);
        print_wtree(l->right);
    }
    else{
        printf("%c", l->data);
    }
}

unsigned int log10i(unsigned long int x){
    unsigned int i = 0;
    while(x){
        x /= 10;
        i++;
    }

    return i - 1;
}

//string must 0 teminated string
char* huffman_encode(const char *str, unsigned long int strlen, unsigned long int *len){
    // count chars in input
    unsigned long int *count = (unsigned long int*)calloc(256, sizeof(unsigned long int));
    for (unsigned long int i = 0; i < strlen; i++)
        count[str[i]]++;
    
    // EOT marker
    count[EOT] = 1;

    unsigned int leaf_count = 0;
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
    WeightedLeaf *wl1, *wl2;
    LinkedList *temp;
    while (sorted->head != sorted->tail)
    {
        wl1 = (WeightedLeaf*)pop_queue(sorted);
        wl2 = (WeightedLeaf*)pop_queue(sorted);

        wl = init_weightedleaf();
        wl->data = LEAF;
        wl->weight = wl1->weight + wl2->weight;
        wl->left = wl1;
        wl->right = wl2;

        push_queue(sorted, wl);
        //quicksort creates new sorted list
        temp = quicksort(sorted, compare);
        free_linkedlist(sorted);
        sorted = temp;

        leaf_count++;
    }

    //write tree to bit array
    BitArray *encode = init_bitarr();
    write_tree_bitarr(encode, (WeightedLeaf*)sorted->head->data);

    //encode str to bit array
    unsigned long int *path_table = calloc(256, sizeof(unsigned long int));
    create_path_table(path_table, 0, (WeightedLeaf*)sorted->head->data);

    for (unsigned long int i = 0; i < strlen; i++)
    {
        unsigned long int path = path_table[str[i]];
        while(path){
            int side = path / powi(10, (unsigned int)log10i(path));

            if(side == LEFT)
                write_bitarr(encode, 0);
            else
                write_bitarr(encode, 1);
            
            path -= (unsigned long int)side * powi(10, (int)log10i(path));
        }
    }

    // add EOF
    unsigned long int path = path_table[EOT];
    while(path){
        int side = path / powi(10, (unsigned int)log10i(path));
        
        if(side == LEFT)
            write_bitarr(encode, 0);
        else
            write_bitarr(encode, 1);
        
        path -= (unsigned long int)side * powi(10, (int)log10i(path));
    }
    
    pad_bitarr(encode);
    write_char_bitarr(encode, '\0');
    char *ptr = encode->data;

    *len = encode->bytecount;

    free_weighted_tree((WeightedLeaf*)sorted->head->data);
    free_linkedlist(leaves);
    free_linkedlist(sorted);
    free(path_table);
    free(encode);
    free(count);
    return ptr;
}

char* huffman_decode(const char *str, unsigned int msglen){
    BitArray *decode = init_bitarr_string(str, msglen);

    Leaf *root = init_leaf();
    read_tree_bitarr(decode, root);  

    unsigned long int messagelength = 1;
    char *message = (char *)malloc(sizeof(char));

    char c;
    do
    {
        // get char from tree
        Leaf *target = root;
        while (!(c = (char)target->data))
        {
            if (read_bitarr(decode))
            {
                target = target->right;
            }
            else
            {
                target = target->left;
            }
        }

        message[messagelength - 1] = c;
        messagelength++;
        message = (char *)realloc(message, messagelength * sizeof(char));
    } while (c != EOT && decode->bitptr / 8 < decode->bytecount); 

    message[messagelength - 2] = '\0';

    free_bitarr(decode);
    free_tree(root);
    return message;
}

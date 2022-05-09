#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Compression/Huffman/huffman.h"

#include "DataStructures/BinaryTree/binary_tree.h"
#include "DataStructures/BitArray/bit_array.h"
#include "DataStructures/LinkedList/linked_list.h"

#define EOT 4
#define LEFT 2
#define RIGHT 1


int compare(void *n1, void *n2)
{
    return ((WeightedLeaf *)n1)->weight - ((WeightedLeaf *)n2)->weight;
}

void write_tree_bitarr(BitArray *ba, WeightedLeaf *wl){
    if(wl->data){
        write_bitarr(ba, 1);
        write_char_bitarr(ba, (char)wl->data);
    }
    else{
        write_bitarr(ba, 0);
        write_tree_bitarr(ba, wl->left);
        write_tree_bitarr(ba, wl->right);
    }
}

void create_path_table(unsigned int *table, int path, WeightedLeaf *leaf){
    if(leaf->data){
        table[(char)leaf->data] = path;
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
    if(!(l->data)){
        print_tree(l->left);
        printf("/A\\");
        print_tree(l->right);
    }
    else{
        printf("%c", l->data);
    }
}

void print_wtree(WeightedLeaf *l){
    if(!(l->data)){
        print_wtree(l->left);
        printf("/%d\\", l->weight);
        print_wtree(l->right);
    }
    else{
        printf("%c", l->data);
    }
}

char* huffman_encode(const char *str){
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
    WeightedLeaf *wl1, *wl2;
    LinkedList *temp;
    while (sorted->head != sorted->tail)
    {
        wl1 = (WeightedLeaf*)pop_queue(sorted);
        wl2 = (WeightedLeaf*)pop_queue(sorted);

        wl = init_weightedleaf();
        wl->data = NULL;
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
    unsigned int *path_table = calloc(256, sizeof(unsigned int));
    create_path_table(path_table, 0, (WeightedLeaf*)sorted->head->data);

    for (int i = 0; i < strlen(str); i++)
    {
        int path = path_table[str[i]];
        while(path){
            int side = path / pow(10, (int)log10(path));

            if(side == LEFT)
                write_bitarr(encode, 0);
            else
                write_bitarr(encode, 1);
            
            path -= side * pow(10, (int)log10(path));
        }
    }

    // add EOF
    int path = path_table[EOT];
    while(path){
        int side = path / pow(10, (int)log10(path));
        
        if(side == LEFT)
            write_bitarr(encode, 0);
        else
            write_bitarr(encode, 1);
        
        path -= side * pow(10, (int)log10(path));
    }
    
    pad_bitarr(encode);
    write_char_bitarr(encode, '\0');
    char *ptr = encode->data;
    free_weighted_tree((WeightedLeaf*)sorted->head->data);
    free_linkedlist(leaves);
    free_linkedlist(sorted);
    free(path_table);
    free(encode);
    free(count);

    return ptr;
}

char* huffman_decode(const char *str){
    BitArray *decode = init_bitarr_string(str);

    Leaf *root = init_leaf();
    read_tree_bitarr(decode, root);  

    int messagelength = 1;
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
    } while (c != EOT); 

    message[messagelength - 2] = '\0';

    free_bitarr(decode);
    free_tree(root);
    return message;
}
/*
int main(){
    const char* str = "";
    char *encoded = huffman_encode(str);
    char *decoded = huffman_decode(encoded);

    printf("%d:%s\n", strlen(encoded), encoded);
    printf("%d:%s\n", strlen(decoded), decoded);

    free(encoded);
    free(decoded);
    return 0;
}
*/
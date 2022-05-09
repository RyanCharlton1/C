#include <stdlib.h>

#include "DataStructures/BinaryTree/binary_tree.h"

Leaf* init_leaf(){
    return (Leaf*)calloc(1, sizeof(Leaf));
}

WeightedLeaf* init_weightedleaf(){
    return (WeightedLeaf*)calloc(1, sizeof(WeightedLeaf));
}

void free_leaf(Leaf* l){
    free(l);
}

void free_weightedleaf(WeightedLeaf* l){
    free(l);
}

void free_weighted_tree(WeightedLeaf *l){
    if(l->left)
        free_weighted_tree(l->left);
    
    if(l->right)
        free_weighted_tree(l->right);

    free_weightedleaf(l);
}

void free_tree(Leaf *l){
    if(l->left)
        free_tree(l->left);
    
    if(l->right)
        free_tree(l->right);

    free_leaf(l);
}

int depth_tree(WeightedLeaf *wl){
    if(wl == NULL)
        return 0;

    int depthleft = depth_tree(wl->left);
    int depthright = depth_tree(wl->right);

    if(depthleft > depthright){
        return depthleft + 1;
    } else{
        return depthright + 1;
    }
}

void find(WeightedLeaf *root, void *data){
    

}

//int main(){
//    return 0;
//}
#ifndef BINARYTREERYAN
#define BINARYTREERYAN


typedef struct {
    void *data;
    struct Leaf *left, *right;
} Leaf;

typedef struct {
    void *data;
    int weight;
    struct WeightedLeaf *left, *right;
} WeightedLeaf;

Leaf* init_leaf();
WeightedLeaf* init_weightedleaf();

void free_leaf(Leaf*);
void free_weightedleaf(WeightedLeaf*);
void free_tree(Leaf*);
void free_weighted_tree(WeightedLeaf*);
int depth_tree(WeightedLeaf*);

void print_weightedtree(WeightedLeaf*, void(void*));
void find(WeightedLeaf*, void*);

#endif
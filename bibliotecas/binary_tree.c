#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct binary_tree binary_tree;
struct binary_tree
{
    void *item;
    binary_tree *left;
    binary_tree *right;
};

binary_tree *create_binary_tree(void *item, binary_tree *left, binary_tree *right)
{
    binary_tree *new_binary_tree = (binary_tree *)malloc(sizeof(binary_tree));
    new_binary_tree->item = item;
    new_binary_tree->left = left;
    new_binary_tree->right = right;
    return new_binary_tree;
}
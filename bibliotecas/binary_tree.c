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

/**
 * @brief Aloca o espaço de memória e cria uma árvore binária.
 *
 * @param item O ítem do node.
 * @param left Ponteiro do tipo binary_tree que aponta para a o node a esquerda.
 * @param right Ponteiro do tipo binary_tree que aponta para a o node a direita.
 * @return Retorna um ponteiro do tipo binary_tree que aponta para a nova árvore.
 */
binary_tree *create_binary_tree(void *item, binary_tree *left, binary_tree *right)
{
    binary_tree *new_binary_tree = (binary_tree *)malloc(sizeof(binary_tree));
    new_binary_tree->item = item;
    new_binary_tree->left = left;
    new_binary_tree->right = right;
    return new_binary_tree;
}
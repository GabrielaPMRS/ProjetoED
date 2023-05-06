#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bibliotecas/linked_list.c"
#include "bibliotecas/auxiliares.c"
#include "bibliotecas/binary_tree.c"

/**
 * @brief Define o tamanho do arquivo comprimido.
 *
 * @param compressed_file Ponteiro do tipo FILE que aponta para o arquivo comprimido.
 * @return Retorna o tamanho do arquivo comprimido.
 */
int define_file_size(FILE *compressed_file)
{
    int file_size = 0;
    unsigned char byte;
    while (fread(&byte, sizeof(unsigned char), 1, compressed_file) > 0)
    {
        file_size++;
    }
    return file_size;
}

/**
 * @brief Define o tamanho da árvore e o tamnho do lixo.
 *
 * @param compressed_file Ponteiro do tipo FILE que aponta para o arquivo comprimido.
 * @param tree_size Ponteiro do tipo inteiro para o tamanho da árvore.
 * @param trash_size Ponteiro do tipo inteiro para o tamanho do lixo.
 * @return void.
 */
void define_treesize_trashsize(FILE *compressed_file, int *tree_size, int *trash_size)
{
    unsigned char first_byte;
    fread(&first_byte, sizeof(unsigned char), 1, compressed_file);
    unsigned char second_byte;
    fread(&second_byte, sizeof(unsigned char), 1, compressed_file);
    int i;
    *trash_size = first_byte >> 5;

    i = 4;
    *tree_size = 0;
    while (i > -1)
    {
        *tree_size = 2 * (*tree_size) + is_bit_i_set(first_byte, i);
        i--;
    }
    i = 7;
    while (i > -1)
    {
        *tree_size = 2 * (*tree_size) + is_bit_i_set(second_byte, i);
        i--;
    }
}

/**
 * @brief Função recursiva que reconstroi a árvore a partir da árvore em pré ordem.
 *
 * @param pre_order_tree Array contendo a árvore em pré ordem.
 * @param i_tree Ponteiro do tipo inteiro que aponta para o índice que percorre o array.
 * @param tree_size Tamanho da árvore.
 * @return Retorna um ponteiro do tipo binary_tree que aponta para a árvore reconstruida.
 */
binary_tree *recreate_tree(unsigned char pre_order_tree[], int *i_tree, int tree_size)
{
    binary_tree *tree;
    unsigned char item = pre_order_tree[*i_tree];

    if (*i_tree == tree_size)
    {
        return tree;
    }

    // node interno
    if (item == '*')
    {
        tree = create_binary_tree(NULL, NULL, NULL);
        *i_tree += 1;
        tree->left = recreate_tree(pre_order_tree, i_tree, tree_size);
        tree->right = recreate_tree(pre_order_tree, i_tree, tree_size);
    }
    // caso ache um escape
    else if (item == '\\')
    {
        tree = create_binary_tree((void *)&pre_order_tree[*i_tree + 1], NULL, NULL);
        *i_tree += 2;
    }
    else
    {
        tree = create_binary_tree((void *)&pre_order_tree[*i_tree], NULL, NULL);
        *i_tree += 1;
    }

    return tree;
}

/**
 * @brief Gera o arquivo descomprimido.
 *
 * @param compresses_file Ponteiro do tipo FILE que aponta para o arquivo comprimido.
 * @param tree Ponteiro do tipo binary_tree que aponta para a árvore reconstruida.
 * @param file_size Tamanho do arquivo comprimido.
 * @param tree_size Tamanho da árvore.
 * @param trash_size Tamanho do lixo.
 * @return void.
 */
void unzip_to_file(FILE *compresses_file, binary_tree *tree,
                   int file_size, int tree_size, int trash_size)
{
    int j, k;
    int bits_to_read = 0;
    int compressed_size = file_size - 2 - tree_size;

    FILE *uf = fopen("unzipped_file", "wb");

    binary_tree *aux = tree;

    for (j = 0; j < compressed_size; j++)
    {
        unsigned char byte;
        fread(&byte, sizeof(unsigned char), 1, compresses_file);

        if (j == (compressed_size - 1))
        {
            bits_to_read = trash_size;
        }

        for (k = 7; k >= bits_to_read; k--)
        {

            int bit = is_bit_i_set(byte, k);

            if (bit == 1)
            {
                aux = aux->right;
            }
            else
            {
                aux = aux->left;
            }

            if (aux->right == NULL && aux->left == NULL)
            {
                fwrite((unsigned char *)aux->item, sizeof(unsigned char), 1, uf);
                aux = tree;
            }
        }
    }

    fclose(uf);
}

int main()
{
    char filename[100];
    printf("entre com o nome do arquivo a ser descomprimido: ");
    scanf("%s", filename);

    FILE *compressed_file = fopen(filename, "rb");
    int file_size = define_file_size(compressed_file);
    fclose(compressed_file);

    compressed_file = fopen(filename, "rb");

    int trash_size, tree_size;
    define_treesize_trashsize(compressed_file, &tree_size, &trash_size);

    unsigned char pre_order_tree[tree_size];
    fread(pre_order_tree, sizeof(unsigned char), tree_size, compressed_file);

    int i_tree = 0;
    binary_tree *tree = recreate_tree(pre_order_tree, &i_tree, tree_size);

    unzip_to_file(compressed_file, tree, file_size, tree_size, trash_size);

    fclose(compressed_file);
    printf("arquivo descomprimido em 'unzipped_file'\n");
}
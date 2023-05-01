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

int is_bit_i_set(unsigned char c, int i)
{
    unsigned char mask = 1 << i;
    unsigned char res = mask & c;
    return res != 0;
}

int define_file_size(FILE *compressed_file)
{
    int file_size = 0;
    unsigned char byte;
    while (fread(&byte, sizeof(unsigned char), 1, compressed_file) > 0)
    {
        file_size++;
        // printf("byte= %c file size= %d\n", byte, file_size);
    }
    printf("file size = %d\n", file_size);
    return file_size;
}

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
    printf("trash size = %d  tree size = %d\n", *trash_size, *tree_size);
}

binary_tree *recreate_tree(unsigned char pre_order_tree[], int *i_tree, int tree_size)
{
    binary_tree *tree;
    unsigned char item = pre_order_tree[*i_tree];
    // printf("hahaha\n");

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

void print_pre_order(binary_tree *tree)
{
    if (tree != NULL)
    {
        if (tree->item == NULL)
        {
            printf("%c ", '*');
        }
        else if (*((unsigned char *)tree->item) == '*')
        {
            printf("\\*");
        }
        else if (*((unsigned char *)tree->item) == '\\')
        {
            printf("\\\\");
        }
        else
        {
            printf("%c ", *((unsigned char *)tree->item));
        }
        print_pre_order(tree->left);
        print_pre_order(tree->right);
    }
}

void unzip_to_file(FILE *compresses_file, binary_tree *tree, int file_size, int tree_size, int trash_size)
{
    int j, i, k;
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

            int res = is_bit_i_set(byte, k);

            // printf("%d ", res);
            if (res == 1)
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
    printf("entre com o nome do arquivo a ser descomprimido \n");
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
    print_pre_order(tree);
    printf("\n");
    unzip_to_file(compressed_file, tree, file_size, tree_size, trash_size);

    fclose(compressed_file);
    printf("arquivo descomprimido em 'unzipped_file'\n");
}
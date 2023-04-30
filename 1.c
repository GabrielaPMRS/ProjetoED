#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

typedef struct list_node list_node;
struct list_node
{
    int valor;
    list_node *next;
};

typedef struct linked_list linked_list;
struct linked_list
{
    list_node *head;
    list_node *tail;
    int size;
};

typedef struct byte_info byte_info;
struct byte_info
{
    unsigned char byte;
    int frequencia;
    linked_list *bits;
};

typedef struct Huff_node Huff_node;
struct Huff_node
{
    int frequencia;
    unsigned char byte;
    Huff_node *next;
    Huff_node *left;
    Huff_node *right;
};

typedef struct Huff Huff;
struct Huff
{
    Huff_node *head;
};

unsigned char set_bit(unsigned char c, int i)
{
    unsigned char mask = 1 << i;
    return mask | c;
}

void header()
{
    int trash_size = 5;
    // int tree_size = PreOrderTree->size;
    int tree_size = 500;
    unsigned char header[2] = {0};

    int i = 5;

    while (trash_size != 0)
    {
        if (trash_size % 2 == 1)
        {
            header[0] = set_bit(header[0], i);
        }

        i++;
        trash_size = trash_size / 2;
    }

    i = 0;

    while (tree_size != 0)
    {
        if (i > 7)
        {
            break;
        }
        if (tree_size % 2 == 1)
        {
            header[1] = set_bit(header[1], i);
        }

        i++;
        tree_size = tree_size / 2;
    }

    if (tree_size > 0)
    {
        i = 0;
        while (tree_size != 0)
        {
            if (i > 4)
            {
                break;
            }
            if (tree_size % 2 == 1)
            {
                header[0] = set_bit(header[0], i);
            }
            i++;
            tree_size = tree_size / 2;
        }
    }

    printf("%d %d\n", header[0], header[1]);
    FILE *hd = fopen("ha.txt", "wb");
    fwrite(header, sizeof(unsigned char), 2, hd);
    fclose(hd);
}

int main()
{
    header();
    return 0;
}
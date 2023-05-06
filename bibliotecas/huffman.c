#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Huff_node Huff_node;
struct Huff_node
{
    int frequencia;
    void *byte;
    Huff_node *next;
    Huff_node *left;
    Huff_node *right;
};

typedef struct Huff Huff;
struct Huff
{
    Huff_node *head;
};

int is_empty(Huff *huff)
{
    return huff->head == NULL;
}

Huff *create_huff_queue()
{
    Huff *new_queue = (Huff *)malloc(sizeof(Huff));
    new_queue->head = NULL;
    return new_queue;
}

void enqueue(Huff *huff, void *valor, int frequencia, Huff_node *left, Huff_node *right)
{
    Huff_node *new_node = (Huff_node *)malloc(sizeof(Huff_node));
    new_node->byte = valor;
    new_node->frequencia = frequencia;
    new_node->left = left;
    new_node->right = right;

    if (is_empty(huff) || frequencia <= huff->head->frequencia)
    {
        new_node->next = huff->head;
        huff->head = new_node;
    }
    else
    {
        Huff_node *current = huff->head;
        while (current->next != NULL && current->next->frequencia < frequencia)
        {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}

Huff_node *dequeue(Huff *huff)
{
    if (is_empty(huff))
    {
        printf("priority queue overflow\n");
    }
    else
    {
        Huff_node *node = huff->head;
        huff->head = huff->head->next;
        node->next = NULL;
        return node;
    }
}

void print_queue(Huff *huff)
{
    Huff_node *current = huff->head;
    while (current != NULL)
    {
        printf("byte: %c | freq: %d \n", *((unsigned char *)current->byte), current->frequencia);
        current = current->next;
    }
    printf("\n");
}

Huff_node *create_huff_tree(Huff *huff)
{
    while (huff->head->next != NULL)
    {
        int soma_freq = huff->head->frequencia + huff->head->next->frequencia;

        Huff_node *left = dequeue(huff);
        Huff_node *right = dequeue(huff);

        // NULL representa o * dos nodes internos da arvore
        enqueue(huff, NULL, soma_freq, left, right);
    }
    return huff->head;
}

void print_pre_order(Huff_node *tree)
{
    if (tree != NULL)
    {
        if (tree->byte == NULL)
        {
            printf("%c ", '*');
        }
        else if (*((unsigned char *)tree->byte) == '*')
        {
            printf("\\*");
        }
        else if (*((unsigned char *)tree->byte) == '\\')
        {
            printf("\\\\");
        }
        else
        {
            printf("%c ", *((unsigned char *)tree->byte));
        }
        print_pre_order(tree->left);
        print_pre_order(tree->right);
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

linked_list *create_linked_list()
{
    linked_list *list = (linked_list *)malloc(sizeof(linked_list));
    list->head = NULL;
    list->size = 0;
    return list;
}

void add_linked_list(linked_list *list, int valor)
{
    list_node *new_node = (list_node *)malloc(sizeof(list_node));
    new_node->valor = valor;
    new_node->next = list->head;
    list->size += 1;
    list->head = new_node;
}

void add_linked_list_tail(linked_list *list, int valor)
{
    list_node *new_node = (list_node *)malloc(sizeof(list_node));
    new_node->valor = valor;
    list_node *current = list->head;
    list->size += 1;
    if (list->head == NULL)
    {
        list->head = new_node;
        new_node->next = NULL;
    }
    else
    {
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_node;
        new_node->next = NULL;
    }
}

list_node *remove_linked_node(linked_list *bits_list)
{
    list_node *removed_node = bits_list->head;
    bits_list->head = bits_list->head->next;
    free(removed_node);
}

void copy_list(linked_list *aux_list, linked_list *final_list)
{
    list_node *aux = aux_list->head;
    while (aux != NULL)
    {
        add_linked_list(final_list, aux->valor);
        aux = aux->next;
    }
}

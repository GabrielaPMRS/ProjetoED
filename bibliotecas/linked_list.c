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

/**
 * @brief Cria uma lista encadeada.
 *
 * @return Retorna um ponteiro do tipo linked_list que aponta para a lista criada.
 */
linked_list *create_linked_list()
{
    linked_list *list = (linked_list *)malloc(sizeof(linked_list));
    list->head = NULL;
    list->size = 0;
    return list;
}

/**
 * @brief Adiciona um novo node no início da lista encadeada.
 *
 * @param list Ponteiro do tipo linked_list que aponta para a lista que está sendo incrementada.
 * @param valor O ítem adicionado.
 * @return void.
 */
void add_linked_list(linked_list *list, int valor)
{
    list_node *new_node = (list_node *)malloc(sizeof(list_node));
    new_node->valor = valor;
    new_node->next = list->head;
    list->size += 1;
    list->head = new_node;
}

/**
 * @brief Adiciona em novo node no fim da lista encadeada.
 *
 * @param list Ponteiro do tipo linked_list que aponta para a lista que está sendo incrementada.
 * @param valor O ítem adicionado.
 * @return void.
 */
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

/**
 * @brief Remove sempre o primeiro node da lista encadeada.
 *
 * @param list Ponteiro do tipo linked_list que aponta para uma lista encadeada.
 * @return void.
 */
void remove_linked_node(linked_list *list)
{
    list_node *removed_node = list->head;
    list->head = list->head->next;
    list->size--;
    free(removed_node);
}

/**
 * @brief Copia os ítens de uma lista encadeada para outra.
 *
 * @param aux_list Ponteiro do tipo linked_list que aponta para uma lista encadeada auxiliar.
 * @param final_list Ponteiro do tipo linked_list que aponta para a lista encadeada final.
 */
void copy_list(linked_list *aux_list, linked_list *final_list)
{
    list_node *aux = aux_list->head;
    while (aux != NULL)
    {
        add_linked_list(final_list, aux->valor);
        aux = aux->next;
    }
}

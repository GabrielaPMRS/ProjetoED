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

/**
 * @brief Checa se a fila de prioridade está vazia.
 *
 * @param huff Ponteiro do tipo Huff que aponta para a fila de prioridade.
 * @return Retorna 1 se a fila estiver vazia, e 0 se não estiver.
 */
int is_empty(Huff *huff)
{
    return huff->head == NULL;
}

/**
 * @brief Aloca o espaço para uma fila e inicializa ela nula.
 *
 * @return Retorna um ponteiro do tipo Huff para a fila.
 */
Huff *create_huff_queue()
{
    Huff *new_queue = (Huff *)malloc(sizeof(Huff));
    new_queue->head = NULL;
    return new_queue;
}

/**
 * @brief Adiciona na fila de prioridade, de forma crescentes.
 *
 * @param huff Ponteiro do tipo Huff que aponta para uma fila de prioridade.
 * @param valor Ponteiro do tipo void para o ítem a ser adicionado.
 * @param frequencia A frequência do ítem a ser adicionado.
 * @param left Ponteiro do tipo Huff_node que aponta para a esquerda.
 * @param right Ponteiro do tipo Huff_node que aponta para a direita.
 * @return void.
 */
void enqueue(Huff *huff, void *valor, int frequencia,
             Huff_node *left, Huff_node *right)
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

/**
 * @brief Retira sempre o primeiro node da fila.
 *
 * @param huff Ponteiro do tipo Huff que aponta para uma fila de prioridade.
 * @return Retorna um ponteiro do tipo Huff_node que aponta para o node retirado da fila.
 */
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

/**
 * @brief Cria uma árvore binária a partir de uma fila de prioridade.
 *
 * @param huff Ponteiro do tipo Huff que aponta para uma fila de prioridade.
 * @return Retorna um ponteiro do tipo Huff_node que aponta para a raiz da árvore criada.
 */
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

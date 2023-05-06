#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

typedef struct lista
{
    int num;
    struct lista *next;
} lista;

typedef struct arvore
{
    int item;
    struct arvore *esq;
    struct arvore *dir;
} arvore;

lista *add_lista(lista *new, int valor)
{
    lista *novo = (lista *)malloc(sizeof(lista));
    novo->num = valor;
    novo->next = new;
    return novo;
}

arvore *cria_arvore(arvore *nova, int valor)
{
    nova = (arvore *)malloc(sizeof(arvore));
    nova->esq = NULL;
    nova->dir = NULL;
    nova->item = valor;
    return nova;
}

arvore *add_arvore(arvore *raiz, int item)
{
    arvore *no = NULL;
    arvore *atual = NULL;
    int i;
    if (raiz == NULL)
    {
        raiz = cria_arvore(raiz, item);
        return raiz;
    }
    else
    {
        atual = raiz;
        while (atual != NULL)
        {
            if (item <= atual->item)
            {
                if (atual->esq == NULL)
                {
                    atual->esq = cria_arvore(no, item);
                    return raiz;
                }
                else
                {
                    atual = atual->esq;
                }
            }
            else
            {
                if (atual->dir == NULL)
                {
                    atual->dir = cria_arvore(no, item);
                    return raiz;
                }
                else
                {
                    atual = atual->dir;
                }
            }
        }
    }
}

int busca_arv_bin(arvore *raiz, int item)
{
    int cont = 0;
    if (raiz == NULL)
    {
        return 0;
    }

    arvore *atual = raiz;
    while (atual != NULL)
    {
        cont++;
        if (item == atual->item)
        {
            return cont;
        }
        if (item <= atual->item)
        {
            atual = atual->esq;
        }
        else
        {
            atual = atual->dir;
        }
    }
    return 0;
}

int busca_lista(lista *inicio, int item)
{
    int cont = 0;
    if (inicio == NULL)
    {
        return 0;
    }
    lista *atual = inicio;
    while (atual != NULL)
    {
        cont++;
        if (item == atual->num)
        {
            return cont;
        }
        atual = atual->next;
    }
    return 0;
}

int main()
{
    int i, numero_entradas, item, cont_lista, cont_arvore;

    FILE *imput = fopen("entrada.txt", "r");
    FILE *output = fopen("saida.txt", "w");

    if (imput == NULL)
    {
        printf("A entrada nao existe\n");
        return 0;
    }

    // planta uma semente aleatoria para que os numeros selecionados sejam diferentes.
    srand(time(NULL));
    while (fscanf(imput, "%d", &numero_entradas) != EOF)
    {
        arvore *nova_arvore = NULL;
        lista *nova_lista = NULL;

        for (i = 0; i < numero_entradas; i++)
        {
            fscanf(imput, "%d", &item);
            nova_arvore = add_arvore(nova_arvore, item);
            nova_lista = add_lista(nova_lista, item);
        }

        int numero_aleatorio = rand() % numero_entradas;

        cont_arvore = busca_arv_bin(nova_arvore, numero_aleatorio);
        cont_lista = busca_lista(nova_lista, numero_aleatorio);
        fprintf(output, "%d,%d,%d\n", numero_entradas, cont_lista, cont_arvore);
    }
    fclose(imput);
    fclose(output);
    printf("Arquivo de saida em 'saida.txt\n");
    return 0;
}
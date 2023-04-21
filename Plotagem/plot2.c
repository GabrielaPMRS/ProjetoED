#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

typedef struct lista
{
    void *num;
    struct lista *next;
} lista;

lista *cria_elo(lista *new, int valor)
{
    lista *novo = (lista *)malloc(sizeof(lista));
    novo->num = valor;
    novo->next = new;
    return novo;
}

typedef struct arvore
{
    void *item;
    struct arvore *esq;
    struct arvore *dir;
} arvore;

arvore *cria_no(arvore *nova, int valor)
{
    nova = (arvore *)malloc(sizeof(arvore));
    nova->esq = NULL;
    nova->dir = NULL;
    nova->item = valor;
    return nova;
}
arvore *insere(arvore *raiz, int item)
{
    arvore *no = NULL;
    arvore *atual = NULL;
    int i;
    if (raiz == NULL)
    {
        raiz = cria_no(raiz, item);
        return raiz;
    }
    else
    {
        atual = raiz;
        while (atual != NULL)
        {
            // printf("No atual:%d No a ser inserido:%d\n",atual->item,item);
            if (item <= atual->item)
            {
                if (atual->esq == NULL)
                {
                    atual->esq = cria_no(no, item);
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
                    atual->dir = cria_no(no, item);
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

void printa_pre(arvore *no)
{
    if (no == NULL)
    {
        return;
    }
    else
    {
        printf("%d ", no->item);
        printa_pre(no->esq);
        printa_pre(no->dir);
    }
}

void printa_em(arvore *no)
{
    if (no == NULL)
    {
        return;
    }
    else
    {
        printa_em(no->esq);
        printf("%d ", no->item);
        printa_em(no->dir);
    }
}

void printa_pos(arvore *no)
{
    if (no == NULL)
    {
        return;
    }
    else
    {
        printa_pos(no->esq);
        printa_pos(no->dir);
        printf("%d ", no->item);
    }
}

int quant_NO(arvore *no)
{
    if (no == NULL)
    {
        return 0;
    }
    int esq = quant_NO(no->esq);
    int dir = quant_NO(no->dir);
    return (esq + dir + 1);
}

void printa_lista(lista *no)
{
    while (no != NULL)
    {
        printf("%d ", no->num);
        no = no->next;
    }
    printf("\n");
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

    FILE *imput = fopen("entrada4.txt", "r");
    FILE *output = fopen("saida.txt", "w");

    if (imput == NULL)
    {
        printf("A entrada nao existe\n");
        return 0;
    }
    // ler a entrada pelo arquivo
    int cont = 0;
    srand(time(NULL));
    while (fscanf(imput, "%d", &numero_entradas) != EOF)
    {
        cont++;
        arvore *nova_arvore = NULL;
        lista *nova_lista = NULL;

        for (i = 0; i < numero_entradas; i++)
        {
            fscanf(imput, "%d", &item);
            nova_arvore = insere(nova_arvore, item);
            nova_lista = cria_elo(nova_lista, item);
        }
        // printf("Avore Busca Binaria:\n");
        // printa_pre(nova_arvore);
        // printf("\n");
        // printf("Lista:\n");
        // printa_lista(nova_lista);

        // planta uma semente aleatoria para que os numeros selecionados sejam diferentes]

        int numero_aleatorio = rand() % numero_entradas;

        cont_arvore = busca_arv_bin(nova_arvore, numero_aleatorio);
        cont_lista = busca_lista(nova_lista, numero_aleatorio);
        fprintf(output, "%d,%d,%d\n", numero_entradas, cont_lista, cont_arvore);

        // print visual
        printf("entrada de tamanho %d\n", numero_entradas);
        printf("Número selecionado: %d\n", numero_aleatorio);
        printf("Iterações na lista: %d\n", cont_lista);
        printf("Iterações na árvore: %d\n", cont_arvore);
        printf("\n");
    }
    fclose(imput);
    fclose(output);
    printf("contador: %d\n", cont);
    return 0;
}
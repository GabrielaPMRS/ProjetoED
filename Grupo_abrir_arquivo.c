#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

struct byte_info
{
    int frequencia;
};

typedef struct fila
{
    int byte;
    int freq;
    struct fila *next;
    struct fila *esq;
    struct fila *dir;
} fila;

fila* insere_fp (fila *base, fila *novo)
{
    fila *inicio = base;
    fila *corrente = base;
    fila *anterior=NULL;
    while (corrente->next != NULL)
    {
        if(novo->freq > corrente->freq)
        {
            anterior = corrente;
            corrente= corrente->next;
        }
        else
        {
            break;
        }                 
    }
    if (corrente->next==NULL && corrente->freq!=novo->freq)
    {
        corrente->next=novo;
        novo->next=NULL;
        return inicio;
    }
    if (anterior!=NULL)
    {
        anterior->next = novo;
        novo->next=corrente;
        return inicio;         
    }
    else
    {
        novo->next=corrente;
        corrente=novo;
        return corrente;
    }        
}

fila *insere_tree (fila *base)
{
    fila * inicio = base;
    fila * primeiro = base;
    fila * segundo = base->next;
    fila *nova= (fila *) malloc(sizeof(fila));
    nova-> byte = 42;
    nova->freq=primeiro->freq + segundo->freq;
    nova->next=NULL;
    nova->esq=primeiro;
    nova->dir=segundo; // terminada a primeira parte de inserir o novo no (arvore)
    primeiro->next=NULL;
    segundo->next=NULL;
    base = insere_fp(base,nova); //inseri o novo no na fila
}

fila *cria_no (fila *base, struct byte_info ref, int byte)
{
    fila *nova = (fila *)malloc(sizeof(fila));
    nova->byte = byte;
    nova->freq = ref.frequencia;
    nova->next = base;
    nova->esq = NULL;
    nova->dir = NULL;
    return nova;
}
//
fila *ordena(fila *head)
{
    fila *previous;
    fila *next;
    fila *inicio;
    fila *conta;
    conta = head;
    int ant_freq, current_freq, change_freq;
    int ant_byte, current_byte, change_byte;
    inicio = head;
    int i = 0;
    while (conta->next != NULL)
    {
        while (head != NULL)
        {
            ant_freq = head->freq;
            ant_byte = head->byte;
            previous = head;
            next = head->next;
            head = head->next;
            if (head == NULL)
            {
                break;
            }
            current_freq = head->freq;
            current_byte = head->byte;

            if (ant_freq > current_freq)
            {
                change_freq = head->freq;
                change_byte = head->byte;
                head->freq = ant_freq;
                head->byte = ant_byte;
                head = previous;
                head->freq = change_freq;
                head->byte = change_byte;
            }
            head = next;
        }
        head = inicio;
        conta = conta->next;
    }
    return head;
}
//

void imprimir_frequencia(struct byte_info info[])
{
    for (int i = 0; i < 256; i++)
    {
        if (info[i].frequencia > 0)
        {
            printf("Byte: %d, Frequência: %d\n", i, info[i].frequencia);
        }
    }
}

void printa_queue(fila *queue)
{
    if (queue == NULL)
    {
        return;
    }
    while (queue != NULL)
    {
        printf("Byte: %d, Frequencia: %d\n", queue->byte, queue->freq);
        queue = queue->next;
    }
}

fila *cria_no_teste (fila *base)
{
    fila *nova = (fila *)malloc(sizeof(fila));
    nova->byte = 65;
    nova->freq = 3;
    nova->next = NULL;
    nova->esq = NULL;
    nova->dir = NULL;
    return nova;
}

fila *gera_fila (struct byte_info info[])
{
    fila* queue = NULL;
    int cont = 0;
    
    for (int i = 0; i < 256; i++)
    {
        if (info[i].frequencia > 0)
        {
            cont++;
            queue = cria_no(queue, info[i], i);
        }
    } 
    return queue;  
}

fila* abrir(char nome_arquivo[100])
{
    fila *queue = NULL;
    FILE *fp = fopen(nome_arquivo, "rb"); // rb modo binario de leitura.
    if (fp == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return queue;
    }

    int frequencia[256] = {0};
    struct byte_info info[256] = {0};
    unsigned char buffer[1024];
    int n;

    while ((n = fread(buffer, sizeof(unsigned char), sizeof(buffer), fp)) > 0)
    {
        for (int i = 0; i < n; i++)
        {
            frequencia[buffer[i]]++;
            info[buffer[i]].frequencia++;
        }
    }
    fclose(fp);    
    queue = gera_fila(info);
    return queue;
}

int main()
{
    char nome_arquivo[100];
    printf("Digite o nome do arquivo com a extensao: ");
    scanf("%s", nome_arquivo);    
    fila *queue = abrir(nome_arquivo);
    ordena(queue);
    printa_queue(queue);
    fila *no_teste = NULL;
    no_teste= cria_no_teste(no_teste);
    queue = insere_fp (queue , no_teste);
    printf("nova fila gerada:\n");
    printa_queue(queue);
    return 0;
}
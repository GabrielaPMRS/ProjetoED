#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>


struct byte_info
{
    int frequencia;
    long posicao;
};

typedef struct lista
{
    int byte;
    int freq;
    long pos;
    struct lista* next;
} lista;

lista* cria_no (lista* base, struct byte_info ref,int byte)
{
    lista* nova=(lista*)malloc(sizeof(lista));
    nova->byte=byte;
    nova->freq=ref.frequencia;
    nova->pos=ref.posicao;
    nova->next=base;
    return nova;    
}

void imprimir_frequencia(struct byte_info info[])
 {
    for (int i = 0; i < 256; i++) 
    {
        if (info[i].frequencia > 0) 
        {
            printf("Byte: %d, Frequência: %d, Posição: %ld\n", i, info[i].frequencia, info[i].posicao);
        }
    }
}

void printa_list(lista* list)
{
    if (list==NULL)
    {
        return;
    }
    while(list!=NULL)
    {
        printf("Byte: %d, Frequencia: %d, Pos: %ld\n",list->byte,list->freq,list->pos);
        list=list->next;
    }
}

void gera_lista (struct byte_info info[])
 {
    lista* list= NULL;
    for (int i = 0; i < 256; i++)
     {
        if (info[i].frequencia > 0) 
        {
            list = cria_no (list,info[i],i);            
        }
    }  
    printa_list(list);  
}



int abrir (char nome_arquivo[100])
{
    FILE* fp = fopen(nome_arquivo, "rb"); //rb modo binario de leitura.
    if (fp == NULL) 
    {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    int frequencia[256] = {0};
    struct byte_info info[256] = {{0, 0}};
    unsigned char buffer[1024];
    int n;
    long posicao = 0;

    while ((n = fread(buffer, sizeof(unsigned char), sizeof(buffer), fp)) > 0)
     {
        for (int i = 0; i < n; i++) 
        {
            frequencia[buffer[i]]++;
            info[buffer[i]].frequencia++;
            info[buffer[i]].posicao = posicao++;
        }
    }
    fclose(fp);
    //imprimir_frequencia(info);
    gera_lista(info);
    return 0;
}

int main() 
{
    char nome_arquivo[100];
    printf("Digite o nome do arquivo com a extensao: ");
    scanf("%s", nome_arquivo);  
    abrir(nome_arquivo);    
    return 0;
}
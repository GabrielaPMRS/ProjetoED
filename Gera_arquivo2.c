#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#define MAX_BITS 256
struct byte_info
{
    int frequencia;
};

typedef struct fila
{
    unsigned char byte;
    int freq;
    struct fila *next;
    struct fila *esq;
    struct fila *dir;
} fila;

typedef struct conteudo
{
    unsigned char byte;
    char valor[33];
    struct conteudo *prox;
} conteudo;

typedef struct tabela_hash
{    
    int tamanho;
    int ocupados;
    conteudo *primeiro;
    conteudo ** tabela;
} tabela_hash;

tabela_hash* criar_tabela(int tamanho) 
{
    tabela_hash * nova_tabela = (tabela_hash *) malloc(sizeof(tabela_hash));
    nova_tabela->tamanho = tamanho;
    nova_tabela->ocupados = 0;
    nova_tabela->primeiro = NULL;
    nova_tabela->tabela = (conteudo**) malloc(sizeof(conteudo*) * tamanho);
    if (nova_tabela->tabela == NULL) 
    {        
        free(nova_tabela);
        return NULL;
    }
    int i;
    for (i=0; i<tamanho; i++)
    {
        nova_tabela->tabela[i] = NULL;
    }
    return nova_tabela;
}

int dispersa (int tamanho, int valor)
{
    return valor % tamanho;
}
int insere_NO (tabela_hash* base, char valor[33], unsigned char byte)
{
    conteudo * novo_NO=(conteudo*)malloc(sizeof(conteudo));    
    novo_NO->byte=byte;    
    sprintf(novo_NO->valor, "%s", valor);
    if (base->tabela[dispersa(base->tamanho,byte)]==NULL)
    {
        base->tabela[dispersa(base->tamanho,byte)]=novo_NO;        
        if (base->ocupados==0)
        {
            base->primeiro=novo_NO;
        }
        else
        {
            conteudo *ultimo=base->primeiro;
            while (ultimo->prox != NULL)
            {
                ultimo = ultimo->prox;
            }
            ultimo->prox = novo_NO;            
        }
    }
    else
    {
        int inicio=dispersa(base->tamanho,byte);
        conteudo *ultimo = base->tabela[inicio];
        while (ultimo->prox != NULL)
        {
            ultimo = ultimo->prox;
        }
        ultimo->prox = novo_NO;         

        while(base->tabela[inicio]!=NULL)
        {                              
            inicio =(inicio+1) % base->tamanho;                
        }
        base->tabela[inicio]=novo_NO;        
    }
    novo_NO->prox=NULL;  
    base->ocupados++;    
    return 1;     
}
//
double carga (tabela_hash *base)
{
    if (base==NULL)
    {
        return -1;
    }
    else
    {
        return (double) (base->ocupados)/(base->tamanho);
    }
}

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
    while (base->next->next != NULL)
    {
        fila * primeiro = base;
        fila * segundo = base->next;
        fila * terceiro = base->next->next;
        fila *nova= (fila *) malloc(sizeof(fila));
        nova-> byte = (unsigned char) 42;
        nova->freq=primeiro->freq + segundo->freq;
        nova->next=NULL;
        nova->esq=primeiro;
        nova->dir=segundo; // terminada a primeira parte de inserir o novo no (arvore)
        primeiro->next=NULL;
        segundo->next=NULL;
        base= terceiro;
         
        base = insere_fp(base,nova); //inseri o novo no na fila        
    }
        fila * primeiro = base;
        fila * segundo = base->next;        
        fila *nova= (fila *) malloc(sizeof(fila));
        nova-> byte = (unsigned char) 42 ;
        nova->freq=primeiro->freq + segundo->freq;
        nova->next=NULL;
        nova->esq=primeiro;
        nova->dir=segundo; // terminada a primeira parte de inserir o novo no (arvore)
        primeiro->next=NULL;
        segundo->next=NULL;
        base= nova;           
    return base;
}

fila *cria_no (fila *base, struct byte_info ref, unsigned char byte)
{
    fila *nova = (fila *)malloc(sizeof(fila));
    nova->byte = byte;
    nova->freq = ref.frequencia;
    nova->next = base;
    nova->esq = NULL;
    nova->dir = NULL;
    return nova;
}

fila *ordena(fila *head)
{
    fila *previous;
    fila *next;
    fila *inicio;
    fila *conta;
    conta = head;
    int ant_freq, current_freq, change_freq;
    unsigned char ant_byte, current_byte, change_byte;
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

fila *gera_fila (struct byte_info info[])
{
    fila* queue = NULL;
    int cont = 0;
    unsigned char byte;
    
    for (int i = 0; i < 256; i++)
    {
        if (info[i].frequencia > 0)
        {
            cont++;
            queue = cria_no(queue, info[i], (unsigned char)i);
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
    unsigned char buffer[1024];
    int n;

    while ((n = fread(buffer, sizeof(unsigned char), sizeof(buffer), fp)) > 0)
    {
        for (int i = 0; i < n; i++)
        {
            unsigned char byte = buffer[i];
            frequencia[byte]++;
        }
    }
    fclose(fp);

    struct byte_info info[256] = {0};
    for (int i = 0; i < 256; i++)
    {
        info[i].frequencia = frequencia[i];
    }
    
    queue = gera_fila(info);
    return queue;
}

void codifica (fila* no, char *bits, int num_bits, tabela_hash *base)  
{    
    unsigned char byte;
    char valor_atual[33]={};
    int i;
    if( no==NULL)
    {
        return;
    }
    if (no->esq==NULL && no->dir == NULL)
    {
        byte = no->byte;            
        for (i=0; i<num_bits;i++)
        {
            valor_atual[i] = bits[i];            
        }
        printf("byte:%d codigo:%s\n",byte,valor_atual); 
        insere_NO(base,valor_atual,byte);                 
    }
    else
    {
        bits[num_bits] = '0';
        codifica (no->esq,bits,num_bits+1,base);
        bits[num_bits] = '1';
        codifica (no->dir,bits,num_bits+1,base);
    }      
}

void gera_codifica (fila* raiz,tabela_hash *base)
{
    char bits[MAX_BITS];
    codifica(raiz,bits,0,base);
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
int count_levels(fila *root, int level) 
{
    if (root == NULL) 
    {
        return level;
    } 
    else 
    {
        int esq = count_levels(root->esq, level + 1);
        int dir = count_levels(root->dir, level + 1);
        return esq > dir ? esq : dir;
    }
}
void print_byte_in_binary(unsigned char byte) 
{
    int i;
    for (i = 7; i >= 0; i--) {
        printf("%d", (byte >> i) & 1);
    }
    printf("\n");
}

int quant_NO (fila* no)  
{
    if (no==NULL)
    {
        return 0;
    }
    int esq=quant_NO(no->esq);
    int dir=quant_NO(no->dir);
    return (esq + dir +1);
}

void cabe (int tam_lixo, int tam_arvore, unsigned char dois_bytes[17])
{
    int i=2;     
    while (tam_lixo>=2)
    {
        if (tam_lixo%2==1)
        {
            dois_bytes[i]='1';
        }
        tam_lixo=tam_lixo/2;
        i--;
    }
    if (tam_lixo==1)
    {
        dois_bytes[i]='1';
    }
    i=15;
    while (tam_arvore>=2)
    {
        if (tam_arvore%2==1)
        {
            dois_bytes[i]='1';
        }
        tam_arvore=tam_arvore/2;
        i--;
    }
    if (tam_arvore==1)
    {
        dois_bytes[i]='1';
    }
}

void comprime (fila* queue, tabela_hash *base, char arquivo [100])
{
    FILE *fp = fopen(arquivo, "rb"); // rb modo binario de leitura.
    FILE *fs = fopen("saida.txt","wb");// wb modo binario de escrita.
    if (fp == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    unsigned char byte_out;
    unsigned char byte[1];
    int i;
    int j=7;
    int tam_lixo=0;
    while(fread(byte,1,1,fp))
    {                  
        printf("Lido o Byte:%d\n",byte[0]);
        int chave = dispersa(base->tamanho, byte[0]);
        conteudo *atual = base->tabela[chave];
        conteudo *volta = atual;
        if (atual==NULL)
        {
            return ;
        }
        while (atual != NULL)
        {
            if (atual->byte == byte[0])
            {
                printf("Codigo do Byte %d: %s\n", byte[0], atual->valor);
                break;
            }
            atual = atual->prox;
        }
        atual = volta;
        int tam=strlen(atual->valor);
        for (i=0;i<tam;i++)
        {
            printf("tamanho do byte em bits:%d\n",tam);
            printf("pos %d, valor do bit %c\n",i,atual->valor[i]);
                        
            if(atual->valor[i]=='1')
            {
                byte_out=(byte_out|(1<<j));
            } 
            printf("Byte esta atualmente:");
            print_byte_in_binary(byte_out);           
            if (j>0)
            {
                j--;
            }
            else
            {
                fwrite(&byte_out,1,1,fs);
                printf("GRAVEI este byte no arquivo:");
                print_byte_in_binary(byte_out);
                byte_out = 0;
                j=7;
            }
            printf("valor do j:%d\n",j);           
        }
    } 
    if (j !=7)
    {
        printf ("Tamanho do Lixo=%d\n",j+1);
        tam_lixo=j+1;
        fwrite(&byte_out, 1, 1, fs);
        printf("GRAVEI este byte no arquivo:");
        print_byte_in_binary(byte_out);
    }
    else
    {
        printf ("Tamanho do Lixo=%d\n",j-7);        
    }
    printf ("Agora vou gravar no inicio do arquivo:\n");
    unsigned char dois_bytes[17]; //para o cabeçalho
    memset(dois_bytes, '0', sizeof(dois_bytes));     
    dois_bytes[16]='\0';    //fim da variavel do cabeçalho;
    int tam_arvore = quant_NO (queue);
    printf("tamanho da arvore retornado:%d\n",tam_arvore);
    cabe (tam_lixo,tam_arvore, dois_bytes);
    if (fseek(fs, 0L, SEEK_SET) != 0) 
    {
        printf("Erro ao voltar para o início do arquivo\n");
        exit(1);
    }
    byte_out=0;
    j=7;
    for (i=0;i<17;i++)
    {
        if(dois_bytes[i]=='1')
        {
            byte_out=(byte_out|(1<<j));
        } 
        printf("Byte esta atualmente:");
        print_byte_in_binary(byte_out);           
        if (j>0)
        {
            j--;
        }
        else
        {
            fwrite(&byte_out,1,1,fs);
            printf("GRAVEI este byte no arquivo:");
            print_byte_in_binary(byte_out);
            byte_out = 0;
            j=7;
        }
        printf("valor do j:%d\n",j);         
    }
    printf ("Agora vou colocar a arvore de hufmman\n");
    void escreve_pre_ordem(fila* raiz)  // aninhada para permanecer.
    {
        if (raiz == NULL) 
        {
            return;
        }
        fwrite(&raiz->byte,1,1,fs);
        printf("GRAVEI este byte no arquivo:");
        print_byte_in_binary(raiz->byte);        
        escreve_pre_ordem(raiz->esq);
        escreve_pre_ordem(raiz->dir);
    }
    escreve_pre_ordem(queue);
    fclose(fp);
    fclose(fs);    
}

int main()
{
    char nome_arquivo[100];
    unsigned char procura;
    printf("Digite o nome do arquivo com a extensao: ");
    scanf("%s", nome_arquivo);    
    fila *queue = abrir(nome_arquivo);
    ordena(queue);
    printa_queue(queue);     
    printf("resultado da tabela: \n");
    queue = insere_tree (queue);
    int niveis=count_levels(queue,0);        
    tabela_hash* tabela = criar_tabela(niveis*(pow(2,niveis))); 
    gera_codifica (queue,tabela); 
    comprime(queue,tabela,nome_arquivo);        
    return 0;
}
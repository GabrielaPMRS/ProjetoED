#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#define MAX_BITS 256
struct byte_info
{
    void* frequencia;
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
    char valor[256];
    struct conteudo *prox;
} conteudo;

typedef struct tabela_hash
{
    void* tamanho;
    void* ocupados;
    conteudo *primeiro;
    conteudo **tabela;
} tabela_hash;

tabela_hash *criar_tabela(int tamanho)
{
    tabela_hash *nova_tabela = (tabela_hash *)malloc(sizeof(tabela_hash));
    nova_tabela->tamanho = tamanho;
    nova_tabela->ocupados = 0;
    nova_tabela->primeiro = NULL;
    nova_tabela->tabela = (conteudo **)malloc(sizeof(conteudo *) * tamanho);
    if (nova_tabela->tabela == NULL)
    {
        free(nova_tabela);
        return NULL;
    }
    int i;
    for (i = 0; i < tamanho; i++)
    {
        nova_tabela->tabela[i] = NULL;
    }
    return nova_tabela;
}

int insere_NO(tabela_hash *base, char valor[256], unsigned char byte)
{
    conteudo *novo_NO = (conteudo *)malloc(sizeof(conteudo));
    novo_NO->byte = byte;
    sprintf(novo_NO->valor, "%s", valor);
    if (base->tabela[byte % 256] == NULL)
    {
        base->tabela[byte % 256] = novo_NO;
        if (base->ocupados == 0)
        {
            base->primeiro = novo_NO;
        }
        else
        {
            conteudo *ultimo = base->primeiro;
            while (ultimo->prox != NULL)
            {
                ultimo = ultimo->prox;
            }
            ultimo->prox = novo_NO;
        }
    }
    else
    {

        conteudo *ultimo = base->tabela[byte % 256];
        while (ultimo->prox != NULL)
        {
            ultimo = ultimo->prox;
        }
        ultimo->prox = novo_NO;
        base->tabela[byte % 256] = novo_NO;
    }
    novo_NO->prox = NULL;
    base->ocupados++;
    return 1;
}

fila *insere_fp(fila *base, fila *novo)
{
    fila *inicio = base;
    fila *corrente = base;
    fila *anterior = NULL;
    while (corrente->next != NULL)
    {
        if (novo->freq > corrente->freq)
        {
            anterior = corrente;
            corrente = corrente->next;
        }
        else
        {
            break;
        }
    }
    if (corrente->next == NULL && corrente->freq != novo->freq)
    {
        corrente->next = novo;
        novo->next = NULL;
        return inicio;
    }
    if (anterior != NULL)
    {
        anterior->next = novo;
        novo->next = corrente;
        return inicio;
    }
    else
    {
        novo->next = corrente;
        corrente = novo;
        return corrente;
    }
}

fila *insere_tree(fila *base)
{
    while (base->next->next != NULL)
    {
        fila *primeiro = base;
        fila *segundo = base->next;
        fila *terceiro = base->next->next;
        fila *nova = (fila *)malloc(sizeof(fila));
        nova->byte = (unsigned char)42;
        nova->freq = primeiro->freq + segundo->freq;
        nova->next = NULL;
        nova->esq = primeiro;
        nova->dir = segundo; // terminada a primeira parte de inserir o novo no (arvore)
        primeiro->next = NULL;
        segundo->next = NULL;
        base = terceiro;

        base = insere_fp(base, nova); // inseri o novo no na fila
    }
    fila *primeiro = base;
    fila *segundo = base->next;
    fila *nova = (fila *)malloc(sizeof(fila));
    nova->byte = (unsigned char)42;
    nova->freq = primeiro->freq + segundo->freq;
    nova->next = NULL;
    nova->esq = primeiro;
    nova->dir = segundo; // terminada parte de inserir os dois restantes  (arvore)
    primeiro->next = NULL;
    segundo->next = NULL;
    base = nova;
    return base;
}

fila *cria_no(fila *base, struct byte_info ref, unsigned char byte)
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

fila *gera_fila(struct byte_info info[])
{
    fila *queue = NULL;
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

fila *abrir(char nome_arquivo[100])
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

void codifica(fila *no, char *bits, int num_bits, tabela_hash *base)
{
    unsigned char byte;
    char valor_atual[256] = {};
    int i;
    if (no == NULL)
    {
        return;
    }
    if (no->esq == NULL && no->dir == NULL)
    {
        byte = no->byte;
        for (i = 0; i < num_bits; i++)
        {
            valor_atual[i] = bits[i];
        }
        insere_NO(base, valor_atual, byte); // posso substituir isto por um array de string ex: tabela[byte] =valor_atual
    }
    else
    {
        bits[num_bits] = '0';
        codifica(no->esq, bits, num_bits + 1, base);
        bits[num_bits] = '1';
        codifica(no->dir, bits, num_bits + 1, base);
    }
}

void gera_codifica(fila *raiz, tabela_hash *base)
{
    char bits[MAX_BITS];
    codifica(raiz, bits, 0, base);
}

int quant_NO(fila *no)
{
    if (no == NULL)
    {
        return 0;
    }
    int esq = quant_NO(no->esq);
    int dir = quant_NO(no->dir);
    return (esq + dir + 1);
}

void header(int tam_lixo, int tam_arvore, unsigned char dois_bytes[17])
{
    int i = 2;
    while (tam_lixo >= 2)
    {
        if (tam_lixo % 2 == 1)
        {
            dois_bytes[i] = '1';
        }
        tam_lixo = tam_lixo / 2;
        i--;
    }
    if (tam_lixo == 1)
    {
        dois_bytes[i] = '1';
    }
    i = 15;
    while (tam_arvore >= 2)
    {
        if (tam_arvore % 2 == 1)
        {
            dois_bytes[i] = '1';
        }
        tam_arvore = tam_arvore / 2;
        i--;
    }
    if (tam_arvore == 1)
    {
        dois_bytes[i] = '1';
    }
}

void comprime(fila *queue, tabela_hash *base, char arquivo[100])
{
    FILE *fp = fopen(arquivo, "rb");    // rb modo binario de leitura.
    FILE *fs = fopen("temp.txt", "wb"); // wb modo binario de escrita.
    if (fp == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    unsigned char byte_out;
    unsigned char byte[1];
    int i;
    int j = 7;
    int tam_lixo = 0;
    while (fread(byte, 1, 1, fp))
    {
        int chave = byte[0] % 256;
        conteudo *atual = base->tabela[chave];
        if (atual == NULL)
        {
            return;
        }
        int tam = strlen(atual->valor);
        for (i = 0; i < tam; i++)
        {
            if (atual->valor[i] == '1')
            {
                byte_out = (byte_out | (1 << j));
            }

            if (j > 0)
            {
                j--;
            }
            else
            {
                fwrite(&byte_out, 1, 1, fs);
                byte_out = 0;
                j = 7;
            }
        }
    }
    if (j != 7)
    {
        tam_lixo = j + 1;
        fwrite(&byte_out, 1, 1, fs);
    }
    fclose(fs);
    fclose(fp);
    FILE *huff = fopen("saida.huff", "wb"); // wb modo binario de escrita.
    fs = fopen("temp.txt", "rb");           // wb modo binario de leitura.

    unsigned char dois_bytes[17]; // para o cabeçalho
    memset(dois_bytes, '0', sizeof(dois_bytes));
    dois_bytes[16] = '\0'; // fim da variavel do cabeçalho;
    int tam_arvore = quant_NO(queue);
    header(tam_lixo, tam_arvore, dois_bytes);
    byte_out = 0;
    j = 7;
    for (i = 0; i < 17; i++)
    {
        if (dois_bytes[i] == '1')
        {
            byte_out = (byte_out | (1 << j));
        }
        if (j > 0)
        {
            j--;
        }
        else
        {
            fwrite(&byte_out, 1, 1, huff);
            byte_out = 0;
            j = 7;
        }
    }
    void escreve_pre_ordem(fila * raiz) // aninhada para permanecer.
    {
        if (raiz == NULL)
        {
            return;
        }
        fwrite(&raiz->byte, 1, 1, huff);
        escreve_pre_ordem(raiz->esq);
        escreve_pre_ordem(raiz->dir);
    }
    escreve_pre_ordem(queue);
    while (fread(byte, 1, 1, fs))
    {
        fwrite(byte, 1, 1, huff);
    }
    fclose(huff);
}

int main()
{
    char nome_arquivo[100];
    printf("Digite o nome do arquivo com a extensao: ");
    scanf("%s", nome_arquivo);
    fila *queue = abrir(nome_arquivo);
    ordena(queue);
    queue = insere_tree(queue);
    tabela_hash *tabela = criar_tabela(256);
    gera_codifica(queue, tabela);
    comprime(queue, tabela, nome_arquivo);
    return 0;
}
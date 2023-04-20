#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

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

typedef struct hashTable
{
  char *key;
  char value;
  struct hashTable *next;
} hashTable;

// Função para criar uma nova entrada na hashtable
hashTable *new_hashTable(char *key, char value, hashTable *next)
{
  hashTable *e = (hashTable *)malloc(sizeof(hashTable));
  e->key = key;
  e->value = value;
  e->next = next;
  return e;
}

// Função auxiliar para percorrer a árvore de huffman e criar a hashtable
void traverse(fila *node, char *code, int depth, hashTable **hashtable)
{
  // Se o nó atual é uma folha, armazene sua chave e valor na hashtable
  if (node->esq == NULL && node->dir == NULL)
  {
    char *key = (char *)malloc((depth + 1) * sizeof(char));
    strncpy(key, code, depth);
    key[depth] = '\0';
    int index = (int)(node->byte) % 26; // Índice simples para evitar colisões
    hashtable[index] = new_hashTable(key, node->byte, hashtable[index]);
    return;
  }

  // Se o nó atual não é uma folha, adicione o código correto e continue percorrendo a árvore
  code[depth] = '0';
  traverse(node->esq, code, depth + 1, hashtable);
  code[depth] = '1';
  traverse(node->dir, code, depth + 1, hashtable);
}

// Função principal para criar a hashtable a partir da árvore de huffman
hashTable **create_hashtable(fila *root)
{
  hashTable **hashtable = (hashTable **)calloc(256, sizeof(hashTable *)); // Hashtable com 26 entradas (A-Z)

  char *code = (char *)malloc(256 * sizeof(char)); // Código binário para cada símbolo (assumindo que são no máximo 256 símbolos)
  memset(code, 0, 256);

  traverse(root, code, 0, hashtable); // Percorre a árvore de huffman e preenche a hashtable

  free(code);
  return hashtable;
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
  nova->dir = segundo; // terminada a primeira parte de inserir o novo no (arvore)
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

void codifica(fila *no, int *bits, int num_bits)
{
  int i;

  if (no == NULL)
  {
    return;
  }

  if (no->esq == NULL && no->dir == NULL)
  {
    printf("Byte %d:", no->byte);

    for (i = 0; i < num_bits; i++)
    {
      printf("%d", bits[i]);
    }

    printf("\n");
  }
  else
  {
    bits[num_bits] = 0;
    codifica(no->esq, bits, num_bits + 1);

    bits[num_bits] = 1;
    codifica(no->dir, bits, num_bits + 1);
  }
}

void imprime_codifica(fila *raiz)
{
  int bits[MAX_BITS];
  codifica(raiz, bits, 0);
}

void print_tree(struct fila *root, int depth) // Função que imprime a árvore de huffman em pré-ordem
{
  if (root == NULL) // Quando for NULL retorna
  {
    return;
  }

  printf("%c", root->byte); // imprime o byte da raíz

  print_tree(root->esq, depth + 1); // recursivamente imprime os bytes da esquerda
  print_tree(root->dir, depth + 1); // recursivamente, após retornar da recursão da esquerda,
                                    // imprime os bytes da direita
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

void print_hashTable(hashTable *e, FILE *hash)
{
  fprintf(hash, "%s -> %c\n", e->key, e->value);
}

// Função para imprimir toda a hashtable
void print_hashtable(hashTable **hashtable)
{
  FILE *hasht = fopen("hash.txt", "w");
  for (int i = 0; i < 256; i++)
  { // Assume que a hashtable tem 26 entradas (A-Z)
    hashTable *e = hashtable[i];
    while (e != NULL)
    {
      print_hashTable(e, hasht);
      e = e->next;
    }
  }
}

int main()
{
  char nome_arquivo[100];
  printf("Digite o nome do arquivo com a extensao: ");
  scanf("%s", nome_arquivo);

  fila *queue = abrir(nome_arquivo);
  ordena(queue);
  printa_queue(queue);

  printf("testando a mexida para arvore \n");
  queue = insere_tree(queue);
  printf("niveis da arvore:%d\n", count_levels(queue, 0));

  imprime_codifica(queue);
  printf("\n");

  printf("Arvore em pre-ordem:\n");
  print_tree(queue, 0);
  printf("\n");

  hashTable **hash = create_hashtable(queue);

  printf("Imprimindo a Tabela Hash:\n");
  print_hashtable(hash);

  return 0;
}
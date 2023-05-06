#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>

typedef struct tree
{
    unsigned char byte;   
    struct tree *esq;
    struct tree *dir;
} tree;

void calcula_dois_bytes (unsigned char byte[2],int *header) 
{
    unsigned char dois_bytes[17]={};
    int i, j;    
    char bit[2] = "0";
    for (i=0;i<2;i++)
    {
        for (j = 7; j >= 0; j--)
        {           
            snprintf(bit, 2, "%d", (byte[i] >> j) & 1);
            strcat(dois_bytes, bit);
        }
    }    
    for (i=0;i<3;i++)
    {
        if(dois_bytes[i]=='1')
        {
            header[0]=header[0]+pow(2,2-i);
        }        
    }    
    for (i=3;i<16;i++)
    {
        if(dois_bytes[i]=='1')
        {
            header[1]=header[1]+pow(2,15-i);
        }        
    }
    header[2]=header[2]-(header[1]+2); //aqui eu atualizei o tamanho de bytes que vou ler.    
}

tree *reconstruir_arvore (unsigned char *bytes_arvore, int tam) 
{
    static int i = 0; // contador para percorrer o array de bytes em pré-ordem
    tree *no = (tree*)malloc(sizeof(tree));
    no->byte = bytes_arvore[i++];
    if (no->byte == 42) // nó intermediário
    { 
        no->esq = reconstruir_arvore(bytes_arvore, tam);
        no->dir = reconstruir_arvore(bytes_arvore, tam);
    } 
    else // nó folha
    { 
        no->esq = NULL;
        no->dir = NULL;
    }
    return no;
}

tree * open (FILE *fc , int *header)
{
    int i;    
    unsigned char bytes_iniciais [2];
    unsigned char dois_bytes[17] ={}; // para o cabeçalho
    tree* arvore= NULL; 
    fseek(fc,0L,SEEK_END);
    header[2] = ftell(fc);//tamanho total do arquivo;
    rewind(fc);
    fread(bytes_iniciais, 1, 2, fc); //li os dois bytes iniciais e coloquei na variavel bytes_iniciais
    calcula_dois_bytes (bytes_iniciais,header); 
    unsigned char bytes_arvore[header[1]]; 
    fread(bytes_arvore,1,header[1],fc);
    printf ("%d bytes em pre-ordem\n",header[1]);
    for (i=0;i<header[1];i++)
    {
        printf("%d ",bytes_arvore[i]);
    }
    arvore = reconstruir_arvore(bytes_arvore, header[1]);
    printf ("\n");     
    return arvore; 
}

void decodifica (FILE *fc, tree* arvore)
{
    int i, j, tam, lixo;
    tree* raiz = arvore;    
    unsigned char byte[9] = {};
    unsigned char mostrabyte;
    printf ("entre com o numero de bytes\n");
    scanf("%d",&tam);
    printf ("entre com o tamanho do lixo\n");
    scanf("%d",&lixo);
    int limite=0;
    for (i=0;i<tam;i++)
    {
        if (i==tam-1)
        {
            limite=lixo;
        }
        printf("Entre com o byte %d\n",i+1);
        scanf("%s",byte);        
        for (j=0;j<8-limite;j++)
        {
            while(!(arvore->esq==NULL && arvore->dir==NULL))
            {                
                if(byte[j]=='0')
                {
                    arvore=arvore->esq;
                }
                else if (byte[j]=='1')
                {
                    arvore=arvore->dir;
                }
                j++;
                if (j>8)
                {
                    break;
                }
            }
            if (j<=8)
            {
                mostrabyte=arvore->byte;
                printf("byte encontrado:%c\n",mostrabyte);
                j--;
                arvore = raiz;
            }            
        }
    }   
}

void print_byte_in_binary(unsigned char byte) 
{
    int i;
    for (i = 7; i >= 0; i--)
    {
        printf("%d", (byte >> i) & 1);
    }
    printf("\n");
}

void decodifica_arquivo (FILE *fc, tree* arvore, int *header)
{
    int i, j, tam,k, lixo;
    tree* raiz = arvore;
    unsigned char mostrabyte;
    unsigned char byte_lido[1];
    char bit[2] = "0";
    int pos = 2 + header[1];
    tam = header[2];
    lixo= header[0];
    int limite = 0;
    int current_pos; 
    unsigned char byte[9] = {};   
    FILE *saida = fopen("teste2.txt", "wb"); // wb modo binario de escrita.    
    for (i=0;i<tam;i++)
    {
        printf ("valor do i:%d\n",i);
        if (i==tam-1)
        {
            limite=lixo;
        }
        current_pos = ftell(fc); // obtém a posição atual do ponteiro de arquivo  
        printf("ponteiro está no %d byte\n",current_pos);
        fread(byte_lido, 1, 1, fc);
        printf ("byte lido:\n");
        print_byte_in_binary(byte_lido[0]);
        unsigned char byte[9] = {};           
        for (k = 7; k >= 0; k--)
        {                       
            snprintf(bit, 2, "%d", (byte_lido[0] >> k) & 1);
            strcat(byte, bit);
        }
        byte[8]='\0';
        printf("byte que vai para decodificar:%s\n",byte);
        for (j=0;j<8-limite;j++)
        {
            while(!(arvore->esq==NULL && arvore->dir==NULL))
            {                
                if(byte[j]=='0')
                {
                    arvore=arvore->esq;
                }
                else if (byte[j]=='1')
                {
                    arvore=arvore->dir;
                }
                j++;
                if (j>8)
                {
                    break;
                }
            }
            if (j<=8)
            {
                mostrabyte=arvore->byte;
                printf("byte encontrado:%c\n",mostrabyte);                
                fwrite(&mostrabyte, 1, 1, saida);
                j--;
                arvore = raiz;
            }            
        }       
    } 
    fclose(saida);          
}
void printa_bytes (FILE *fc)
{
    unsigned char byte[1];
    while( fread(byte, 1, 1, fc))
    {
        print_byte_in_binary(byte[0]);
    }
}

int main()
{
    char nome_arquivo[100];
    char arquivo_saida[100];
    char extensao[10];
    int header [3]={0};
    printf("Digite o nome do arquivo .huff:\n");
    scanf("%s", nome_arquivo);
    printf("Digite a extensao original do arquivo\n");
    scanf ("%s",extensao);  
     
    if (!(strstr(nome_arquivo,".huff")))
    {
        strcat(nome_arquivo,".huff");
    } 
    FILE *fc = fopen(nome_arquivo, "rb"); // rb modo binario de leitura.
    if (fc == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return 0;        
    }  
    printa_bytes (fc);
    tree *arvore = open(fc, header);
    // decodifica(fc, arvore);
    decodifica_arquivo (fc,arvore, header);
    fclose(fc);
    return 0;
}
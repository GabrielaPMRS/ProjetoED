#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

typedef struct lista
{
    void* num;
    struct lista* next;
} lista;

lista* cria_elo (lista* new, int valor)
{
   lista* novo=(lista*)malloc(sizeof(lista));
    novo->num=valor;
    novo->next=new;
    return novo;
}

typedef struct arvore  
{
    void* item;
    struct arvore* esq;
    struct arvore* dir;
} arvore;

arvore* cria_no (arvore* nova, int valor) 
{
    nova=(arvore*)malloc(sizeof(arvore));
    nova->esq=NULL;
    nova->dir=NULL;
    nova->item=valor;
    return nova;

}
arvore* insere(arvore* raiz, int item)  
{
    arvore* no=NULL;  
    arvore* atual=NULL;  
    int i;
    if (raiz==NULL)  
    {
        raiz=cria_no(raiz,item);        
        return raiz;
    }
    else  
    {
        atual=raiz;
        while (atual!= NULL)  
        {
            //printf("No atual:%d No a ser inserido:%d\n",atual->item,item);  
            if(item<=atual->item)  
            {
                if (atual->esq==NULL)  
                {
                    atual->esq=cria_no(no,item);
                    return raiz;
                    
                }
                else  
                {             
                    atual=atual->esq;       
                }
            }
            else  
            {
                if(atual->dir==NULL)  
                {
                    atual->dir=cria_no(no,item); 
                    return raiz;
                    
                }
                else  
                {
                    atual=atual->dir;                    
                }
            }
        }
    }        
}

void printa_pre (arvore* no)  
{
   if( no==NULL)
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

void printa_em (arvore* no)  
{
   if( no==NULL)
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

void printa_pos (arvore* no)  
{
   if( no==NULL)
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
int quant_NO (arvore* no)  
{
    if (no==NULL)
    {
        return 0;
    }
    int esq=quant_NO(no->esq);
    int dir=quant_NO(no->dir);
    return (esq + dir +1);
}

void printa_lista (lista* no)
{
    while (no!=NULL)
    {
        printf("%d ",no->num);
        no=no->next;
    }
    printf("\n");
}

int busca_arv_bin(arvore* raiz, int item)
{
    int cont=0;
    if(raiz==NULL)
    {
        return 0;
    }
    arvore* atual=raiz;
    while (atual != NULL)
    {
        cont++;
        if (item==atual->item)
        {
            return cont;
        }
        if (item<=atual->item)
        {
            atual= atual->esq;
        }
        else
        {
            atual=atual->dir;
        }

    }
    return 0;
}

int busca_lista(lista* inicio, int item)
{
    int cont=0;
    if (inicio==NULL) 
    {
        return 0;
    }
    lista *atual=inicio;
    while(atual != NULL)
    {
        cont++;
        if(item==atual->num)
        {
            return cont;
        }
        atual=atual->next;
    }
    return 0;
}

int main ()
{
    int i,n,item, contlist,contarv;
    arvore* nova = NULL; 
    lista* new = NULL;
    FILE* imput = fopen("entrada_pequena.txt", "r");
    if(imput == NULL)
    {
        printf("A entrada nao existe\n");
        return 0;
    }
    fscanf(imput, "%d", &n); //ler a entrada pelo arquivo
    for (i = 0; i < n; i++)
    { 
        fscanf(imput, "%d", &item);             
        nova = insere(nova,item);        
        new = cria_elo(new,item);
    } 
    printf( "Avore Busca Binaria:\n");       
    printa_pre(nova);
    printf ("\n");  
    printf( "Lista:\n");
    printa_lista(new); 

    //fscanf(imput, "%d", &item);
    fclose(imput);

    FILE* output = fopen("saida.txt", "w");
    int quant_numeros;
    printf("numeros aleatorios: \n");
    srand(time(NULL));
    for(quant_numeros = 0; quant_numeros < (int)(0.25 * n); quant_numeros++)
    {
        int r = rand() % n;
        //printf("%d: ", r);
        contarv = busca_arv_bin (nova,r);
        contlist = busca_lista(new,r);
        //printf("%d ", contlist);
        //printf("%d\n", contarv);

        fprintf(output, "%d,%d,%d\n", r, contlist, contarv);
        
    }
    fclose(output);
    printf("\n");
    
    if (contarv == 0)
    {
        printf("Nao encontrado na arvore\n");
    }
    else
    {
        printf("encontrado na arvore com %d iteracoes\n",contarv);

    }  
    if (contlist == 0)
    {
        printf("Nao encontrado na lista\n");
    }
    else
    {
        printf("encontrado na lista com %d iteracoes\n",contlist);

    }    
    return 0;
}
#include <stdio.h>
#include <string.h>


struct byte_info
{
    int frequencia;
    long posicao;
};

void sortFrequency(struct byte_info info[]) // Função de ordenar o array da struct byte
{
    for (int j = 256; j >= 0; j--) //Fiz um bubbleSort tradicional
    {
        for (int i = 0; i < j; i++)
        {
            if (info[i].frequencia < info[i + 1].frequencia)
            {
                struct byte_info aux = info[i];
                info[i] = info[i + 1];
                info[i + 1] = aux;
            }
        }
    }
}

void imprimir_frequencia(FILE *saida, struct byte_info info[])
{

    sortFrequency(info); // Antes de printar, ele ordena o array

    for (int i = 0; i < 256; i++)
    {
        if (info[i].frequencia > 0)
        {
            fprintf(saida, "Byte: %d, Frequencia: %d, Posicao: %ld\n", i, info[i].frequencia, info[i].posicao);
        } //Agora no lugar de printar o for, ele vai ser escrito no arquivo saida.txt
    }
}

int main() 
{
    char nome_arquivo[100];
    printf("Digite o nome do arquivo com a extensao: ");
    scanf("%s", nome_arquivo);

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

    FILE *saida = fopen("saida.txt", "w"); // w modo de escrita que cria e sobrescreve o arquivo .txt
    
    imprimir_frequencia(saida, info);    // Passa o ponteiro do arquivo
                                        //O ponteiro do arquivo é uma referência ao objeto FILE que representa o arquivo aberto. 
                                        
    fclose(saida); // "Fecha" o arquivo criado


    // Agora, você pode usar o array "frequencia" para construir a árvore de Huffman
    // e gerar os códigos binários para cada byte. Para associar a posição de cada byte
    // à sua frequência, basta acessar o array "info".


    // Agora, você pode usar o array "frequencia" para construir a árvore de Huffman
    // e gerar os códigos binários para cada byte. Para associar a posição de cada byte
    // à sua frequência, basta acessar o array "info".

    return 0;
}

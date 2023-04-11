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

void imprimir_frequencia(struct byte_info info[])
 {
    sortFrequency(info);
    
    for (int i = 0; i < 256; i++) {
        if (info[i].frequencia > 0) 
        {
            printf("Byte: %d, Frequência: %d, Posição: %ld\n", i, info[i].frequencia, info[i].posicao);
        }
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
    imprimir_frequencia(info);

    // Agora, você pode usar o array "frequencia" para construir a árvore de Huffman
    // e gerar os códigos binários para cada byte. Para associar a posição de cada byte
    // à sua frequência, basta acessar o array "info".

    return 0;
}

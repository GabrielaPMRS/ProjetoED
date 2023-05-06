#include <stdio.h>

int main() 
{
    FILE *fp;
    long num_bytes;

    fp = fopen("saida.huff", "rb"); // abre o arquivo para leitura em modo binário

    if (fp == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }

    fseek(fp, 0L, SEEK_END); // move o cursor para o final do arquivo
    num_bytes = ftell(fp); // obtém a posição do cursor, que é igual ao número de bytes do arquivo
    fclose(fp); // fecha o arquivo

    printf("O arquivo tem %ld bytes\n", num_bytes);

    return 0;
}

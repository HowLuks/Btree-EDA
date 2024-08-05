#include <stdio.h>
#include <stdlib.h>

#define MAX_NUMEROS 1000 // Define o tamanho máximo do vetor

int main() {
    FILE *arquivo;
    int numeros[MAX_NUMEROS];
    int i = 0;

    // Abra o arquivo em modo de leitura
    arquivo = fopen("numeros.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }

    // Leia os números do arquivo e armazene-os no vetor
    while (fscanf(arquivo, "%d", &numeros[i]) != EOF) {
        i++;
        if (i >= MAX_NUMEROS) {
            printf("Vetor cheio. Certifique-se de que o número de linhas no arquivo não exceda %d.\n", MAX_NUMEROS);
            break;
        }
    }

    fclose(arquivo);

    // Exibe os números lidos
    for (int j = 0; j < i; j++) {
        printf("Número %d: %d\n", j+1, numeros[j]);
    }

    return 0;
}

// gerador.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Gera um timestamp aleatório entre dois valores (inicio e fim)
long gerar_timestamp_aleatorio(time_t inicio, time_t fim) {
    return inicio + rand() % (fim - inicio + 1);
}

// Gera um valor aleatório com base no tipo especificado: int, float, bool ou str
void gerar_valor(char tipo[], char *destino) {
    if (strcmp(tipo, "int") == 0)
        sprintf(destino, "%d", rand() % 100);  // Inteiro entre 0 e 99
    else if (strcmp(tipo, "float") == 0)
        sprintf(destino, "%.2f", (rand() % 10000) / 100.0);  // Float com 2 casas decimais
    else if (strcmp(tipo, "bool") == 0)
        sprintf(destino, "%s", (rand() % 2) ? "true" : "false");  // true ou false
    else if (strcmp(tipo, "str") == 0)
        sprintf(destino, "msg%d", rand() % 1000);  // String tipo "msg123"
}

int main(int argc, char *argv[]) {
    // Verifica se os argumentos necessários foram passados
    if (argc < 5) {
        printf("Uso: %s <inicio_epoch> <fim_epoch> <saida.txt> <sensor:tipo> [...]\n", argv[0]);
        return 1;
    }

    // Inicializa o gerador de números aleatórios com base na hora atual
    srand(time(NULL));

    // Converte os argumentos de início e fim do intervalo de timestamps
    time_t inicio = atol(argv[1]);
    time_t fim = atol(argv[2]);

    // Abre o arquivo de saída onde os dados gerados serão gravados
    FILE *saida = fopen(argv[3], "w");

    // Para cada sensor passado como argumento (a partir do 4º)
    for (int i = 4; i < argc; i++) {
        char sensor[16], tipo[16];
        // Separa o nome do sensor e o tipo do dado (ex: temp:int)
        sscanf(argv[i], "%[^:]:%s", sensor, tipo);

        // Gera 2000 leituras para cada sensor
        for (int j = 0; j < 2000; j++) {
            long ts = gerar_timestamp_aleatorio(inicio, fim);  // Gera timestamp aleatório
            char valor[64];
            gerar_valor(tipo, valor);  // Gera valor com base no tipo
            // Escreve no arquivo no formato: timestamp sensor valor
            fprintf(saida, "%ld %s %s\n", ts, sensor, valor);
        }
    }

    // Fecha o arquivo de saída
    fclose(saida);
    return 0;
}

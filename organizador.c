// organizador.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SENSORES 10     // Número máximo de sensores diferentes que o programa pode processar
#define MAX_LINHA 256       // Tamanho máximo de uma linha lida do arquivo

// Estrutura que representa uma leitura de sensor
typedef struct {
    long timestamp;         // Momento da leitura
    char sensor[16];        // Nome do sensor
    char valor[64];         // Valor registrado pelo sensor (pode ser string, int, float, etc.)
} Leitura;

// Função de comparação usada para ordenar as leituras por timestamp
int comparar_leitura(const void *a, const void *b) {
    Leitura *la = (Leitura *)a;
    Leitura *lb = (Leitura *)b;
    return (la->timestamp - lb->timestamp);
}

int main(int argc, char *argv[]) {
    // Verifica se foi fornecido o nome do arquivo de entrada
    if (argc < 2) {
        printf("Uso: %s <arquivo_de_entrada>\n", argv[0]);
        return 1;
    }

    // Abre o arquivo de entrada com os dados brutos
    FILE *entrada = fopen(argv[1], "r");
    if (!entrada) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    // Arrays para armazenar dados por sensor
    Leitura *leituras[MAX_SENSORES];      // Ponteiros para arrays de leituras por sensor
    int contadores[MAX_SENSORES] = {0};   // Contador de leituras por sensor
    char sensores[MAX_SENSORES][16];      // Nome dos sensores
    int numSensores = 0;                  // Quantidade de sensores únicos encontrados

    char linha[MAX_LINHA];
    // Lê cada linha do arquivo
    while (fgets(linha, sizeof(linha), entrada)) {
        Leitura l;
        // Extrai os dados da linha: timestamp, nome do sensor e valor
        sscanf(linha, "%ld %s %s", &l.timestamp, l.sensor, l.valor);

        // Verifica se o sensor já foi encontrado anteriormente
        int i;
        for (i = 0; i < numSensores; i++) {
            if (strcmp(l.sensor, sensores[i]) == 0)
                break;
        }

        // Se for um novo sensor, registra e aloca espaço para suas leituras
        if (i == numSensores) {
            if (numSensores >= MAX_SENSORES) {
                printf("Erro: muitos sensores diferentes\n");
                return 1;
            }
            strcpy(sensores[numSensores], l.sensor);                   // Guarda o nome do novo sensor
            leituras[numSensores] = malloc(5000 * sizeof(Leitura));   // Aloca espaço para até 5000 leituras
            numSensores++;
        }

        // Adiciona a leitura ao sensor correspondente
        leituras[i][contadores[i]++] = l;
    }
    fclose(entrada); // Fecha o arquivo de entrada

    // Para cada sensor, ordena suas leituras por timestamp e grava em um novo arquivo
    for (int i = 0; i < numSensores; i++) {
        // Ordena as leituras por timestamp usando qsort
        qsort(leituras[i], contadores[i], sizeof(Leitura), comparar_leitura);

        // Cria o nome do arquivo de saída (ex: temperatura.txt)
        char nomeArquivo[32];
        sprintf(nomeArquivo, "%s.txt", sensores[i]);

        // Abre o arquivo para escrita
        FILE *out = fopen(nomeArquivo, "w");

        // Escreve as leituras ordenadas no novo arquivo
        for (int j = 0; j < contadores[i]; j++) {
            fprintf(out, "%ld %s %s\n", leituras[i][j].timestamp, sensores[i], leituras[i][j].valor);
        }

        fclose(out);          // Fecha o arquivo de saída
        free(leituras[i]);    // Libera a memória alocada para as leituras
    }

    return 0;
}

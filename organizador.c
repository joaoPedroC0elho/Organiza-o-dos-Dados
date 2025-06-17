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

    // Verifica se o arquivo de entrada existe
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
        // Verifica se a linha está vazia
        if (strlen(linha) <= 1) {
            printf("Aviso: Linha vazia encontrada no arquivo\n");
            continue;
        }

        Leitura l;
        // Extrai os dados da linha: timestamp, nome do sensor e valor
        if (sscanf(linha, "%ld %s %s", &l.timestamp, l.sensor, l.valor) != 3) {
            printf("Erro: Formato inválido na linha: %s", linha);
            continue;
        }

        // Validação do timestamp
        if (l.timestamp <= 0) {
            printf("Erro: Timestamp inválido na linha: %s", linha);
            continue;
        }

        // Validação do nome do sensor
        if (strlen(l.sensor) == 0 || strlen(l.sensor) > 15) {
            printf("Erro: Nome do sensor inválido na linha: %s", linha);
            continue;
        }

        // Verifica se o sensor já foi encontrado anteriormente
        int i;
        for (i = 0; i < numSensores; i++) {
            if (strcmp(l.sensor, sensores[i]) == 0)
                break;
        }

        // Se for um novo sensor, registra e aloca espaço para suas leituras
        if (i == numSensores) {
            if (numSensores >= MAX_SENSORES) {
                printf("Erro: muitos sensores diferentes (máximo: %d)\n", MAX_SENSORES);
                fclose(entrada);
                // Libera memória alocada anteriormente
                for (int j = 0; j < numSensores; j++) {
                    free(leituras[j]);
                }
                return 1;
            }
            strcpy(sensores[numSensores], l.sensor);
            leituras[numSensores] = malloc(5000 * sizeof(Leitura));
            if (leituras[numSensores] == NULL) {
                printf("Erro: Falha ao alocar memória para o sensor %s\n", l.sensor);
                fclose(entrada);
                // Libera memória alocada anteriormente
                for (int j = 0; j < numSensores; j++) {
                    free(leituras[j]);
                }
                return 1;
            }
            numSensores++;
        }

        // Verifica se atingiu o limite de leituras para o sensor
        if (contadores[i] >= 5000) {
            printf("Aviso: Limite de leituras atingido para o sensor %s\n", l.sensor);
            continue;
        }

        // Adiciona a leitura ao sensor correspondente
        leituras[i][contadores[i]++] = l;
    }

    if (ferror(entrada)) {
        perror("Erro durante a leitura do arquivo");
        fclose(entrada);
        // Libera memória alocada
        for (int i = 0; i < numSensores; i++) {
            free(leituras[i]);
        }
        return 1;
    }
    fclose(entrada);

    // Para cada sensor, ordena suas leituras por timestamp e grava em um novo arquivo
    for (int i = 0; i < numSensores; i++) {
        if (contadores[i] == 0) {
            printf("Aviso: Nenhuma leitura válida para o sensor %s\n", sensores[i]);
            continue;
        }

        // Ordena as leituras por timestamp usando qsort
        qsort(leituras[i], contadores[i], sizeof(Leitura), comparar_leitura);

        // Cria o nome do arquivo de saída (ex: temperatura.txt)
        char nomeArquivo[32];
        sprintf(nomeArquivo, "%s.txt", sensores[i]);

        // Abre o arquivo para escrita
        FILE *out = fopen(nomeArquivo, "w");
        if (!out) {
            printf("Erro: Não foi possível criar o arquivo %s\n", nomeArquivo);
            // Libera memória alocada
            for (int j = 0; j < numSensores; j++) {
                free(leituras[j]);
            }
            return 1;
        }

        // Escreve as leituras ordenadas no novo arquivo
        for (int j = 0; j < contadores[i]; j++) {
            if (fprintf(out, "%ld %s %s\n", leituras[i][j].timestamp, sensores[i], leituras[i][j].valor) < 0) {
                printf("Erro ao escrever no arquivo %s\n", nomeArquivo);
                fclose(out);
                // Libera memória alocada
                for (int k = 0; k < numSensores; k++) {
                    free(leituras[k]);
                }
                return 1;
            }
        }

        if (fclose(out) != 0) {
            printf("Erro ao fechar o arquivo %s\n", nomeArquivo);
            // Libera memória alocada
            for (int j = 0; j < numSensores; j++) {
                free(leituras[j]);
            }
            return 1;
        }

        free(leituras[i]);
    }

    return 0;
}

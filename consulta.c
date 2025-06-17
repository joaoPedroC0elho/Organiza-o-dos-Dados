// consulta.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// Estrutura que representa uma leitura do sensor
typedef struct {
    long timestamp;    // Momento da leitura (em milissegundos ou segundos)
    char valor[64];    // Valor lido pelo sensor (pode ser temperatura, pressão, etc.)
} Leitura;

// Função auxiliar para calcular a diferença absoluta entre dois valores
long abs_diff(long a, long b) {
    return a > b ? a - b : b - a;
}

int main(int argc, char *argv[]) {
    // Verifica se os argumentos foram passados corretamente
    if (argc < 3) {
        printf("Uso: %s <NOME_SENSOR> <TIMESTAMP>\n", argv[0]);
        return 1;
    }

    // Validação do nome do sensor
    if (strlen(argv[1]) == 0 || strlen(argv[1]) > 15) {
        printf("Erro: Nome do sensor inválido. Deve ter entre 1 e 15 caracteres.\n");
        return 1;
    }

    // Validação do timestamp
    char *endptr;
    long alvo = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0' || alvo <= 0) {
        printf("Erro: Timestamp inválido. Deve ser um número positivo.\n");
        return 1;
    }

    // Monta o nome do arquivo baseado no nome do sensor
    char nomeArquivo[32];
    sprintf(nomeArquivo, "%s.txt", argv[1]);

    // Abre o arquivo do sensor
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Erro: Arquivo do sensor '%s' não encontrado\n", nomeArquivo);
        return 1;
    }

    // Conta quantas linhas (leituras) existem no arquivo
    int total = 0;
    char linha[128];
    while (fgets(linha, sizeof(linha), arquivo)) {
        // Valida o formato da linha
        long ts;
        char sensor[16], valor[64];
        if (sscanf(linha, "%ld %s %s", &ts, sensor, valor) != 3) {
            printf("Aviso: Linha com formato inválido ignorada: %s", linha);
            continue;
        }
        total++;
    }

    if (total == 0) {
        printf("Erro: Nenhuma leitura válida encontrada no arquivo\n");
        fclose(arquivo);
        return 1;
    }

    rewind(arquivo); // Volta ao início do arquivo para leitura dos dados

    // Aloca memória para armazenar todas as leituras do arquivo
    Leitura *dados = malloc(total * sizeof(Leitura));
    if (dados == NULL) {
        printf("Erro: Falha ao alocar memória para as leituras\n");
        fclose(arquivo);
        return 1;
    }

    // Lê os dados do arquivo linha por linha
    int leituras_validas = 0;
    for (int i = 0; i < total; i++) {
        if (fgets(linha, sizeof(linha), arquivo)) {
            if (sscanf(linha, "%ld %*s %s", &dados[leituras_validas].timestamp, dados[leituras_validas].valor) == 2) {
                leituras_validas++;
            }
        }
    }
    fclose(arquivo);

    if (leituras_validas == 0) {
        printf("Erro: Nenhuma leitura válida encontrada no arquivo\n");
        free(dados);
        return 1;
    }

    // Algoritmo de busca binária para encontrar a leitura mais próxima do timestamp alvo
    int esq = 0, dir = leituras_validas - 1, melhor = 0;
    while (esq <= dir) {
        int meio = (esq + dir) / 2;

        // Atualiza a melhor leitura se a atual estiver mais próxima do alvo
        if (abs_diff(dados[meio].timestamp, alvo) < abs_diff(dados[melhor].timestamp, alvo)) {
            melhor = meio;
        }

        // Ajusta os limites da busca
        if (dados[meio].timestamp < alvo) {
            esq = meio + 1;
        } else {
            dir = meio - 1;
        }
    }

    // Imprime a leitura mais próxima encontrada
    printf("Leitura mais proxima:\nTIMESTAMP: %ld\nVALOR: %s\n", dados[melhor].timestamp, dados[melhor].valor);

    // Libera a memória alocada
    free(dados);

    return 0;
}

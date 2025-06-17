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

    // Monta o nome do arquivo baseado no nome do sensor
    char nomeArquivo[32];
    sprintf(nomeArquivo, "%s.txt", argv[1]);  // Ex: sensor1 -> sensor1.txt

    // Converte o timestamp alvo (procurado) de string para long
    long alvo = atol(argv[2]);

    // Abre o arquivo do sensor
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir arquivo do sensor");
        return 1;
    }

    // Conta quantas linhas (leituras) existem no arquivo
    int total = 0;
    char linha[128];
    while (fgets(linha, sizeof(linha), arquivo)) total++;
    rewind(arquivo); // Volta ao início do arquivo para leitura dos dados

    // Aloca memória para armazenar todas as leituras do arquivo
    Leitura *dados = malloc(total * sizeof(Leitura));

    // Lê os dados do arquivo linha por linha
    // Ignora o segundo campo (identificador ou separador) usando %*s
    for (int i = 0; i < total; i++) {
        fscanf(arquivo, "%ld %*s %s", &dados[i].timestamp, dados[i].valor);
    }
    fclose(arquivo); // Fecha o arquivo após leitura

    // Algoritmo de busca binária para encontrar a leitura mais próxima do timestamp alvo
    int esq = 0, dir = total - 1, melhor = 0;
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

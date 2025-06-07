# Sistema de Gerenciamento de Dados de Sensores Industriais

Este sistema é composto por três programas que trabalham em conjunto para gerenciar dados de sensores industriais.

## Compilação

Para compilar todos os programas, execute:

```bash
make
```

Isso irá gerar três executáveis:
- `organizar`: Programa 1 - Organização dos Dados
- `consultar`: Programa 2 - Consulta por Instante
- `gerar_teste`: Programa 3 - Geração de Arquivo de Teste

## Programas

### 1. Organização dos Dados (`organizar`)

Este programa processa um arquivo de entrada contendo leituras de diversos sensores e organiza os dados em arquivos separados por sensor, ordenados por timestamp.

Uso:
```bash
./organizar <arquivo_entrada>
```

O arquivo de entrada deve conter linhas no formato:
```
<TIMESTAMP> <ID_SENSOR> <VALOR>
```

### 2. Consulta por Instante (`consultar`)

Este programa realiza busca binária para encontrar a leitura mais próxima de um timestamp específico para um determinado sensor.

Uso:
```bash
./consultar <sensor_id> <timestamp>
```

Exemplo:
```bash
./consultar TEMP 1622505600
```

### 3. Geração de Arquivo de Teste (`gerar_teste`)

Este programa gera um arquivo de teste com leituras aleatórias para os sensores especificados.

Uso:
```bash
./gerar_teste <timestamp_inicio> <timestamp_fim> <sensor1> <tipo1> [<sensor2> <tipo2> ...]
```

Tipos suportados: INT, FLOAT, DOUBLE, LONG

Exemplo:
```bash
./gerar_teste 1622505600 1622592000 TEMP FLOAT PRES INT VIBR DOUBLE
```

## Tipos de Dados Suportados

O sistema suporta os seguintes tipos de dados para os sensores:
- INT: Valores inteiros
- FLOAT: Valores de ponto flutuante (precisão simples)
- DOUBLE: Valores de ponto flutuante (precisão dupla)
- LONG: Valores inteiros longos

## Exemplo de Uso Completo

1. Primeiro, gere dados de teste:
```bash
./gerar_teste 1622505600 1622592000 TEMP FLOAT PRES INT VIBR DOUBLE UMID FLOAT FLUX LONG
```

2. Organize os dados:
```bash
./organizar sensor_data.txt
```

3. Consulte leituras específicas:
```bash
./consultar TEMP 1622548800
```

## Limpeza

Para limpar os arquivos compilados e gerados:
```bash
make clean
``` 
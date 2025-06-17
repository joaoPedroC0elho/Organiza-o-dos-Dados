# Sistema de Gerenciamento de Sensores

Este projeto consiste em um sistema para gerenciar dados de sensores, com três componentes principais: um gerador de dados, um organizador e um consultor.

## Componentes

### 1. Gerador de Dados (gerador.c)
Gera dados simulados de sensores com diferentes tipos de valores (inteiro, float, booleano, string).

### 2. Organizador (organizador.c)
Organiza os dados gerados, separando-os por sensor e ordenando por timestamp.

### 3. Consultor (consulta.c)
Permite consultar o valor mais próximo de um determinado timestamp para um sensor específico.

## Requisitos

- Compilador C (gcc recomendado)
- Sistema operacional Unix-like ou Windows


## Compilação

Para compilar todos os componentes, execute:

```bash
gcc -o gerador gerador.c
gcc -o organizador organizador.c
gcc -o consulta consulta.c
```

## Uso

### 1. Gerando Dados

```bash
./gerador <inicio_epoch> <fim_epoch> <saida.txt> <sensor:tipo> [...]
```

Exemplo:
```bash
./gerador 1718496000 1718582400 dados.txt TEMP:float PRES:int VIBR:bool
```

Tipos de dados suportados:
- `int`: números inteiros
- `float`: números decimais
- `bool`: valores booleanos (true/false)
- `str`: strings

### 2. Organizando Dados

```bash
./organizador <arquivo_de_entrada>
```

Exemplo:
```bash
./organizador dados.txt
```

### 3. Consultando Dados

```bash
./consulta <NOME_SENSOR> <TIMESTAMP>
```

Exemplo:
```bash
./consulta temp 1617235200
```

## Formato dos Dados

### Arquivo de Entrada/Saída
Cada linha do arquivo segue o formato:
```
timestamp sensor valor
```

Exemplo:
```
1617235200 temp 25
1617235201 umidade 65.5
1617235202 presenca true
1617235203 status msg123
```

## Tratamento de Erros

O sistema inclui várias validações e tratamentos de erro:

- Validação de argumentos de entrada
- Verificação de arquivos
- Validação de timestamps
- Verificação de tipos de dados
- Tratamento de memória
- Validação de formato de dados
- Mensagens de erro descritivas

## Limitações

- Máximo de 10 sensores diferentes
- Máximo de 5000 leituras por sensor
- Nome do sensor limitado a 15 caracteres
- Timestamps devem ser positivos

## Contribuição

1. Faça um fork do projeto
2. Crie uma branch para sua feature (`git checkout -b feature/nova-feature`)
3. Commit suas mudanças (`git commit -m 'Adiciona nova feature'`)
4. Push para a branch (`git push origin feature/nova-feature`)
5. Abra um Pull Request


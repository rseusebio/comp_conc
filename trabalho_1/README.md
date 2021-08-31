# Introdução

Este programa consiste em achar a determinante de uma matriz (gerada aleatoriamente pelo programa) 

# Execução do programa

Há duas maneiras de executar o programa

## Manualmente

Compilar o programa main.c:

`gcc -o main main.c -Wall -lpthread`

Em seguida, executá-lo passando a dimensão do vetor, o tipo da matriz e o número de threads. Exemplo:

`./main 100 5 2`

## Através de um script para executá-lo mais de uma vez seguida e salvar o resultado em um arquivo:

Para executar o experimento rode:

`sh script.sh`

Para pegar o CSV final, rode:

`python3.8 analyse.py`

# Tipos de matrizes

- 1: triangular superior
- 2: triangular inferior
- 3: diagonal
- 4: identidade
- 5 ou mais: matriz sem elementos zero.

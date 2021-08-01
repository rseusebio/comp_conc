# Introdução

Este programa consiste em achar o menor e maior valores de um vetor de dimensão configurável atráves de execuções sequenciais ou concorrentes (com 2 ou 4 threads)

# Execução do programa

Há duas maneiras de executar o programa

## Manualmente

Compilar o programa main.c:

`gcc -o main main.c -Wall -lpthread`

Em seguida, executá-lo passando a dimensão do vetor e o número de threads. Exemplo:

`./main 100000 2`

## Através de um script para executá-lo mais de uma vez seguida e salvar o resultado em um arquivo:

Para executar o experimento rode:

`sh script.sh`

Para pegar o CSV final, rode:

`python3.8 analyse.py`

# Resultado

- Através dos valores obtidos dos experimentos podemos verificar a corretudo do programa.

- O resultados finais foram:

| threads | dimension | execution_time | performance_gain |
| ------- | ------- | ------- | ------- |
| 0 | 100000 | 0.00031650000000000005 | 0 |
| 0 | 10000000 | 0.0241617 | 0 |
| 0 | 1000000000 | 2.4493281 | 0 |
| 2 | 100000 | 0.0012495999999999998 | 0.2532810499359796 |
| 2 | 10000000 | 0.0190298 | 1.2696770328642446 |
| 2 | 1000000000 | 1.3515481 | 1.8122389428833496 |
| 4 | 100000 | 0.0015712 | 0.20143839103869657 |
| 4 | 10000000 | 0.013878699999999999 | 1.7409195385734977 |
| 4 | 1000000000 | 1.2719627999999998 | 1.9256287212173187 |


* threads with value 0 represent the sequential execution   
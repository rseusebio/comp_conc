# Introdução

Este programa consiste em ordernar linha a linha as litas de valores interiores em um determinado arquivo de entrada e escrever as resposta em um arquivo de saída.

Este programa foi escrito inteiramente em java, com exceção do script que gera o arquivo de entrada, escrito em python (input_file_generator.py).

# Execução do programa

## Script gerador do arquivo de entrada

Para executar o script basta executar o seguinte comando:

`python3.8 input_file_generator.py arg1 arg2`

O arg1 simboliza o inteiro que determina quantas linhas terá o arquivo.
O arg2 simboliza o inteiro que determina o tamanho de cada linha do arquivo.

## Programa principal

Compilar o programa Main.java dentro da pasta code:

`javac Main.java`

Em seguida, executá-lo passando a número de threads consumidoras, o do tamanho de cada linha, o arquivo de entrada e o arquivo de sáida. Exemplo:

`java Main 2 1000 input_file.txt output_file.txt`

## Shell script:

Existe um shell script usado para rodar os teste de cada arquivo de entrada.

`sh script.sh`


# Pasta de estatisticas
As pastas stats_*_** contém os resultados das execução realizadas através do shell script `script.sh`.
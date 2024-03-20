# Projeto Huffman | Estrutura de Dados

Projeto da disciplina de Estrutura de Dados sobre o algoritmo de Huffman. 2º semestre(2023.2) do curso de Engenharia da Computação na UFAL: Universidade Federal de Alagoas.

<p align="center">
  <img src="https://user-images.githubusercontent.com/91018438/204195385-acc6fcd4-05a7-4f25-87d1-cb7d5cc5c852.png" alt="animated" />
</p>

<center>
Integrantes:

  
    Robson de Paula | Francys Samuel | Athisson Alberto | Jader Rogerio
 </center>

## O que é o Algoritmo de Huffman?
A codificação de Huffman é um método de compressão que usa as probabilidades de ocorrência dos símbolos no conjunto de dados a ser comprimido para determinar códigos de tamanho variável para cada símbolo.

## Instalação do programa

<p>

> Clone esse projeto em seu computador com o comando:
> ```
> 	git clone https://github.com/Starbobinho/huffman_project
> ```
> Acesse a pasta do projeto seu terminal:
> ```
> 	cd [Nome da pasta onde você salvou]
> ```
> Já na pasta da aplicação em seu terminal, digite o seguinte comando:
> ```
> 	gcc -Iinc src/decoding.c src/encoding.c src/tree.c src/list.c src/main.c -o zip/zip
> ```
> 	Logo depois, basta executar com o seguinte comando:
> ```
>         ./zip/zip.exe
> ```
>   Para os testes do CUnit se faz necessário a instalação do mesmo, porém na mesma pasta que executou o comando acima pode ser executado o comando abaixo:
> ```
>    gcc -o unit_test src/decoding.c src/encoding.c src/list.c src/tree.c src/unit.c -Iinc -lcunit
> ```
>   Logo após isso, pode executar o seguinte comando:
> ```
>         ./unit_test
  </p>


## Execução do programa
<p> Ao inicializar o programa, você terá direito de escolha de 2 opções:
  
* Compactar um arquivo
* Descompactar o arquivo

# Obrigado pela atenção!

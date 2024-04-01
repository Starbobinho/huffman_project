#include <lista.h>
#include <arvore.h>
#include <string.h>
#include <stdlib.h>

#ifndef ENCODE_H
#define ENDODE_H

typedef struct bithuff{
    int bitH;
    int size;
}BitHuff;
/**
 * @brief Pega o tamanho de um árquivo
 * 
 * @param archive: O ponteiro que aponta para o Arquivo
 * @return O tamanho do arquivo em bytes na maior variável do c unsigne long long int
 */
unsigned long long int sizeArchive(FILE *archive);
/**
 * @brief Montar a Tabela que vai conter as informações dos bit que vão representar os caracteres(bits de Huffman)
 * 
 * @param tree_node: A raiz da árvore de Huffman
 * @param table: A tabela do que vai conter as informações dos bit de Huffman
 * @param code: A variável que vai armazenar os bits de Huffman e seu tamanho até chegar em um nó folha
 */
void construirTabelaCodigos(Node* tree_node,BitHuff table[],BitHuff code);
/**
 * @brief Retorna o tamanho do lixo no final do árquivo
 * 
 * @param frequency: Uma tabela com a frequência de cada caractere
 * @param table: A tabela do que contém as informações dos bit de Huffman
 * @return A quantidade de bits que não serão lidos no ultimo byte em inteiro
 */
int tamanhoLixo(int frequency[],BitHuff table[]);
/**
 * @brief Seta o primeiro byte do arquivo compactado
 * 
 * @param file: Um ponteiro para o arquivo compactado
 * @param trashSize: O tamanho do lixo
 * @param treeSize: O tamanho da árvore de Huffman
 */
void escreverPrimeiroByte(FILE *file,int trashSize,int treeSize);
/**
 * @brief Seta o segundo byte do arquivo compactado
 * 
 * @param file: Um ponteiro para o arquivo compactado
 * @param treesize: O tamanho da árvore de Huffman
 */
void escreverSegundoByte(FILE *file,int treesize);
/**
 * @brief Seta a árvore obtida em pré ordem
 * 
 * @param file: Um ponteiro para o arquivo compactado
 * @param bt: O Nó raiz da Árvore de Huffman
 */
void escreverArvore(FILE *file,struct Node *bt);
/**
 * @brief Escreve a extensão e o tamanho dela no arquivo compactado
 * 
 * @param file: ponteiro para o arquivo compactado
 * @param ext: String da extensão
 * @param tamanho: Tamanho da extensão do arquivo
 */
void escreverExt(FILE *file, char ext[], int tamanho);
/**
 * @brief Setar os bytes com os novos bits correspondentes de cada caractere
 * 
 * @param fileIn: Ponteiro para o árquivo original 
 * @param fileOut: Ponteiro para o arquivo compactado
 * @param table: A tabela do que contém as informações dos bit de Huffman
 * @param trashSize: O tamanho do lixo
 */
void escreverBytesCodificados(FILE *fileIn,FILE *fileOut,BitHuff table[],int trashSize);

#endif

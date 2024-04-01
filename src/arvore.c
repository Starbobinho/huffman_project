#include <stdio.h>
#include <lista.h>
#include <arvore.h>
#include <stdbool.h>


bool isLeaf(struct Node *node){
    return node->left == NULL && node->right == NULL;
}
/*
    monta a árvore de huffman com a lista ordenada
*/
void arvoreDeHuffman(struct Node **lista,int *tamanho_atual){
    while(*tamanho_atual > 1){
        int frequencia_nova = (*lista)->frequency + (*lista)->next->frequency;// soma das frequências dos primeiros nós

        struct Node *novo_Node = createNode(createVoidPointer('*'), frequencia_nova);
        novo_Node->left = *lista;// esquerda do novo nó vai receber o primeiro elemento
        removeFirst(lista, tamanho_atual);

        novo_Node->right = *lista;//direita do novo nó vai receber o segundo elemento
        removeFirst(lista, tamanho_atual);

        addInOrder(lista, novo_Node, tamanho_atual);// adiciona o novo nó de maneira ordenada
    }
    return;
}
/*
    acha o tamanho da árvore 
*/
int tamanhoArvore(struct Node *arvore_huffman){
    if(arvore_huffman == NULL){
        return 0;
    }
    else{
        int esquerda = tamanhoArvore(arvore_huffman->left);
        int direita = tamanhoArvore(arvore_huffman->right);

        unsigned char x = getByteFromVoidPointer(arvore_huffman->byte);

        // Se for uma folha e o byte for um '*' ou uma '\' contamos como 2 pois sempre terá o caractere de escape para representá-lo na arvore
        if(((x == '*') || (x == '\\')) && isLeaf(arvore_huffman)){
        return 2 + esquerda + direita;
    }
        return 1 + esquerda + direita;
    }
}

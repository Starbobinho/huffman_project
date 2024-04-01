#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <lista.h>

// Criar lista encadeada vazia
struct Node* createList() {
    return NULL;
}
struct Node *createNode(void *byte,int frequencia){
    struct Node* novo_Node = (struct Node*)malloc(sizeof(struct Node));
    novo_Node->next = NULL;
    novo_Node->left = NULL;
    novo_Node->right = NULL;
    novo_Node->byte = byte;
    novo_Node->frequency = frequencia;
    return novo_Node;
}
// Verificar se list esta vazia
bool isEmpty(struct Node** list) {
    return (*list == NULL);
}

void adquirirFrequencia(FILE *fileIn,int frequencia[]){
    unsigned char byte;
    while (fread(&byte, sizeof(unsigned char), 1, fileIn) == 1)
    {   
        // Soma 1 em cada respectivo byte em cada iteração na sua posição específica do array indo de 0 a 255
        frequencia[byte]++;
    }
}

// frequency = {0, 155, 0, 259, 0, 0, 168, 0...}

void *createVoidPointer(unsigned char byte) {
    unsigned char *pointer = malloc(sizeof(unsigned char*));
    *pointer = byte;
    return (void*) pointer;
}

void addInOrder(struct Node **lista,struct Node *novo_Node,int *tamanho_atual){
    if (*lista == NULL || novo_Node->frequency <= (*lista)->frequency){
        novo_Node->next = *lista;
        *lista = novo_Node;
    }
    // Aqui é para caso a inserção não ocorra no inicio
    else{
        struct Node* atual = *lista;
        while (atual->next != NULL && novo_Node->frequency > atual->next->frequency){
            atual = atual->next;
        }
        novo_Node->next = atual->next;
        atual->next = novo_Node;
    }
    (*tamanho_atual)++;
    return;
}

unsigned char getByteFromVoidPointer(struct Node *byte) {
    unsigned char *item = (unsigned char*) byte;
    return *item;
}

/* OBJETIVO: Inserir na lista encadeada lista a frequencia e o byte
             de forma ordenada na ordem crescente. */
void inserirFrequencia(struct Node** lista, int frequencia[],int *tamanho_atual) {
    for (int i = 0; i < 256; i++)
    {
        if (frequencia[i] > 0)
        {
            struct Node* novo_Node = createNode(createVoidPointer(i), frequencia[i]);
            addInOrder(lista,novo_Node,tamanho_atual);
        }
    }
}
unsigned char removeFirst(struct Node **lista, int *tamanho_atual){
    unsigned char item = getByteFromVoidPointer((*lista)->byte); 
    (*lista) = (*lista)->next;
    (*tamanho_atual)--;
    return item;
}
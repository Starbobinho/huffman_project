#include <stdio.h> // Padrão (scanf, printf)
#include <stdlib.h> // Para usar a operação malloc
#include <time.h> // Para usar a função time
#include <stdbool.h> // Para usar "valores" do tipo true, false e bool

// Definição da estrutura de um nó da árvore binária de busca
struct NoArvore {
    int numero;
    struct NoArvore* esquerda;
    struct NoArvore* direita;
};

// Definição da estrutura da lista encadeada
typedef struct no_lista
{
    int numero; // Valor armazenado no nó
    struct no_lista *proximo; // Ponteiro para o próximo nó na lista
} NoLista;

// Definição da estrutura da lista encadeada
typedef struct lista
{
    NoLista *cabeca; // Ponteiro para o primeiro nó da lista
} Lista;

// Função para criar um novo nó
struct NoArvore* criarNo(int numero) {
    struct NoArvore* novoNo = (struct NoArvore*)malloc(sizeof(struct NoArvore));
    novoNo->numero = numero;
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    return novoNo;
}

// Função para inserir um número na árvore
struct NoArvore* inserir(struct NoArvore* raiz, int numero) {
    if (raiz == NULL) {
        // Se a árvore estiver vazia, cria um novo nó e retorna
        return criarNo(numero);
    }
    
    // Se o número for menor que o valor do nó, insere na subárvore esquerda
    if (numero < raiz->numero) {
        raiz->esquerda = inserir(raiz->esquerda, numero);
    }
    // Se o número for maior que o valor do nó, insere na subárvore direita
    else if (numero > raiz->numero) {
        raiz->direita = inserir(raiz->direita, numero);
    }
    
    // Se o número for igual ao valor do nó, não faz nada (supondo que não são permitidos valores duplicados)
    
    return raiz;
}

// Função para inserir um novo nó no início da lista encadeada
void inserir_lista_inicio(NoLista **cabeca, int item)
{
    // Aloca memória para o novo nó
    NoLista *novo = (NoLista*)malloc(sizeof(NoLista));

    // Verifica se a alocação foi bem-sucedida
    if (novo != NULL)
    {
        // Configura os valores do novo nó
        novo->numero = item;
        novo->proximo = *cabeca;
        *cabeca = novo; // Atualiza a cabeça da lista para apontar para o novo nó
    }
    else
    {
        printf("ERRO ao alocar memória para o nó\n");
    }
}

// Função para buscar um número na árvore binária de busca
void buscar(struct NoArvore* raiz, int numero, int* comparacoes) {
    if (raiz == NULL) {
        return;
    }

    (*comparacoes)++; // Incrementa o contador de comparações

    if(raiz->numero == numero){
        return;
    }
    
    // Se o número for menor que o valor do nó, busca na subárvore esquerda
    if (numero < raiz->numero) {
        buscar(raiz->esquerda, numero, comparacoes);
    }
    // Se o número for maior que o valor do nó, busca na subárvore direita
    buscar(raiz->direita, numero, comparacoes);

}




// Função para buscar um número na lista encadeada
int buscar_lista(NoLista *cabeca, int item)
{
    int posicao = 1;

    // Percorre a lista a partir da cabeça
    while (cabeca != NULL)
    {
        if (cabeca->numero == item)
        {
            // Se o item for encontrado, retorna a posição atual
            return posicao;
        }
        // Move para o próximo nó na lista
        cabeca = cabeca->proximo;
        // Incrementa a posição
        posicao++;
    }

    // Se o item não for encontrado, retorna -1
    return posicao - 1;
}

// Função para liberar a memória de todos os nós da árvore
void liberar_arvore(struct NoArvore *raiz) 
{
    if (raiz != NULL) 
    {
        // Libera recursivamente a memória dos nós da subárvore esquerda
        liberar_arvore(raiz->esquerda);
        // Libera recursivamente a memória dos nós da subárvore direita
        liberar_arvore(raiz->direita);
        // Libera a memória do nó atual
        free(raiz);
    }
}

// Função para liberar a memória de todos os nós da lista
void liberar_lista(NoLista *cabeca) 
{
    NoLista *atual = cabeca;
    while (atual != NULL) 
    {
        NoLista *temp = atual; // Salva o ponteiro para o nó atual
        atual = atual->proximo; // Move para o próximo nó na lista
        free(temp); // Libera a memória do nó atual
    }
}

int generate_random(int lower, int upper) {

    int num = rand() % (upper - lower + 1); /* Gera um número aleatório no intervalo [lower, upper] */
    return num;
}

int main() 
{
    int numero_gerado;
    int numero_pesquisado;
    int posicao_arvore;
    int posicao_lista;

    FILE *posicoes_lista = fopen("posicoes_lista.txt", "w");
    FILE *posicoes_arvore = fopen("posicoes_arvore.txt", "w");
    // Inicializa as estruturas
    struct NoArvore *raiz = NULL;
    Lista lista;
    lista.cabeca = NULL;
    // Pergunta a quantidade de números aleatórios a serem gerados


    // Inicializa o gerador de números aleatórios com o tempo atual como semente
    srand(time(NULL));

    // Gera e insere os números aleatórios na árvore e na lista
    for (int i = 0; i < 100000; i += 100)
    {   
        struct NoArvore *raiz = NULL;
        lista.cabeca = NULL;
        for(int j = 0; j < i; j++){
            // Gera um número aleatório entre 1 e 100000
            numero_gerado = generate_random(1, 100000);

            // Insere o número aleatório na árvore
            raiz = inserir(raiz, numero_gerado);

            // Insere o número aleatório na lista
            inserir_lista_inicio(&lista.cabeca, numero_gerado);
        }
        numero_pesquisado = generate_random(1, 100000);

        int comparacoes_arv = 0;
        buscar(raiz, numero_pesquisado, &comparacoes_arv);
        //printf("%d <- Target\n", numero_gerado)
        
        //printf("PA = %d\n", posicao_arvore);
        posicao_lista = buscar_lista(lista.cabeca, numero_pesquisado);
        //printf("PL = %d\n", posicao_lista);

        fprintf(posicoes_lista, "%d\t%d\n", i, posicao_lista);
        fprintf(posicoes_arvore, "%d\t%d\n", i, comparacoes_arv);
    }
    printf("\n");
    fclose(posicoes_arvore);
    fclose(posicoes_lista);
    liberar_arvore(raiz);
    liberar_lista(lista.cabeca);
    
    return 0;
}